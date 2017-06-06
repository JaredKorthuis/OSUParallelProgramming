#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


struct s
{
	float value;
	int pad[NUM];
} Array[4];

float Func(){
    return 2.0;
   }

int main(){
	omp_set_num_threads( NUMT );
    double time0 = omp_get_wtime( );

	unsigned int someBigNumber = 100000000;	// if > 4B, use "long unsigned int"

	#pragma omp parallel for
	for( int i = 0; i < 4; i++ )
	{
		for( unsigned int j = 0; j < someBigNumber; j++ )
		{
			Array[ i ].value = Array[ i ].value + Func();
		}
	}
    double time1 = omp_get_wtime( );
    double total_time =(time1 - time0);
    printf("%lf\n",total_time); 
	return 0;
}

