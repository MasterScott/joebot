// WorldGnome.h: interface for the CWorldGnome class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__WORLDGNOME_H)
#define __WORLDGNOME_H

//#include "LListElem.h"

#include "Gnome.h"

class CWorldGnome
{
public:
	virtual void Think(void);
	CLListElem * AddGnome(void);
	CLListElem * Add1stGnome(void);
	CWorldGnome();
	virtual ~CWorldGnome();

	CGnome *next;
};

#endif // !defined(__WORLDGNOME_H)
