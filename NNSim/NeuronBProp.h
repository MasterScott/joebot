// NeuronBProp.h: interface for the CNeuronBProp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__NEURONBPROP_H)
#define __NEURONBPROP_H
#include "BaseNeuronFF.h"
#include "LinkBProp.h"
#include "WeightBProp.h"

class CNeuronBProp : public CBaseNeuronFF  
{
	friend class CNeuralNetBProp;
public:
	virtual void Load(FILE *);
	virtual void Save(FILE *);
	CNeuronBProp();
	virtual ~CNeuronBProp();

	virtual CBaseLink *NewLink(void);

	virtual void Calc(void);
	virtual void CalcDelta(void);
protected:
};

#endif // !defined(__NEURONBPROP_H)
