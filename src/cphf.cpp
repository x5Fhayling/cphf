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


#include "ProcFileEntryBuilder.h"
#include "PrettyPrinter.h"
#include "getopt.h"



bool checkOptions(const std::string defaults, std::string& options, std::string cmd){
    bool flagsOkay=true;
    if(options!="no"){
        for(size_t i=0; i<options.length(); i++){
            
            if(defaults.find(options[i], 0)== std::string::npos){
                printf("ERROR: option %c is not a recognized %s flag\n", options[i], cmd.c_str());
                flagsOkay=false;
            }
        }
    }
    return flagsOkay;
    //Note: Even though cphf will abort if just one flag is bad, all flags for
    //all options will be checked sot aht if there are n errors cphf will only
    //have to run once to allert for all n errors
}

int main(int argc, char **argv){
    

    bool flagsOkay=true;
    data_digger_list funcvector;
    ProcFileEntryBuilder * lb = new ProcFileEntryBuilder;
    bool procs=true;
    bool files=true;
 
    static struct option long_options[] = {
        {"proc",    required_argument, nullptr,  'p' },
        {"file",    required_argument, nullptr,  'f' },
        {0,         0,                 0,  0 }
    };

    int opts_index;
    char opt;
    
    std::string proc_ops = PROC_OPTS_DEFAULT;
    std::string file_ops = FILESTAT_OPTS_DEFAULT;

    while ((opt = getopt_long(argc, argv,"p:f:", long_options, &opts_index )) != -1){
    
        switch(opt){
                case 'p':   proc_ops = std::string(optarg);
                            flagsOkay =checkOptions(PROC_OPTS_DEFAULT, proc_ops, "proc");
                    break;
                case 'f':   file_ops = std::string(optarg);
                            flagsOkay= checkOptions(FILESTAT_OPTS_DEFAULT, file_ops, "file");
                    break;
        }
        
    }
    
    if(flagsOkay){
        funcvector.push_back(data_digger(CPHF_READER_LSOF,LSOF_OPTS_DEFAULT));

        if(proc_ops!="no"){
            funcvector.push_back(data_digger(CPHF_READER_PROC, proc_ops));
        }else{
            procs = false;
        }

        if(file_ops!="no"){
            funcvector.push_back(data_digger(CPHF_READER_FILESTAT, file_ops));
        }else{
            files = false;
        }
    }
    
    if(funcvector.size()!=1){lb->build(funcvector);}
    auto printQueue = lb->flattenMap(procs, files);
    PrettyPrinter::printFiles(printQueue);
    
    delete lb;
}














