#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<long> now = LinuxParser::CpuUtilization();
    //calc    
    if(prev.size() == 0) {
      prev.push_back(0);
      prev.push_back(0);
      prevUtilization = 0;
    }
    else {
        prevUtilization = 1-(now[1]-prev[1])/(float)(now[0]-prev[0]);
        prev[0] = now[0];
        prev[1] = now[1];
    }


    return prevUtilization;
 }