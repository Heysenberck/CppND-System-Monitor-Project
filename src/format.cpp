#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    long hhours   = seconds/3600;
    long mminutes = (seconds%3600)/60;
    long sseconds = seconds - mminutes*60 - hhours*3600;
    std::ostringstream sstream;
    sstream << std::setw(2) << std::setfill('0') << hhours << ":" << std::setw(2) << std::setfill('0') << mminutes << ":" << std::setw(2) << std::setfill('0') << sseconds; 
    return sstream.str();
}
