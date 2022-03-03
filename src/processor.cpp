#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<long> now = LinuxParser::CpuUtilization();
    //calc    
    if(prev.size() == 0) {
      // init
      prev.push_back(0);
      prev.push_back(0);
      prevUtilization = 0;
    }
    else {
      // now[1] and prev[1] are "idled"
      // now[0] and prev[0] are "totald"
      // using fomular: CPU_Percentage = 1 - idled/totald
        prevUtilization = 1-(now[1]-prev[1])/(float)(now[0]-prev[0]);
        prev[0] = now[0];
        prev[1] = now[1];
    }


    return prevUtilization;
 }