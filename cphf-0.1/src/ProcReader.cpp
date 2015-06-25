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



#include "ProcReader.h"


std::map<char, std::string> ProcReader::procstate = {
    {PR_ID_STATE_RUNNING, PR_LABEL_STATE_RUNNING},
    {PR_ID_STATE_ISLEEPING, PR_LABEL_STATE_ISLEEPING},
    {PR_ID_STATE_UISLEEPING,PR_LABEL_STATE_UISLEEPING},
    {PR_ID_STATE_ZOMBIE, PR_LABEL_STATE_ZOMBIE},
    {PR_ID_STATE_PAGING,PR_LABEL_STATE_PAGING},
    {PR_ID_STATE_STOPPED, PR_LABEL_STATE_STOPPED}
};

void ProcReader::do_createAttribMap(std::unordered_map<int,std::string>& attribFlags){
    attribFlags.insert(cphf_attribute(CPHF_ID_CMD,           CPHF_LABEL_CMD));
    attribFlags.insert(cphf_attribute(CPHF_ID_PPID,          CPHF_LABEL_PPID));
    attribFlags.insert(cphf_attribute(CPHF_ID_PROCGID,       CPHF_LABEL_PROCGID));
    attribFlags.insert(cphf_attribute(CPHF_ID_LOGINNAME,     CPHF_LABEL_LOGINNAME));
    attribFlags.insert(cphf_attribute(CPHF_ID_UID,           CPHF_LABEL_UID));
    attribFlags.insert(cphf_attribute(CPHF_ID_UGID_NAME,     CPHF_LABEL_UGID_NAME));
    attribFlags.insert(cphf_attribute(CPHF_ID_UGID,          CPHF_LABEL_UGID));
    attribFlags.insert(cphf_attribute(CPHF_ID_PSTART_TIME,   CPHF_LABEL_PSTART_TIME));
    attribFlags.insert(cphf_attribute(CPHF_ID_PCPU_TIME,     CPHF_LABEL_PCPU_TIME));
    attribFlags.insert(cphf_attribute(CPHF_ID_DEVICE,        CPHF_LABEL_DEVICE));
    attribFlags.insert(cphf_attribute(CPHF_ID_PVMS,          CPHF_LABEL_PVMS));
    attribFlags.insert(cphf_attribute(CPHF_ID_PRSS,          CPHF_LABEL_PRSS));
    attribFlags.insert(cphf_attribute(CPHF_ID_TEXT_REGION,   CPHF_LABEL_TEXT_REGION));
    attribFlags.insert(cphf_attribute(CPHF_ID_DATA_REGION,   CPHF_LABEL_DATA_REGION));
    attribFlags.insert(cphf_attribute(CPHF_ID_STACK_REGION,  CPHF_LABEL_STACK_REGION));
    attribFlags.insert(cphf_attribute(CPHF_ID_EPI,           CPHF_LABEL_EPI));
    attribFlags.insert(cphf_attribute(CPHF_ID_PERMS,         CPHF_LABEL_PERMS));
    attribFlags.insert(cphf_attribute(CPHF_ID_PSTATE,        CPHF_LABEL_PSTATE));
}


cphf_attribute ProcReader::processOpiton(char option, struct proc_info * info){
    std::string attrib;
    int attribID;
    switch(option){
            
        case PR_OPT_COMMAND:    attrib   = readCMD(info->pid);
                                attribID = CPHF_ID_CMD;
            break;
        case PR_OPT_PPID:       attrib   = format(info->ppid);
                                attribID = CPHF_ID_PPID;
            break;
        case PR_OPT_PROCGID:    attrib   = format(info->procgid);
                                attribID = CPHF_ID_PROCGID;
            break;
        case PR_OPT_USERNAME:   attrib   = readIDs(option, info);
                                attribID = CPHF_ID_LOGINNAME;
            break;
        case PR_OPT_UID:        attrib   = readIDs(option, info);
                                attribID = CPHF_ID_UID;
            break;
        case PR_OPT_UGID_NAME:  attrib   = readIDs(option, info);
                                attribID = CPHF_ID_UGID_NAME;
            break;
        case PR_OPT_UGID:       attrib   = readIDs(option, info);
                                attribID = CPHF_ID_UGID;
            break;
        case PR_OPT_START_TIME: attrib   = readTime(info->starttime,
                                                    CPHF_FORMAT_DATE_TIME);
                                attribID = CPHF_ID_PSTART_TIME;
            break;
        case PR_OPT_CPU_TIME:   attrib   = readETime(info->pstime+info->putime,
                                                     CPHF_FORMAT_TIME);
                                attribID = CPHF_ID_PCPU_TIME;
            break;
        case PR_OPT_STATE:      attrib   = procstate[info->state];
                                attribID = CPHF_ID_PSTATE;
            break;
        case PR_OPT_DEVICE:     attrib   = readDevice(info->info);
                                attribID = CPHF_ID_DEVICE;
            break;
        case PR_OPT_VMS:    attrib   = format(info->vms);
                            attribID = CPHF_ID_PVMS;
            break;
        case PR_OPT_RMS:    attrib   = format(info->rss);
                            attribID = CPHF_ID_PRSS;
            break;
        case PR_OPT_MEM_TEXT:   attrib   = format(info->textbegin, info->textend);
                                attribID = CPHF_ID_TEXT_REGION;
            break;
        case PR_OPT_MEM_DATA:   attrib   = format(info->databegin, info->dataend);
                                attribID = CPHF_ID_DATA_REGION;
            break;
       case PR_OPT_MEM_STACK:    attrib   = format(info->stackbegin, info->esp);
                                attribID = CPHF_ID_STACK_REGION;
            break;
        case PR_OPT_INST_PTR:   attrib   = format(info->epi, INST_POINTER);
                                attribID = CPHF_ID_EPI;
            break;
        case PR_OPT_PERMS:  attrib   = readPerms(info->info->st_mode);
                            attribID = CPHF_ID_PERMS;
            break;
        default:    printf("ERROR: proc option %c not recognized\n",option);
            
    }

    return cphf_attribute(attribID, attrib);
}

void ProcReader::do_read(std::string options, std::map<int, cphf_file_ptr>& procMap){
    struct proc_info * info = new struct proc_info;
    info->info = new struct stat;

    for(auto& procMap_itr : procMap){
        
        std::map<int, std::function<void(void)>> procReaderFuncs;
        std::string attrib;
        
        unsigned int pid = procMap_itr.first;
        proc_file_ptr proc = std::dynamic_pointer_cast<ProcFile>(procMap_itr.second);
        
        
        cphf_attribute attributePair;
        
        if(readProcStat(pid, info) != CPHF_ERROR){
            info->pid = pid;
            
            for(size_t i=0; i< options.length(); i++){
                
                attributePair=processOpiton(options[i], info);
                addReader(attributePair.first, attributePair.second, proc, procReaderFuncs);
            }
            
            for(auto & procReaderFuncs_itr : procReaderFuncs){
                procReaderFuncs_itr.second();
            }
            

        }
        
    }
    delete info->info;
    delete info;
}


//
//  Workerhorse fucntions.  Most attributes are gleaned from readProcStat
//
int ProcReader::readProcStat(unsigned int pid, struct proc_info* info){
    std::stringstream strbuff;
    strbuff << "/proc/" << pid << "/stat";
    int status = CPHF_SUCCESS;
    
    FILE * fin;
    fin = fopen(strbuff.str().c_str(), "r");
    
    strbuff.clear();
    strbuff.str("");
    int fscanResult=0;
    if(fin!= nullptr){
        //using scanf because /proc/stat is specifically designed to use scanf format strings
       	fscanResult  = fscanf(fin,"%*d %*s %c %d %d %*d %d %*u %*u %*u", &info->state, &info->ppid, &info->procgid, &info->device);
	fscanResult += fscanf(fin,"%*u %*u %*u %lu %lu %lu %lu %*u %*u %*u", &info->putime, &info->pstime, &info->pcutime, &info->pcstime);
        fscanResult += fscanf(fin,"%*u %llu %lu %ld %*u %lu", &info->starttime, &info->vms, &info->rss, &info->textbegin);
        fscanResult += fscanf(fin,"%lu %lu %lu %lu", &info->textend, &info->stackbegin, &info->esp, &info->epi);
        fscanResult += fscanf(fin,"%*u %*u %*u %*u %*u %*u %*u %*u %*d %*d %*u %*u %*u %*u %*d %lu %lu", &info->databegin, &info->dataend);
        if(fscanResult!=18){printf("ERROR: problem reading /proc/%d/stat\n", pid);} 
	
        info->pw = nullptr;
        info->gr = nullptr;
        fclose(fin);
    }else{
        strbuff << "ERROR looking up PID " << pid;
        perror (strbuff.str().c_str());
        status =CPHF_ERROR;
    }
    
     strbuff << "/proc/" << pid << "/exe";
     status = stat(strbuff.str().c_str(), info->info);
     
     strbuff.clear();
     strbuff.str("");
    
    return status;
}

// adds readers to the reader map.  Because the order of the outputted fields is dependent
// upon when they are added as an attribute all attributes are added to a map and that map is
// iterated through once all attributes for a proc have been collected
void ProcReader::addReader(unsigned int id, std::string attrib, proc_file_ptr proc,
                           std::map<int, std::function<void(void)>>& procReaderFuncs){
    
    attribute_reader ar;
    ar = std::make_pair(id, std::bind(&ProcFile::addAttribute, proc, id, attrib));
    procReaderFuncs.insert(ar);
    
}


// formatting memory addresses
std::string ProcReader::format(unsigned long int begin, unsigned long int end){
    std::string value;
    std::stringstream strbuff;
    strbuff.setf(std::ios_base::hex,std::ios_base::basefield);
    strbuff.setf(std::ios_base::showbase);
    strbuff <<begin;
    
    if(end!=INST_POINTER){
        strbuff << "  -  ";
        strbuff << end;
    }
    value = strbuff.str();
    
    strbuff.unsetf(std::ios_base::hex);
    strbuff.unsetf(std::ios_base::basefield);
    strbuff.unsetf(std::ios_base::showbase);
    
    return value;
}



//
//  Attribute getters/processors that need more work than just formatting
//

std::string ProcReader::readCMD(unsigned int pid){ //also add passingthe ref to proc obj
    std::stringstream strbuff;
    std::string tmp;
    
    strbuff << "/proc/" << pid << "/cmdline";
    
    std::fstream fsin;
    fsin.exceptions ( std::fstream::failbit | std::fstream::badbit);
   
    try {
        fsin.open(strbuff.str(), std::fstream::in);
    }catch(std::fstream::failure e) {
        perror ("ERROR retrieving command and arguments.");
    }
    fsin.exceptions(std::fstream::goodbit);
        strbuff.str("");
        strbuff.clear();
        
    
    while(std::getline(fsin, tmp,'\0')){
        
        strbuff << tmp << " ";

    }
    if(fsin.is_open()){
        fsin.clear();
        fsin.close();
    }

    return strbuff.str();
    
}

std::string ProcReader::readIDs(char option, struct proc_info* info){
    std::stringstream strbuff;

    if(option == PR_OPT_USERNAME || option == PR_OPT_UID ){
        
        if(info->pw==nullptr){
            info->pw=getpwuid(info->info->st_uid);
        }
        
        if(option == PR_OPT_USERNAME){
            strbuff << info->pw->pw_name;
        }else{
            strbuff << info->pw->pw_uid;
        }
    }else{
        
        if(info->gr==nullptr){
            info->gr=getgrgid(info->info->st_gid);
        }
        
        if(option == PR_OPT_UGID_NAME){
            strbuff << info->gr->gr_name;
        }else{
            strbuff << info->gr->gr_gid;
        }
    }
    
    
    return strbuff.str();
}

std::string ProcReader::readTime(time_t t, std::string format){
    //because start time is the time the proc started since boot need to find out
    //what time the system booted and then add the proc's start time.

    std::string output;
     
    //read uptime so that we can compute things like when the proc started and its cpu time
    time_t timeSinceBoot;
    std::fstream fsin;
    fsin.exceptions(std::fstream::failbit | std::fstream::badbit);

    try{
        fsin.open("/proc/uptime", std::ifstream::in);
        fsin >> timeSinceBoot;
        fsin.clear();
        fsin.close();

        //currentTime
        time_t currenttime = time (NULL);

        //convert what was in /proc/PID/stat from clock ticks to something usable
        time_t procStartTime = currenttime - timeSinceBoot  + (t/sysconf(_SC_CLK_TCK));
        output = CPHFReader::readTime(procStartTime, format);

    }catch(std::fstream::failure e){
         output = "ERROR READING TIME";
    }
    return output;
 }
