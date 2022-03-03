#include <string>

#include "format.h"

using namespace std;
using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int h, m, s;
    string s_output;
    h = seconds/3600;
    m = (seconds - 3600*h)/60;
    s = (seconds - 3600*h)%60;
    if (h<10) s_output = "0" + to_string(h);
    else s_output = to_string(h);

    if (m<10) s_output += ":0" + to_string(m);
    else s_output += ":" + to_string(m);

    if (s<10) s_output += ":0" + to_string(s);
    else s_output += ":" + to_string(s);

    return s_output; 
}