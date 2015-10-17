#include <stdlib.h>
#include <stdio.h>

#include "doctree.h"
#include "smbios.h"

// http://www.dmtf.org/sites/default/files/standards/documents/DSP0134_2.8.0.pdf
// https://developer.apple.com/library/mac/documentation/Porting/Conceptual/PortingUnix/compiling/compiling.html
// http://www.opensource.apple.com/source/AppleSMBIOS/AppleSMBIOS-38/AppleSMBIOS.cpp

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


int main(int argc, char* argv[])
{
	DTnode *doc, *nHardware, *nSystem, *nProcessor, *nBios;
	SMstruct *t1, *tn;
	int i = 0;
	char *c = NULL;

	t1 = SMfirstStruct();

	doc = DTnewNode(NULL, _T("Inventory"), 0);

	tn = SMgetStructByType(t1, 0x01);
	nSystem = DTnewNode(doc, _T("System"), 0);
	DTsetAtt(nSystem, _T("Manufacturer"), SMgetStringAtOffset(tn, 0x04), 0);
	DTsetAtt(nSystem, _T("ProductName"), SMgetStringAtOffset(tn, 0x05), 0);
	DTsetAtt(nSystem, _T("Version"), SMgetStringAtOffset(tn, 0x06), 0);
	DTsetAtt(nSystem, _T("SerialNumber"), SMgetStringAtOffset(tn, 0x07), 0);
	DTsetAtt(nSystem, _T("SKUNumber"), SMgetStringAtOffset(tn, 0x19), 0);
	DTsetAtt(nSystem, _T("Family"), SMgetStringAtOffset(tn, 0x1A), 0);

	nHardware = DTnewNode(doc, _T("Hardware"), 0);

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

	tn = SMgetStructByType(t1, 0x00);
	nBios = DTnewNode(nHardware, _T("Bios"), 0);
	DTsetAtt(nBios, "Vendor", SMgetStringAtOffset(tn, 0x04), 0);
	DTsetAtt(nBios, "Version", SMgetStringAtOffset(tn, 0x05), 0);
	DTsetAtt(nBios, "ReleaseDate", SMgetStringAtOffset(tn, 0x08), 0);

	// print
	DTprintJson(stdout, doc, DTOUT_WHITESPACE);
	
	free(t1);

    return 0;
}