/*
 * See:
 * http://www.dmtf.org/sites/default/files/standards/documents/DSP0134_2.8.0.pdf
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