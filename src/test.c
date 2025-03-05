#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "test.h"
 
#include "cglm/types.h"


 
void print_me(){

	float r1 = 1;
	
	float pi = 3.14159265359;
	float rad = pi/180 ;
	float r2 = pow(r1+r1, 0.5);
	
	float delta = r2-r1;
	printf("r1 %f r2 %f \n",r1,r2);
	float adeg = 0.;
 
	//for(float adeg =0;adeg<=360.;adeg+=45./4 )
	while(adeg<=360.)
	{
	
		
		float arad =adeg * rad;
		 
		float r3 = (1+ (delta)*fabs(sin(2*arad)));
		float x =   cos(arad);
		float y =   sin(arad);
		printf("angle: d:%8.3f rad:%6.3f r3:%6.3f x:%6.3f y:%6.3f (1-x):%6.3f (1-y):%6.3f\n",adeg,arad,r3,x,y,1-x,1-y);
		adeg+=45./4;
	}
	 
}