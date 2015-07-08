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


#ifndef PROCREADER_H
#define PROCREADER_H


#include "ProcFile.h"
#include "CPHFReader.h"
#include <pwd.h>
#include <grp.h>


const std::string PROC_OPTS_DEFAULT = "PnUqMduGgesvrTDSic";



const std::string	PR_LABEL_STATE_RUNNING  = "R (Running)";
const std::string	PR_LABEL_STATE_ISLEEPING = "S (Sleeping in an interruptible wait)";
const std::string	PR_LABEL_STATE_UISLEEPING = "D (Waiting in uninterruptible disk sleep)";
const std::string	PR_LABEL_STATE_ZOMBIE = "Z (Zombie)";
const std::string   PR_LABEL_STATE_PAGING = "W (Paging)";
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,33)
const std::string PR_LABEL_STATE_STOPPED  = "T (Stopped on a signal)";
#else
const std::string	PR_LABEL_STATE_STOPPED = "T (Trace stopped)"
#endif


class ProcReader : public CPHFReader{

        public:
            ProcReader()=default;
            ~ProcReader()=default;
    
        private:
            struct proc_info{
                uint32_t device;
                char state;
                unsigned int pid;
                unsigned int ppid;
                unsigned int procgid;
                unsigned int session;
                unsigned long int flags;
                
                long unsigned int putime;	//clockticks this proc has been scheduled in user mode
                long unsigned int pstime;	//clockticks this proc has been scheduled in kernel mode
                unsigned long int pcutime;	//clockticks this proc's waited-for children have been scheduled in user mode
                unsigned long int pcstime;	//clockticks this proc's waited-for children have been scheduled in kernel mode
                
                unsigned long long int starttime; //time in clockticks the process started after system boot.
                unsigned long int textbegin;
                unsigned long int textend;
                unsigned long int databegin;
                unsigned long int dataend;
                unsigned long int stackbegin;
                unsigned long int esp;
                unsigned long int epi;
                
                unsigned long int vms;
                unsigned long int rss;
                
                struct stat *info;
                struct passwd *pw;
                struct group  *gr;
                
                
            };
            enum mem_ranges{    TEXT,
                                DATA,
                                STAT};
    
            enum procreader_opts{   PR_OPT_COMMAND  =    'c',
                                    PR_OPT_PID      =    'p', //default
                                    PR_OPT_PPID     =    'P',
                                    PR_OPT_PROCGID  =    'n',
                                    PR_OPT_USERNAME =    'U',
                                    PR_OPT_UID      =    'u',
                                    PR_OPT_UGID_NAME =   'G',
                                    PR_OPT_UGID      =   'g',
                                    PR_OPT_START_TIME =  'e',
                                    PR_OPT_CPU_TIME   =  'q',
                                    PR_OPT_STATE      =  's',
                                    PR_OPT_DEVICE     =  'd',
                                    PR_OPT_VMS        =  'v',
                                    PR_OPT_RMS        =  'r',
                                    PR_OPT_MEM_TEXT   =  'T',
                                    PR_OPT_MEM_DATA   =  'D',
                                    PR_OPT_MEM_STACK  =  'S',
                                    PR_OPT_INST_PTR   =  'i',
                                    PR_OPT_PERMS      =  'M',
                                    INST_POINTER      =  '!'};


            enum file_stat_ids{ PR_ID_STATE_RUNNING    = 'R',
                                PR_ID_STATE_ISLEEPING  = 'S',
                                PR_ID_STATE_UISLEEPING = 'D',
                                PR_ID_STATE_ZOMBIE     = 'Z',
                                PR_ID_STATE_PAGING     = 'W',
                                PR_ID_STATE_STOPPED    = 'T'};
    
    

            static std::map<char, std::string> procstate;
        
            std::string readTime(time_t t);
            std::string readTime(time_t t, std::string format);
            int readProcStat(unsigned int pid, struct proc_info* pinfo);
            std::string readIDs(char option, struct proc_info* pinfo);
            std::string readCMD(unsigned int pid);
        
            std::string format(unsigned long int begin, unsigned long int end);

            using CPHFReader::format;
            using CPHFReader::readTime;

    cphf_attribute processOpiton(char option, struct proc_info * info);
            void addReader(unsigned int id, std::string attrib, proc_file_ptr proc, std::map<int,
                       std::function<void(void)>>& procReaderFuncs);
    
            void do_read(std::string options, std::map<int, cphf_file_ptr>& procMap);
            void do_createAttribMap(std::unordered_map<int,std::string>& attribFlags);
    
    
};

#endif
