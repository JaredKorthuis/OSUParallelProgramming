#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<stdio.h>
#include<iostream>
#include<omp.h>

using namespace std;

int	NowYear;		// 2016 - 2021
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population

const float GRAIN_GROWS_PER_MONTH =		8.0;
const float ONE_DEER_EATS_PER_MONTH =		0.5;

const float AVG_PRECIP_PER_MONTH =		6.0;
const float AMP_PRECIP_PER_MONTH =		6.0;
const float RANDOM_PRECIP =			2.0;

const float AVG_TEMP =				50.0;
const float AMP_TEMP =				20.0;
const float RANDOM_TEMP =			10.0;

float tempFactor;
float precipFactor;
int my_aliens =0;
float
Ranf( float low, float high )
{
        float r = (float) rand( );               // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

float Get_Temp(){
	float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

	float temp = AVG_TEMP - AMP_TEMP * cos( ang );
	NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP );
	tempFactor = pow(-((NowTemp - AVG_TEMP)/10),2);
	
	
	return temp;
}
float Get_Precip(){
	float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP );
	precipFactor = pow(-((NowPrecip-AVG_PRECIP_PER_MONTH)/10),2);
	if( NowPrecip < 0. ){
		NowPrecip = 0.;
	}
	return NowPrecip;
}
int Get_Alien(){
    int s = rand()%6+1;
    //cout<<"alien rand is:  "<<s<<endl;
    if(s == 5){
        return 0;
    }
    else{
        return 1;
    }
    
}
/**************************************************************************
 * MY AGENT: ALIENS HAVE COME IN! If they want to send a message(20% chance)
 * They make a crop circle ruining half the grain.
 * if they find an odd number of deer, they take a deer.
 **************************************************************************/
void MyAgent(){
	NowMonth =0;
	NowYear = 2016;
	NowNumDeer = 1;
	NowHeight =  1.;
	while( NowYear <= 2021 ){
	  	
	   // compute a temporary next-value for this quantity based on the current state of the simulation:
	   

	   // DoneComputing barrier:
	   #pragma omp barrier
	   

	   // DoneAssigning barrier:
	   #pragma omp barrier
        //cout<<"JJJJAAAARRREEEEEDDDDD"<<endl;
        
        if(Get_Alien() ==0){
          my_aliens+=5;

          //cout<<"ALIENS HAVE MADE A CROP CIRCLE"<<endl;
          NowHeight = NowHeight/2;
        }

        if(NowNumDeer % 6 == 0 ){
          NowNumDeer--;
          my_aliens++;
          //cout<<"ALIENS TOOK A DEER!"<<endl;
          if(NowNumDeer <0){
              NowNumDeer=0;

          }

          
        }
        if(NowNumDeer ==0){
            NowNumDeer += 2;
            my_aliens-=5;
            //cout<<"ALIENS RETURNED 2 DEER"<<endl;
        }
	

		// DonePrinting barrier:
	   #pragma omp barrier
       
       

		
	}
}
void Change_Heights(){
	NowHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
	NowHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
    if(NowHeight < 0){
        NowHeight =0;
    }
}
void Watcher(){
	NowMonth =0;
	NowYear = 2016;
	NowNumDeer = 1;
	NowHeight =  1.;
		while( NowYear <= 2021 )
	{
	    // compute a temporary next-value for this quantity based on the current state of the simulation:
	

	    // DoneComputing barrier:
	    #pragma omp barrier
	

	    // DoneAssigning barrier:
	    #pragma omp barrier
	

	    // DonePrinting barrier:
	    #pragma omp barrier
        cout<<NowHeight<<";"<<NowNumDeer<<";"<<NowTemp<<";"<<NowPrecip<<";"<<NowMonth<<";"<<NowYear<<";"<<my_aliens<<";"<<endl;
	    //cout<<"Grain Height: "<<NowHeight<<endl;
	    //cout<<"Deer Population: "<<NowNumDeer<<endl;
	    //cout<<"Temperature: "<<NowTemp<<endl;
	    //cout<<"Precipitation: "<<NowPrecip<<endl;
        //cout<<"MONTH: "<<NowMonth<<endl;
        //cout<<"Year: "<<NowYear<<endl;
        NowMonth++;
        if(NowMonth % 12 ==0){
            //NowMonth = 0;
            NowYear++;
        }
	    

   }
   exit(2);
}
void Grain(){
	NowMonth =0;
	NowYear = 2016;
	NowNumDeer = 1;
	NowHeight =  1.;
	while( NowYear <= 2021 )
	{
	   // compute a temporary next-value for this quantity based on the current state of the simulation:
	   
	
	   // DoneComputing barrier:
	   #pragma omp barrier
	   NowTemp = Get_Temp();
	   NowPrecip = Get_Precip();
	   
	   // DoneAssigning barrier:
	   #pragma omp barrier
	   Change_Heights();
	    

	   // DonePrinting barrier:
	   #pragma omp barrier
	
	}
}



void GrainDeer(){
	NowMonth =0;
	NowYear = 2016;
	NowNumDeer = 1;
	NowHeight =  1.;
		while( NowYear <= 2021 )
	{
	// compute a temporary next-value for this quantity based on the current state of the simulation:
	

	// DoneComputing barrier:
	#pragma omp barrier
		if(NowNumDeer<NowHeight){
			NowNumDeer++;
			
		}
		else{
			NowNumDeer--;
			
		}
        if(NowNumDeer <0){
            NowNumDeer = 0;
        }

	// DoneAssigning barrier:
	#pragma omp barrier
		

	// DonePrinting barrier:
	#pragma omp barrier
	
	}
}

	

int main(){
	omp_set_num_threads( 4 );
	
	
	#pragma omp parallel sections
	{
	#pragma omp section
	{
		GrainDeer( );
	}

	#pragma omp section
	{
		Grain( );
	}

	#pragma omp section
	{
		Watcher( );
	}

	#pragma omp section
	{
		MyAgent( );
	}
	
    }
  
  return 0;  // implied barrier -- all functions must return in order to allow any of them to get past here
}




