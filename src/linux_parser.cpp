#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string key, value;
  int MT, MF, shmem, SRcm;
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          MT = std::stoi(value);
        }
        if (key == "MemFree:") {
          MF = std::stoi(value);
        }  
        if (key == "SReclaimable:") {
          SRcm = std::stoi(value);
        }  
        if (key == "Shmem:") {
          shmem = std::stoi(value);
        }                                      
      }
    }

  }

  return (MT-MF+SRcm-shmem)/(float)MT; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string v1;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream>>v1;
  }
  return std::stol(v1);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return 0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line, key, value;
  long total=0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    std::stringstream cstream;
    cstream << filestream.rdbuf();
    string s = cstream.str();
    std::istringstream fstr(s);
    for(int i=0;i<=16;++i){ 
      fstr>>value; 
      if(i>=13 && i<=16) total += std::stol(value);
    } 
    
  }
  return total; 
}

long LinuxParser::StartTimeJiffies(int pid) { 
  string line, key, value;
  long startTime = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    std::stringstream cstream;
    cstream << filestream.rdbuf();
    string s = cstream.str();
    std::istringstream fstr(s);
    for(int i=0;i<=21;++i){ 
      fstr>>value; 
      if(i==21) startTime += std::stol(value);
    } 
    
  }
  return startTime; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> CC(10);
  string line, key;
  long aj = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> CC[0]>> CC[1]>> CC[2]>> CC[3]>> CC[4]>> CC[5]>> CC[6]>> CC[7]>> CC[8]>>CC[9]){
        if (key == "cpu") {
          for(int i=0;i<10;++i){
            aj += std::stol(CC[i]);
          }
        };
      }
    }
  }
  return aj; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long ij=0;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream>>key;
      if(key == "cpu"){
        for(int i =0;i<3;++i) {linestream>>value;}
        ij = std::stol(value);
        linestream>>value;
        ij += std::stol(value);
      }
    }
  }
  return ij; 
}

// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() { 
  // if only one CPU will be calculated, then the vector results has only one member
  // results[0] is "totald", results[1] is "idled"
  vector<long> results;
  results.push_back(LinuxParser::ActiveJiffies());
  results.push_back(LinuxParser::IdleJiffies());

  return results;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key, value, line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
       std::istringstream linestream(line);
       while (linestream >> key >> value) {
         if(key == "processes") return std::stoi(value);
       }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key, value, line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key == "procs_running") return std::stoi(value);
      }
    }
  }
  return 0;
}

string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    return line;
  }
  return string();
 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
  	while(std::getline(filestream, line)){
    	std::istringstream linestream(line);
      while(linestream >> key >> value){
     	if(key=="VmSize:") {
        int v = std::stoi(value)/1024;        
        return to_string(v);
        } 
      }
    }
  }
  return string(); 
}


// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
  	while(std::getline(filestream, line)){
    std::istringstream linestream(line);
    while(linestream >> key >> value){
    	if(key=="Uid:") return value;
     }    
    }
  }
  return string(); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string line, key, s1, value;
  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open()){
  	while(std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
    	std::istringstream linestream(line);      
      while(linestream >> key >> s1 >> value){
     	 if(value==uid) return key;
      }
    }
  }
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, value;
  std::ifstream filestream("/proc/" + to_string(pid) + "/stat");
  if(filestream.is_open()){
    std::stringstream cstream;
    cstream << filestream.rdbuf();
    string s = cstream.str();
    std::istringstream fstr(s);
    for(int i=0;i<=21;++i){
    	fstr>>value;
    } 
    return std::stol(value)/sysconf(_SC_CLK_TCK);
  }
  return 0; 
}
