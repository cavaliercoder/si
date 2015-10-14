#include "doctree.h"
#include "smbios.h"

/*
 * See:
 * http://www.dmtf.org/sites/default/files/standards/documents/DSP0134_2.8.0.pdf
 */
 
PRAW_SMBIOS_DATA smbios = NULL;

PRAW_SMBIOS_DATA GetSmbiosData()
{
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
}

void ReleaseSmbiosData()
{
	if (NULL != smbios) {
		LocalFree(smbios);
		smbios = NULL;
	}
}

PSMBIOS_STRUCT_HEADER GetNextStructure(PSMBIOS_STRUCT_HEADER previous)
{
	PSMBIOS_STRUCT_HEADER next = NULL;
	PBYTE c = NULL;

	// Init SMBIOS data
	if (NULL == smbios)
		smbios = GetSmbiosData();

	// Return NULL is no data found
	if (NULL == smbios)
		return NULL;
	
	// Return first table if previous was NULL
	if (NULL == previous)
		return (PSMBIOS_STRUCT_HEADER)(&smbios->SMBIOSTableData[0]);

	// Move to the end of the formatted structure
	c = ((PBYTE)previous) + previous->Length;
	
	// Search for the end of the unformatted structure (\0\0)
	while (true) {
		if ('\0' == *c && '\0' == *(c + 1)) {
			/* Make sure next table is not beyond end of SMBIOS data
			 * (Thankyou Microsoft for ommitting the structure count
			 * in GetSystemFirmwareTable
			 */
			if ((c + 2) < ((PBYTE)smbios->SMBIOSTableData + smbios->Length))
				return (PSMBIOS_STRUCT_HEADER)(c + 2);
			else
				return NULL; // We reached the end
		}
			
		c++;
	}

	return NULL;
}

PSMBIOS_STRUCT_HEADER GetNextStructureOfType(PSMBIOS_STRUCT_HEADER previous, DWORD type)
{
	PSMBIOS_STRUCT_HEADER next = previous;
	while (NULL != (next = GetNextStructure(next))) {
		if (type == next->Type)
			return next;
	}

	return NULL;
}

PSMBIOS_STRUCT_HEADER GetStructureByHandle(WORD handle)
{
	PSMBIOS_STRUCT_HEADER header = NULL;

	while (NULL != (header = GetNextStructure(header)))
		if (handle == header->Handle)
			return header;

	return NULL;
}