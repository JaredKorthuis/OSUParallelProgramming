#include <simd.p5.h>
#include <omp.h>
#include <stdio.h>
#include <math.h>


#define XMIN	 0.
#define XMAX	 3.
#define YMIN	 0.
#define YMAX	 3.

#define Z00	0.
#define Z10	1.
#define Z20	0.
#define Z30	0.

#define Z01	1.
#define Z11	6.
#define Z21	1.
#define Z31	0.

#define Z02	0.
#define Z12	1.
#define Z22	0.
#define Z32	4.

#define Z03	3.
#define Z13	2.
#define Z23	3.
#define Z33	3.

//#define NUMT    1 
//#define NUMS    10
#define NUMTRIES 10

float
Height( int iu, int iv )	// iu,iv = 0 .. NUMS-1
{
	float u = (float)iu / (float)(NUMS-1);
	float v = (float)iv / (float)(NUMS-1);

	// the basis functions:

	float bu0 = (1.-u) * (1.-u) * (1.-u);
	float bu1 = 3. * u * (1.-u) * (1.-u);
	float bu2 = 3. * u * u * (1.-u);
	float bu3 = u * u * u;

	float bv0 = (1.-v) * (1.-v) * (1.-v);
	float bv1 = 3. * v * (1.-v) * (1.-v);
	float bv2 = 3. * v * v * (1.-v);
	float bv3 = v * v * v;

	// finally, we get to compute something:

	float height = 	  bu0 * ( bv0*Z00 + bv1*Z01 + bv2*Z02 + bv3*Z03 )
			+ bu1 * ( bv0*Z10 + bv1*Z11 + bv2*Z12 + bv3*Z13 )
			+ bu2 * ( bv0*Z20 + bv1*Z21 + bv2*Z22 + bv3*Z23 )
			+ bu3 * ( bv0*Z30 + bv1*Z31 + bv2*Z32 + bv3*Z33 );

	return height;
}

float Height( int, int );

int main( int argc, char *argv[ ] )
{
	omp_set_num_threads( NUMT );
	double maxmults=0;
    double summmults=0;
    //double mah_time =0;	
	// the area of a single full-sized tile:

	float fullTileArea = (  ( (XMAX-XMIN)/(float)(NUMS-1) )  *  ( ( YMAX - YMIN )/(float)(NUMS-1) )  );
	

	// sum up the weighted heights into the variable "volume"
	
	// using an OpenMP for loop and a reduction:
	for(int t=0; t < NUMTRIES; t++){
        double volume =0;
        double mah_numz =0;
		double time0 = omp_get_wtime();
	
	#pragma omp parallel for reduction(+:volume),private(mah_numz)
	   for( int i = 0; i < NUMS*NUMS; i++ )
	   {
		
		   int iu = i % NUMS;
		   int iv = i / NUMS;
		   mah_numz=Height(iu, iv);
		   if((   iu == 0 && iv == 0) || (iu == 0 && iv == NUMS-1) || (iu == NUMS-1 && iv == NUMS-1) || (iu == NUMS -1 && iv == 0)){
			volume += (mah_numz * (fullTileArea/4.));
           }
           else if(   (iu == 0 && (iv != 0 || iv != NUMS-1))|| (iv == 0 && (iu != 0 || iu != NUMS-2))|| (iu == NUMS-1 && (iv != 0 || iv != NUMS-1))|| (iv == NUMS-1 && (iu != 0 || iu != NUMS-1))){
               volume += (mah_numz * (fullTileArea/2.));
           }
           else{
               volume += mah_numz* (fullTileArea);
           }
	   }
	   double time1 = omp_get_wtime();
	   double mmults = (NUMS*NUMS)/(time1-time0)/1000000;
	   printf("mah_time =%f\n", (time1-time0));
       summmults += mmults;
	   if(mmults > maxmults){
		   maxmults = mmults;
	   }
	}
	printf( "   Peak Performance = %8.2lf MegaHeights/Sec\n", maxmults );
    printf( "Average Performance = %8.2lf MegaHeights/Sec\n", summmults/(double)NUMTRIES );
    printf("NUMS = %d\n",NUMS);
    printf("NUMT = %d\n",NUMT);
   // printf("mah time = %d\n",mah_time);
    
    return 0;
}
