#include <stdlib.h>
#include <stdio.h>

#include "doctree.h"
#include "smbios.h"

// http://www.dmtf.org/sites/default/files/standards/documents/DSP0134_2.8.0.pdf
// https://developer.apple.com/library/mac/documentation/Porting/Conceptual/PortingUnix/compiling/compiling.html
// http://www.opensource.apple.com/source/AppleSMBIOS/AppleSMBIOS-38/AppleSMBIOS.cpp

int main(int argc, char* argv[])
{
	DTnode *doc, *nHardware, *nSystem, *nProcessor, *nBios;

	SMstruct *t1, *tn;

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

	tn = SMgetStructByType(t1, 0x00);
	nBios = DTnewNode(nHardware, _T("Bios"), 0);
	DTsetAtt(nBios, "Vendor", SMgetStringAtOffset(tn, 0x04), 0);
	DTsetAtt(nBios, "Version", SMgetStringAtOffset(tn, 0x05), 0);
	DTsetAtt(nBios, "ReleaseDate", SMgetStringAtOffset(tn, 0x08), 0);

	// print
	DTprintJson(stdout, doc, DTOUT_WHITESPACE);
	
    return 0;
}