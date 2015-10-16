#include <stdlib.h>
#include <stdio.h>

#include "doctree.h"
#include "smbios.h"

// http://www.dmtf.org/sites/default/files/standards/documents/DSP0134_2.8.0.pdf
// https://developer.apple.com/library/mac/documentation/Porting/Conceptual/PortingUnix/compiling/compiling.html
// http://www.opensource.apple.com/source/AppleSMBIOS/AppleSMBIOS-38/AppleSMBIOS.cpp

int main(int argc, char* argv[])
{
	DTnode *doc, *nHardware, *nProcessor;
	SMstruct *t = SMfirstStruct();

	doc = DTnewNode(NULL, _T("Inventory"), 0);
	nHardware = DTnewNode(doc, _T("Hardware"), 0);


	nProcessor = DTnewNode(nHardware, _T("Processor"), 0);
	DTsetAtt(nProcessor, "Socket", SMgetString(t, SMbyteAtOffset(t, 0x04)), 0);
	DTsetAtt(nProcessor, "Manufacturer", SMgetString(t, SMbyteAtOffset(t, 0x07)), 0);
	DTsetAtt(nProcessor, "Version", SMgetString(t, SMbyteAtOffset(t, 0x10)), 0);
	DTsetAtt(nProcessor, "SerialNumber", SMgetString(t, SMbyteAtOffset(t, 0x20)), 0);
	DTsetAtt(nProcessor, "AssetTag", SMgetString(t, SMbyteAtOffset(t, 0x21)), 0);
	DTsetAtt(nProcessor, "PartNumber", SMgetString(t, SMbyteAtOffset(t, 0x22)), 0);

	// print
	DTprintJson(stdout, doc, DTOUT_WHITESPACE);
	
    return 0;
}