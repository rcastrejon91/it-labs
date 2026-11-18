#include <algorithm>
#include <cmath>
extern "C" {
struct Output { double accel; double ttc; int emergency; };
static double previous_accel=0;
void av_reset(){previous_accel=0;}
Output av_step(double speed,double lead_speed,double gap,double dt,int valid){
 if(!valid||!std::isfinite(speed)||!std::isfinite(lead_speed)||!std::isfinite(gap)||!(dt>0&&dt<=0.2)) {previous_accel=-6;return {-6,0,1};}
 const double closing=std::max(0.0,speed-lead_speed);
 const double ttc=closing>0.01?gap/closing:1e9;
 if(gap<=0||ttc<1.5){previous_accel=-6;return {-6,ttc,1};}
 const double desired=5+2*std::max(0.0,speed);
 const double target=std::clamp(0.35*(gap-desired)+0.7*(lead_speed-speed),-6.0,2.0);
 const double a=std::clamp(target,previous_accel-3*dt,previous_accel+3*dt);
 previous_accel=a;return {a,ttc,0};
}
double av_accel(double speed,double lead_speed,double gap,double dt,int valid){return av_step(speed,lead_speed,gap,dt,valid).accel;}
}
