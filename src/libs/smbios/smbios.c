/*
 * See:
 * http://www.dmtf.org/sites/default/files/standards/documents/DSP0134_2.8.0.pdf
 * http://www.opensource.apple.com/source/AppleSMBIOS/AppleSMBIOS-28/
 * https://github.com/RevoGirl/RevoBoot/blob/master/i386/libsaio/smbios/tools/smbios2struct.c
 * http://linux.dell.com/libsmbios/main/
 * http://download.microsoft.com/download/5/D/6/5D6EAF2B-7DDF-476B-93DC-7CF0072878E6/SMBIOS.doc
 */

#ifdef __APPLE__
#include <Carbon/Carbon.h>
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

#include <string.h>
 
#include "smbios.h"

/*
 * Returns a checksum for the given memory range to validate a SMBIOS 5.2.1
 * structure table entry point.
 */
static SMbyte checksum(void *b, size_t num)
{
	int		i = 0;
    SMbyte	csum = 0;
    SMbyte	*cp = (SMbyte *) b;

    for (i = 0; i < num; i++)
        csum += *cp++;

    return csum;
}

/*
 * Returns a pointer to a copy of the SMBIOS entry point or NULL on error.
 * It is the responsibility of the caller to free the result.
 */
SMentryPoint*
SMgetEntryPoint()
{
	SMentryPoint *out = NULL;

#ifdef __APPLE__
	mach_port_t		masterPort;
    io_service_t	service = MACH_PORT_NULL;
	CFDataRef		dataRef;
	CFIndex			dataLen;

	IOMasterPort(MACH_PORT_NULL, &masterPort);
	service = IOServiceGetMatchingService(masterPort, IOServiceMatching("AppleSMBIOS"));
    if (0 == service)
    	return NULL;

	dataRef = (CFDataRef) IORegistryEntryCreateCFProperty(service, 
														  CFSTR("SMBIOS-EPS"), 
														  kCFAllocatorDefault, 
														  kNilOptions);

	if (NULL == dataRef)
		return NULL;

	// allocate a new buffer
	dataLen = CFDataGetLength(dataRef);
	out = malloc(dataLen);
	if (NULL == out)
		return NULL;

	// copy data
	CFDataGetBytes (dataRef, CFRangeMake(0, dataLen), (UInt8*)out);

	// clean up
	CFRelease(dataRef);
	IOObjectRelease(service);
#endif

	// validate struct
	if (0 != memcmp(out->anchor, "_SM_", 4))
		return NULL;

	if (0 != checksum(out, sizeof(SMentryPoint)))
		return NULL;

	if (0 != memcmp(out->dmi.anchor, "_DMI_", 5))
		return NULL;

	if (0 != checksum(&out->dmi, sizeof(DMIentryPoint)))
		return NULL;

	return out;
}

/*
 * Return a pointer to the first SMBIOS table struct. This pointer should be
 * free by the caller.
 */
static SMstruct*
SMgetFirstStruct()
{
#if defined(_WIN32) // Windows
	DWORD bufferSize = 0;

	if (NULL != smbios)
		return smbios;

	// Get required buffer size
	bufferSize = GetSystemFirmwareTable('RSMB', 0, NULL, 0);
	if (bufferSize) {
		smbios = (PRAW_SMBIOS_DATA)LocalAlloc(LPTR, bufferSize);
		bufferSize = GetSystemFirmwareTable('RSMB', 0, (PVOID)smbios, bufferSize);
	}

	return smbios;

#elif defined(__APPLE__) // OS X
	mach_port_t				myMasterPort;
	CFMutableDictionaryRef	myMatchingDictionary;
	CFMutableDictionaryRef	properties = NULL;
	io_object_t				foundService;
	CFDataRef				smbiosdata;
	CFIndex					len;
	SMstruct				*data;

	IOMasterPort(MACH_PORT_NULL, &myMasterPort);

	myMatchingDictionary = IOServiceMatching("AppleSMBIOS");
	foundService = IOServiceGetMatchingService(myMasterPort, myMatchingDictionary);
	
	IORegistryEntryCreateCFProperties(foundService,
									  &properties,
									  kCFAllocatorDefault,
									  kNilOptions);
	
	CFDictionaryGetValueIfPresent(properties,
								  CFSTR( "SMBIOS"),
								  (const void **)&smbiosdata);
	
	len = CFDataGetLength(smbiosdata);
	data = malloc(sizeof(SMbyte) * len);
	if (NULL == data)
		return NULL;

	CFDataGetBytes(smbiosdata, CFRangeMake(0, len), (UInt8*)data);

	return data;
#else // hopefully a unix derivative with /dev/mem

#endif
}

SMstruct*
SMnextStruct(SMstruct *prev)
{
	SMstruct *next = NULL;
	SMbyte *c = NULL;

	// Return first SMBIOS structure
	if (NULL == prev)
		return SMgetFirstStruct();

	// Move to the end of the formatted structure
	c = ((SMbyte*) prev) + prev->length;
	
	// Search for the end of the unformatted structure (\0\0)
	while (1) {
		if ('\0' == *c && '\0' == *(c + 1)) {
			/* Make sure next table is not beyond end of SMBIOS data (Thankyou
			 * Microsoft for ommitting the structure count in
			 * GetSystemFirmwareTable lol)
			 */
			//if ((c + 2) < ((SMbyte*)smbios->SMBIOSTableData + smbios->Length))
			return (SMstruct*)(c + 2);
			//else
			//	return NULL; // We reached the end
		}
			
		c++;
	}

	return NULL;
}

SMstruct*
SMgetStructByType(SMstruct *s, SMbyte type)
{
	while (NULL != s && type != s->type)
		s = SMnextStruct(s);

	return s;
}

char*
SMgetString(SMstruct *s, SMbyte index)
{
	SMbyte	i = 0;
	char	*c = NULL;

	// iterate each string in the struct payload
	for (i = 1, c = (char *)s + s->length; *c; c += strlen(c) + 1, i++)
		if (i == index)
			return c;

	return NULL;
}