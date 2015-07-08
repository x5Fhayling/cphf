///////////////////////////////////////////////////////////////////////////////
//    Crouching Process Hidden File (cphf)
//    Spilling the details on unlinked files and the processes accessing them
//    Copyright (C) 2015 Erin Adamek
//    Please report bugs to: https://github.com/x5Fhayling/cphf/issues
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////


#include "CPHFReader.h"

std::string CPHFReader::readTime(time_t t, std::string format){
    std::stringstream strbuff;
    struct tm  *timeGetter;
    
    //because the format for the date is set don't need to worry
    //about overflow
    char modDate[30];
  
  
    timeGetter= localtime(&t);
    if(strftime(modDate,30,format.c_str(),timeGetter)==0){
        strbuff << "ERROR READING TIME";
        
    }else{
        strbuff << modDate;
    }
    
    return strbuff.str();
}

std::string CPHFReader::readETime(time_t t, std::string format){
    std::string elapsedTime;
    char charBuff[14];
    time_t hms[3];
    
    const int SECONDS_IN_HR = 3600;
    const int MINS_IN_HR = 60;
    
    
    int sec = t/sysconf(_SC_CLK_TCK);
    hms[0] = sec/SECONDS_IN_HR;
    hms[1] = (sec % SECONDS_IN_HR)/MINS_IN_HR;
    hms[2] = (sec % SECONDS_IN_HR) % MINS_IN_HR;

    
    if(snprintf(charBuff, 13, format.c_str(), hms[0], hms[1], hms[2])<-1){
        elapsedTime="ERROR READING TIME";
    }else{
        charBuff[13]=0;
        elapsedTime = std::string(charBuff);
    }
    
    return elapsedTime;
}

void CPHFReader::buildDeviceMap(std::map<dev_t, std::map<struct minor_dev_range, std::string>>& deviceMap){
    char rangeCheck;
    std::ifstream fsin;
    fsin.exceptions ( std::fstream::failbit);
    try {
        fsin.open("/proc/tty/drivers", std::ifstream::in);
        
        std::string serialDevice, tmp;
        dev_t  major;
        
        minor_dev_range minor;
        
        while(fsin>>tmp){
            
            fsin >> serialDevice;
            serialDevice = serialDevice.substr(5);
            
            fsin >> major;
            fsin >> minor.min;
            
            fsin >> rangeCheck;
            
            if(rangeCheck=='-'){
                fsin >> minor.max;
            }else{
                minor.max = minor.min;
            }
            
            fsin >> tmp;
            
            try {
                auto minMap = deviceMap.at(major);
                std::pair<struct minor_dev_range, std::string> minEntry = make_pair(minor,serialDevice);
                minMap.insert(minEntry);
                
            }catch(const std::out_of_range& oor){
                std::map<struct minor_dev_range, std::string> minMap;
                std::pair<struct minor_dev_range, std::string> minEntry = make_pair(minor,serialDevice);
                minMap.insert(minEntry);
                std::pair<dev_t, std::map<struct minor_dev_range, std::string>>devEntry = make_pair(major, minMap);
                deviceMap.insert(devEntry);
            }
        }
        fsin.clear();
        fsin.close();
        
    }catch(std::fstream::failure e) {
        //quietly move on if something goes wrong
    }
}




// formatting ints to strings
std::string CPHFReader::format(unsigned int value){
    std::stringstream strbuff;
    strbuff << value;
    return strbuff.str();
}


std::string CPHFReader::readPerms(mode_t mode){
   // char perms[9];
    std::string perms;

    const char NOPE = '-';
    const char READ = 'r';
    const char WRITE = 'w';
    const char EXECUTE = 'x';
    const char DIRECTORY = 'd';
    const char STICKYBIT = 't';
    const char SETGUID = 's';
   
    //directory or file?
    ((mode & S_IFMT)==S_IFDIR)?(perms += DIRECTORY):(perms += NOPE);
    
    //user perms
    ((mode & S_IRUSR)==S_IRUSR)?(perms += READ):(perms += NOPE);
    ((mode & S_IWUSR)==S_IWUSR)?(perms += WRITE):(perms += NOPE);
    //s instead of x in the owner permissions setuid
    if((mode & S_ISGID)==S_ISGID){
        perms += SETGUID;
    }else if((mode & S_IXUSR)==S_IXUSR){
        perms += EXECUTE;
    }else{
        perms += NOPE;
    }
    
    
    //group perms
    ((mode & S_IRGRP)==S_IRGRP)?(perms += READ):(perms += NOPE);
    ((mode & S_IWGRP)==S_IWGRP)?(perms += WRITE):(perms += NOPE);
    ((mode & S_IXGRP)==S_IXGRP)?(perms += EXECUTE):(perms += NOPE);

    
    //other perms
    ((mode & S_IROTH)==S_IROTH)?(perms += READ):(perms += NOPE);
    ((mode & S_IWOTH)==S_IWOTH)?(perms += WRITE):(perms += NOPE);
    //sticky bit is a t instead of an x in the other permissions
    if((mode & S_ISVTX)==S_ISVTX){
        perms += STICKYBIT;
    }else if((mode & S_IXOTH)==S_IXOTH){
        perms += EXECUTE;
    }else{
        perms += NOPE;
    }

    return perms;
    
}

std::string CPHFReader::readDevice(struct stat * info){

    dev_t devID;
    std::stringstream strbuff;
    std::string devlabel;
    
    
    if(info->st_dev==FS_CHARCTER_DEVICE){
        devID =info->st_rdev;
    }else{
        devID =info->st_dev;

    }
    
    std::map<dev_t, std::map<struct minor_dev_range, std::string>>deviceMap;
    
    buildDeviceMap(deviceMap);
    
    dev_t major;
    minor_dev_range minor;
    major = major(devID);
    minor.min = minor(devID);
    minor.max = minor.min;
        
    strbuff << major << "," << minor.min << "\t";
    
    try {
        auto minorMap = deviceMap.at(major);
        devlabel = minorMap.at(minor);
        if(devlabel.find("tty", 0)!=std::string::npos){
            strbuff << devlabel <<minor.min;
        }else if(devlabel.find("pts", 0)!=std::string::npos){
            strbuff<< devlabel << "/" <<minor.min;
        }
        
    }catch (const std::out_of_range& oor) {
        strbuff << '?';
    }
    
    
    return strbuff.str();
}
