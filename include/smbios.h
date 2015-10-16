#ifndef SMBIOS_H
#define SMBIOS_H

/*
 * See:
 * http://www.dmtf.org/sites/default/files/standards/documents/DSP0134_2.8.0.pdf
 * http://linux.dell.com/libsmbios/main/
 */

 #include <stdint.h>
 #include <unistd.h>

// 8bit BYTE type
typedef unsigned char SMbyte;

// 16bit WORD type
typedef unsigned short int SMword;

// 32bit DWORD type
typedef unsigned long int SMdword;

// 64bit QWORD type
typedef unsigned long long SMqword;

/*
 * Structures
 */
typedef struct _SMstruct
{
	SMbyte type;
	SMbyte length;
	SMword handle;
	SMbyte *data;
} SMstruct;

/*
 * Functions
 */
SMstruct	*SMnextStruct(SMstruct *prev);
SMstruct	*SMgetStructByType(SMstruct *s, SMbyte type);
char		*SMgetString(SMstruct *s, SMbyte index);

#define 	SMfirstStruct()	SMnextStruct(NULL)

#define SMvalAtOffet(type, s, offset)	*((type *)((SMbyte*)s + offset))
#define SMbyteAtOffset(s, offset)		*((SMbyte*)((SMbyte*)s + offset))
#define SMwordAtOffset(s, offset)		*((SMword*)((SMbyte*)s + offset))
#define SMdwordAtOffset(s, offset)		*((SMdword*)((SMbyte*)s + offset))
#define SMqwordAtOffset(s, offset)		*((SMqword*)((SMbyte*)s + offset))

#define SMgetStringAtOffset(s, offset)	SMgetString(s, *((SMbyte*)(SMbyte*)s + offset))

#endif
