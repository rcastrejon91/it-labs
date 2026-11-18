#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
extern "C" {void av_reset();double av_accel(double,double,double,double,int);}
struct Scenario {const char* name;double ego;double lead;double gap;int valid;};
int main(int argc,char**argv){
 std::string path=argc>1?argv[1]:"metrics.csv";
 std::ofstream out(path);if(!out){std::cerr<<"Cannot write metrics\n";return 1;}
 out<<"scenario,time_s,ego_speed_mps,lead_speed_mps,gap_m,accel_mps2,jerk_mps3,ttc_s,emergency,collision\n";
 Scenario cases[]={{"free_flow",15,15,90,1},{"slow_lead",22,13,60,1},{"cut_in",22,12,19,1},{"stopped_obstacle",25,0,14,1},{"sensor_failure",20,12,50,0}};
 for(auto s:cases){
  av_reset();double v=s.ego,g=s.gap,previous=0;int interventions=0,collisions=0;double peakJerk=0,minGap=g;
  for(int step=0;step<200;step++){
   constexpr double dt=.1;
   double a=av_accel(v,s.lead,g,dt,s.valid);
   double jerk=(a-previous)/dt;previous=a;peakJerk=std::max(peakJerk,std::abs(jerk));
   if(a<=-5.99)interventions++;
   const double closing=std::max(0.0,v-s.lead);
   double ttc=closing>0.01?g/closing:1e9;
   v=std::max(0.0,v+a*dt);g+=(s.lead-v)*dt;
   bool collision=g<=0;if(collision){collisions++;g=0;}
   minGap=std::min(minGap,g);
   out<<s.name<<','<<std::fixed<<std::setprecision(3)<<step*dt<<','<<v<<','<<s.lead<<','<<g<<','<<a<<','<<jerk<<','<<ttc<<','<<(a<=-5.99)<<','<<collision<<"\n";
   if(collision)break;
  }
  std::cout<<s.name<<": minimum gap="<<minGap<<" m, emergency steps="<<interventions<<", peak jerk="<<peakJerk<<" m/s^3, collisions="<<collisions<<"\n";
 }
 std::cout<<"Telemetry written to "<<path<<"\n";
}
