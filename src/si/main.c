#include <stdlib.h>
#include <stdio.h>

#include "doctree.h"
#include "smbios.h"

// 7.5.1 Processor Information — Processor Type 
static DTchar* SMPROC_TYPES[] = {
	_T("Invalid"),						// 0x00 Invalid
	_T("Other"),						// 0x01 Other
	_T("Unknown"),						// 0x02 Unknown
	_T("Central Processor"),			// 0x03 Central Processor
	_T("Math Processor"),				// 0x04 Math Processor
	_T("DSP Processor"),				// 0x05 DSP Processor
	_T("Video")							// 0x06 Video Processor
};

// 7.5.9 Processor Characteristics 
static DTchar* SMPROC_CHARACTERISTICS[] = {
	_T("Reserved"),						// Bit 0
	_T("Unknown"),						// Bit 1
	_T("64-bit Capable"),				// Bit 2
	_T("Multi-Core"),					// Bit 3
	_T("Hardware Thread"),				// Bit 4
	_T("Execute Protection"),			// Bit 5
	_T("Enhanced Virtualization"),		// Bit 6
	_T("Power/Performance Control")		// Bit 7
};

#define BUFLEN	4096

int main(int argc, char* argv[])
{
	DTnode *doc, *nHardware, *nSmbios, *nSystem, *nProcessor, *nBios;
	SMstruct *t1, *tn;
	int i = 0;
	char *c = NULL;
	char buf[BUFLEN];

	// SMBIOS entry point
	SMentryPoint *ep = SMgetEntryPoint();

	t1 = SMfirstStruct();

	doc = DTnewNode(NULL, _T("Inventory"), 0);

		snprintf(buf, BUFLEN, "%p", t1);
		DTsetAtt(doc, _T("Location"), buf, 0);

	// System
	tn = SMgetStructByType(t1, 0x01);
	nSystem = DTnewNode(doc, _T("System"), 0);
	DTsetAtt(nSystem, _T("Manufacturer"), SMgetStringAtOffset(tn, 0x04), 0);
	DTsetAtt(nSystem, _T("ProductName"), SMgetStringAtOffset(tn, 0x05), 0);
	DTsetAtt(nSystem, _T("Version"), SMgetStringAtOffset(tn, 0x06), 0);
	DTsetAtt(nSystem, _T("SerialNumber"), SMgetStringAtOffset(tn, 0x07), 0);
	DTsetAtt(nSystem, _T("SKUNumber"), SMgetStringAtOffset(tn, 0x19), 0);
	DTsetAtt(nSystem, _T("Family"), SMgetStringAtOffset(tn, 0x1A), 0);

	nHardware = DTnewNode(doc, _T("Hardware"), 0);

	// SMBIOS
	nSmbios = DTnewNode(nHardware, _T("Smbios"), 0);
	if (NULL != ep) {
		snprintf(buf, BUFLEN, "%i.%i", ep->majorVersion, ep->minorVersion);
		DTsetAtt(nSmbios, _T("Version"), buf, 0);

		snprintf(buf, BUFLEN, "0x%0lX", ep->entryPointLength);
		DTsetAtt(nSmbios, _T("EntryPointLength"), buf, 0);

		snprintf(buf, BUFLEN, "0x%0lX", ep->dmi.tableAddress);
		DTsetAtt(nSmbios, _T("Address"), buf, 0);

		snprintf(buf, BUFLEN, "%i", ep->maxStructureSize);
		DTsetAtt(nSmbios, _T("MaxStructureSize"), buf, 0);

		snprintf(buf, BUFLEN, "%i", ep->dmi.tableLength);
		DTsetAtt(nSmbios, _T("TableLength"), buf, 0);

		snprintf(buf, BUFLEN, "%i", ep->dmi.structureCount);
		DTsetAtt(nSmbios, _T("StructureCount"), buf, 0);

		snprintf(buf, BUFLEN, "%i", ep->dmi.bcdRevision);
		DTsetAtt(nSmbios, _T("BcdRevision"), buf, 0);
	}

	// Processor
	tn = SMgetStructByType(t1, 0x04);
	nProcessor = DTnewNode(nHardware, _T("Processor"), 0);
	DTsetAtt(nProcessor, "Socket", SMgetStringAtOffset(tn, 0x04), 0);
	DTsetAtt(nProcessor, "Manufacturer", SMgetStringAtOffset(tn, 0x07), 0);
	DTsetAtt(nProcessor, "Version", SMgetStringAtOffset(tn, 0x10), 0);
	DTsetAtt(nProcessor, "SerialNumber", SMgetStringAtOffset(tn, 0x20), 0);
	DTsetAtt(nProcessor, "AssetTag", SMgetStringAtOffset(tn, 0x21), 0);
	DTsetAtt(nProcessor, "PartNumber", SMgetStringAtOffset(tn, 0x22), 0);
	DTsetAtt(nProcessor, "Type", SMPROC_TYPES[SMbyteAtOffset(tn, 0x05)], 0);

	/*
	for (i = 0; i < 8; i++)
		if (0 != (SMbyteAtOffset(tn, 0x26) & i))
			DTsetAttAppend(nProcessor, "Characteristics", SMPROC_CHARACTERISTICS[SMbyteAtOffset(tn, 0x26) & (1 << i)],0);
	*/

	// BIOS
	tn = SMgetStructByType(t1, 0x00);
	nBios = DTnewNode(nHardware, _T("Bios"), 0);
	DTsetAtt(nBios, "Vendor", SMgetStringAtOffset(tn, 0x04), 0);
	DTsetAtt(nBios, "Version", SMgetStringAtOffset(tn, 0x05), 0);
	DTsetAtt(nBios, "ReleaseDate", SMgetStringAtOffset(tn, 0x08), 0);

	// print
	DTprintJson(stdout, doc, DTOUT_WHITESPACE);
	
	free(t1);
	free(ep);

    return 0;
}