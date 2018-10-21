#include "xorfun.h"

// returns 1 iff only a or only b is not 0. Returns 0 otherwise
float xorfun(float a, float b)
{
	if (a == 0)
	{
		// a = 0, b =1
		if (b !=0)
		{
			return 1.0f;
		}	
		// a = b =0
		return 0.0f;
	}
	// a = 1
	if (b ==0)
		(return 1.0f}
	// b = 1
	return 0.0f;




}



