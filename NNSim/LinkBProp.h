// LinkBProp.h: interface for the CLinkBProp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__LINKBPROP_H)
#define __LINKBPROP_H

#include "BaseLinkFF.h"

class CLinkBProp  : public CBaseLinkFF
{
public:
	virtual void Load(FILE *fhd);
	virtual void Save(FILE *fhd);
	CLinkBProp();
	virtual ~CLinkBProp();

};

#endif // !defined(__LINKBPROP_H)
