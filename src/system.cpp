#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pPids = LinuxParser::Pids();
    //vector<Process> pPids = LinuxParser::Pids();
    for(unsigned int i=0;i<pPids.size();++i){
        Process p1(pPids[i]);
        /*p1.setPID(pPids[i]);
        p1.setCommand(LinuxParser::Command(pPids[i]));
        p1.setUser(LinuxParser::User(pPids[i]));
        p1.setRam(LinuxParser::Ram(pPids[i]));
        p1.setUpTime(LinuxParser::UpTime(pPids[i]));
        p1.setJiffies(LinuxParser::ActiveJiffies(pPids[i]), LinuxParser::StartTimeJiffies(pPids[i]), uptime_);*/
        processes_.push_back(p1);
    }
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
    uptime_ = LinuxParser::UpTime();
    return  uptime_;
}
