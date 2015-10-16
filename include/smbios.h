#ifndef SMBIOS_H
#define SMBIOS_H

/*
 * See:
 * http://www.dmtf.org/sites/default/files/standards/documents/DSP0134_2.8.0.pdf
 * http://linux.dell.com/libsmbios/main/
 */

 #include <stdint.h>
 #include <unistd.h>

// 1byte type
typedef unsigned char SMbyte;

// 32bit Word type
typedef unsigned long int SMword;

// 64bit Word type
typedef unsigned long long SMqword;

/*
 * Structures
 */
typedef struct _SMstruct
{
	SMbyte type;
	SMbyte length;
	SMword handle;
} SMstruct;

/*
 * Functions
 */
SMstruct	*SMnextStruct(SMstruct *prev);
char		*SMgetString(SMstruct *s, SMbyte index);

#define 	SMfirstStruct()	SMnextStruct(NULL)

#define SMvalAtOffet(type, s, offset)	*((type *)((SMbyte*)s + offset))
#define SMbyteAtOffset(s, offset)		*((SMbyte*)(SMbyte*)s + offset)
#define SMwordAtOffset(s, offset)		*((SMword*)((SMbyte*)s + offset))
#define SMdwordAtOffset(s, offset)		*((SMword*)((SMbyte*)s + offset))
#define SMqwordAtOffset(s, offset)		*((SMqword*)((SMbyte*)s + offset))

#endif
