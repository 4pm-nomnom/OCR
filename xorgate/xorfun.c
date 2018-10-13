#include "xorfun.h"

//This is the function xor, to check is the neuralnetwork behaves correctly
//Returns 0 if a and b are both 0 or both not-0
//Returns 1 if only a or only b is 0 
float xorfun(float a, float b)
{
	//the first control node, which returns 1 iff a is 0 and b is not 0	
	float hidnd1 = 1.0f;
	{
		//a is signed => a is not 0 <=> it is stricly positive or stricly negative
		if (a > 0.0f)
		{	
			hidnd1 = 0.0f;	
			return hidnd1;
		// a is not 0 => we want to return 0
		}

		if (a < 0.0f)
		{
			hidnd1 = 0.0f;
			return hidnd1;
		}

		//if this state is reached => a is 0
		if (b == 0.0f)
		{
			hidnd1 = 0.0f;
			return hidnd1;
		}
		//if this state is reached => a is 0 and b is not => 1 is returned
		return hidnd1;
		

	}
	//the second control node, which returns 1 iff a is not 0 and b is 0
	float hidnd2 = 1.0f;
	{
		//returns 0 if a is 0.
		if (a == 0.0f)
		{
			hidnd2 = 0.0f;
			return hidnd2;
		}
		//a is not 0
		if (b > 0.0f)
		{
			hidnd2 = 0.0f;
			return hidnd2;
		}

		if (b < 0.0f)
		{	
			hidnd2 = 0.0f;
			return hidnd2;
		}
		
	}

	//the  output, we check if a control node returns 1 (since both cannot return 1 at the same time)
	float res = hidnd1 + hidnd2;
	return res;

}
