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
// Neighborhood kernel functions for SOMs
// by Johannes Lampel / LampelSoft
// (c) Johannes Lampel / LampelSoft
// written Sept 13rd 2000

// z -> Distance of neurons
// d -> size of neighborhood kernel
#include <math.h>
#include "GlobalCqSOMDefs.h"

#define pi 3.14159265358979

double DllExport fNKcos(const double &z,const double &d){	
	if(z < d)
		return (cos( z/d * pi/2.0));
	else
		return 0;
}

double DllExport fNKgauss1(const double &z,const double &d){
	return exp(-((z*z)/(d*d)));									// don't use pow(?,2),'cause it's to time expensive
}

double DllExport fNKgauss2(const double &z,const double &d){
	return (1-((z*z)/(d*d)))*fNKgauss1(z,d);
}

double DllExport fNKcyl(const double &z,const double &d){
	if(z < d)
		return 1.0;
	else
		return 0.0;
}

double DllExport fNKcone(const double &z,const double &d){
	if(z < d)
		return 1.0-(z/d);
	else
		return 0.0;
}