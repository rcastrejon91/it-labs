#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
extern "C" { void av_reset(); double av_accel(double,double,double,double,int); }
void check(bool ok,const char* name){std::cout<<(ok?"PASS ":"FAIL ")<<name<<"\n";if(!ok)std::exit(1);}
int main(){
 av_reset();double a=av_accel(15,15,90,.1,1);check(a>=0&&a<=.300001,"jerk-limited acceleration");
 av_reset();a=av_accel(20,15,25,.1,1);check(a<0&&a>=-.300001,"short-gap deceleration");
 av_reset();a=av_accel(25,0,10,.1,1);check(a==-6,"emergency TTC brake");
 av_reset();a=av_accel(15,10,NAN,.1,1);check(a==-6,"invalid range fails safe");
 av_reset();a=av_accel(15,10,30,.1,0);check(a==-6,"sensor failure fails safe");
 av_reset();a=av_accel(15,10,30,0,1);check(a==-6,"invalid timestep fails safe");
 std::cout<<"6/6 control checks passed\n";
}
