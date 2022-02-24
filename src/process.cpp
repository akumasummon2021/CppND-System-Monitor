#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

void Process::setPID(int pid){
	pid_ = pid;
}

void Process::setCommand(string c){
   command_ = c;
}

void Process::setUser(string u){
   user_ = u;
}

void Process::setRam(string r){
	ram_ = r;
}

void Process::setUpTime(long ut){
	uptime_ = ut;
}

Process(int pid){
   pid_ = pid;
	command_ = LinuxParser::Command(pid_);
	user_ = LinuxParser::User(pid_);
}

void Process::setJiffies(long aj, long st, long ut){
	activejiffies_ = aj;
	starttime_ = st;
	utilization_ = (aj/(float)sysconf(_SC_CLK_TCK))/(ut-st/(float)sysconf(_SC_CLK_TCK))*100;
}


// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
	activejiffies_ = LinuxParser::ActiveJiffies(pid_);
	starttime_ = LinuxParser::StartTimeJiffies(pid_);
	long upTimeSystem = LinuxParser::UpTime();
	utilization_ = (activejiffies_/(float)sysconf(_SC_CLK_TCK))/(upTimeSystem-starttime_/(float)sysconf(_SC_CLK_TCK))*100;
	return utilization_; 
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return ram_<a.ram_; }