/******************************************************************************

    JoeBOT - a bot for Counter-Strike
    Copyright (C) 2000-2002  Johannes Lampel

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

******************************************************************************/
#ifndef __ACTIVATIONS_H
#define __ACTIVATIONS_H

#include "NeuralNetDefs.h"

bool SetAct(int iType, int iTypeoC,netvar_t (*&p)(const netvar_t &), netvar_t (*&pS)(const netvar_t &));

#define TOAC_DEFAULT	0

#define TOAC_HASH		0
#define TOAC_DIRECT		1

#define NEURON_ACT_TANH 1
netvar_t fNActtanh(const netvar_t &dParam);
netvar_t fNActtanhS(const netvar_t &dParam);

netvar_t foNActtanh(const netvar_t &dParam);
netvar_t foNActtanhS(const netvar_t &dParam);

#define NEURON_ACT_LOG 2
netvar_t fNActlog(const netvar_t &dParam);
netvar_t fNActlogS(const netvar_t &dParam);

netvar_t foNActlog(const netvar_t &dParam);
netvar_t foNActlogS(const netvar_t &dParam);

#endif  __ACTIVATIONS_H