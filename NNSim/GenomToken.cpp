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
#include "GenomToken.h"

short psTokenDataSize[GT_NUMBEROFTOKEN]={
	0,					// because the flags begin with 1
	sizeof(short)
	+ sizeof(short),	//GT_NetType
	sizeof(short)
	+ sizeof(short),	//GT_NumLayer,
	sizeof(short)
	+ sizeof(short)
	+ sizeof(int),		//GT_NumNeuronsOL,
	sizeof(short),		//GT_Alloc,
	sizeof(short)
	+sizeof(short)
	+sizeof(int)
	+sizeof(short)
	+sizeof(int)
	+sizeof(int),		//GT_ConnectNeurons,
	sizeof(short)
	+sizeof(short)
	+sizeof(int)
	+sizeof(int),		//GT_SetNeuronBias,
	sizeof(short)		//GT_End
};
