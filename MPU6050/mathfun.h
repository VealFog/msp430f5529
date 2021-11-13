#ifndef _MATHFUN_H_
#define _MATHFUN_H_

/*
 int abs(int val)
 { 		   
 int temp;
 if(val<0) 
 temp=-val;  
 else 
 temp = val;
 return temp;
 }
 */

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):(-x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
/*
 long mymap(long x,long in_min,long in_max,long out_min,long out_max)
 {
 return (x-in_min)*(out_max - out_min)/(in_max - in_min)+out_min;
 }
 */

#endif
