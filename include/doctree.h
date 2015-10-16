#ifndef LIBDOCTREE_H
#define LIBDOCTREE_H

#include <stdio.h>

#ifdef _UNICODE
typedef wchar_t DTchar;

#ifndef _T
#define _T(s)		L ## s
#endif

#else
typedef char DTchar;

#ifndef _T
#define _T(s)		s
#endif

#endif

typedef struct _DTnode {
	DTchar				*label;
	struct _DTnode		*parent;
	struct _DTnode		**children;
	struct _DTattribute	**attributes;
	int					flags;
} DTnode;

typedef struct _DTattribute {
	DTchar				*key;
	DTnode				*parent;
	DTchar				*value;
	int					flags;
} DTattribute;

DTnode			*DTnewNode(DTnode *parent, const DTchar *label, int flags);
void			DTfreeNode(DTnode *node);
int				DTappendNode(DTnode *parent, DTnode *child);
int				DTchildCount(DTnode *node);
int				DTnodeHasChildren(DTnode *node);
int				DTnodeHasAtts(DTnode *node);

DTattribute		*DTnewAtt(const DTchar *key, const DTchar *val, int flags);
void			DTfreeAtt(DTattribute *att);
int				DTattCount(DTnode *node);
int				DTappendAtt(DTnode *node, DTattribute *att);
DTattribute		*DTgetAtt(DTnode *node, const DTchar *key);
int				DTsetAtt(DTnode *node, const DTchar *key, const DTchar *val, int flags);
#define			DTsetAttArray(node, key, val, flags)	DTsetAtt(node, key, val, DTATT_ARRAY | flags)

void			DTprintXml(FILE *f, DTnode *node, int flags);
void			DTprintJson(FILE *f, DTnode *node, int flags);

// node flags
#define DTNODE_DEFAULT		0x0
#define DTNODE_ARRAY		0x1 // node is an array

// attribute flags
#define DTATT_DEFAULT		0x0
#define DTATT_ARRAY			0x1 // attribute is a multistring array

// output flags
#define DTOUT_DEFAULT		0x0
#define DTOUT_WHITESPACE	0x1 // print whitespace

#endif // LIBDOCTREE_H
