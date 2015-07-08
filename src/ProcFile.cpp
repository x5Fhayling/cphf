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


#include "ProcFile.h"



void ProcFile::do_printAttributes(std::function<void(std::string label,
                                                  std::string value)> prettyprinter){
    std::string label;
    
    for(auto& it : attributes){
        label = std::string(CPHFFile::attributeFlags.at(it.first));
        prettyprinter(label, it.second);
    }
    
}


void ProcFile::do_addAttribute(unsigned int attrid, std::string value){
    
    switch(attrid){
        case CPHF_ID_PID:   this->pid = atoi(value.c_str());
                            attributes.push_back(cphf_attribute(attrid,value));
            break;
        case CPHF_ID_CMD:   attributes.insert(attributes.begin(), cphf_attribute(attrid, value));
            break;
        default:            attributes.push_back(cphf_attribute(attrid,value));
            
    }
    
}





std::shared_ptr<UnlinkedFile> ProcFile::addFile(unsigned long long fd,
                                                std::shared_ptr<UnlinkedFile> ulf){
    
    return ulFiles.insert(std::pair<unsigned int,
                          std::shared_ptr<UnlinkedFile>>(fd, ulf)).first->second;
}

void ProcFile::addFileAttributes(std::function<void(unsigned int fid, unsigned int pid,
                                                    std::shared_ptr<UnlinkedFile> ulf)> func){

    for(auto& ulf : ulFiles){
        func( ulf.first, this->pid, ulf.second);
    }
}

void ProcFile::flatten(std::vector<std::shared_ptr<CPHFFile>>& builtList){
    
    for(auto& itr : ulFiles){
        std::shared_ptr<UnlinkedFile> ulf (itr.second);
        
        builtList.push_back(ulf);
    }
}

