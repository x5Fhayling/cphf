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


#include "UnlinkedFile.h"


void UnlinkedFile::do_printAttributes(std::function<void(std::string, std::string)> prettyprinter){
    std::string label;
    
    for(auto& it : attributes){
        label = std::string(CPHFFile::attributeFlags.at(it.first));
        prettyprinter(label, it.second);
        
    }
    
}

UnlinkedFile::UnlinkedFile(unsigned long long pid, unsigned long long fid){
    std::string fdresult;
    this->pid = pid;
    this->fid = fid;
    std::stringstream strbuff;
    strbuff << "/proc/" << pid;

    
    if (fid > ULF_TXT_FD) {
        strbuff << "/exe";
    }else{
        strbuff << "/fd/" << fid;
    }

    this->path = strbuff.str();
    
}


void UnlinkedFile::do_addAttribute(unsigned int attrid, std::string value){
    std::stringstream strbuff;
    
    switch(attrid){
            
        case CPHF_ID_FILENAME:   attributes.insert(attributes.begin(),
                                                   cphf_attribute(attrid, value));
            break;
        default:    attributes.push_back(cphf_attribute(attrid,value));
            
    }

}


