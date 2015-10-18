#ifndef SMBIOS_H
#define SMBIOS_H

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
#pragma pack(1)             // enable 8-bit struct packing

typedef struct _DMIentryPoint {
    SMbyte    anchor[5];
    SMbyte    checksum;
    SMword    tableLength;
    SMdword   tableAddress;
    SMword    structureCount;
    SMbyte    bcdRevision;
} DMIentryPoint;

typedef struct _SMentryPoint {
    SMbyte    anchor[4];
    SMbyte    checksum;
    SMbyte    entryPointLength;
    SMbyte    majorVersion;
    SMbyte    minorVersion;
    SMword    maxStructureSize;
    SMbyte    entryPointRevision;
    SMbyte    formattedArea[5];
    DMIentryPoint   dmi;
} SMentryPoint;

typedef struct _SMstruct
{
	SMbyte type;
	SMbyte length;
	SMword handle;
	SMbyte data[1];
} SMstruct;

/*
 * Functions
 */
SMentryPoint	*SMgetEntryPoint();
SMstruct		*SMnextStruct(SMstruct *prev);
SMstruct		*SMgetStructByType(SMstruct *s, SMbyte type);
char			*SMgetString(SMstruct *s, SMbyte index);

#define 		SMfirstStruct()	SMnextStruct(NULL)

#define SMvalAtOffet(type, s, offset)	*((type *)((SMbyte*)s + offset))
#define SMbyteAtOffset(s, offset)		*((SMbyte*)((SMbyte*)s + offset))
#define SMwordAtOffset(s, offset)		*((SMword*)((SMbyte*)s + offset))
#define SMdwordAtOffset(s, offset)		*((SMdword*)((SMbyte*)s + offset))
#define SMqwordAtOffset(s, offset)		*((SMqword*)((SMbyte*)s + offset))

#define SMgetStringAtOffset(s, offset)	SMgetString(s, *((SMbyte*)(SMbyte*)s + offset))

#endif
