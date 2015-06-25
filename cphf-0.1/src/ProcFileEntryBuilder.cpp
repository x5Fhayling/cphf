///////////////////////////////////////////////////////////////////////////////
//    Crouching Process Hidden File (cphf)
//    Spilling the details on unlinked files and the processes accessing them
//    Copyright (C) 2015 Erin Adamek
//    Please report bugs to: cphfbugs@gmail.com
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

namespace init{
    unsigned long long init_TxtFD(){
        unsigned long long maxFDSize;
        struct rlimit rlim;
        if(getrlimit(RLIMIT_NOFILE, &rlim)!=CPHF_ERROR){
            maxFDSize =rlim.rlim_max +1;
        }else{
            maxFDSize = UINT_MAX+1;
        }
        return maxFDSize;
    }
}

//setting externed consts ULF_TXT_FD and attributeFlags
const unsigned long long ULF_TXT_FD = init::init_TxtFD();
std::unordered_map<int,std::string> CPHFFile::attributeFlags;


void ProcFileEntryBuilder::build(data_digger_list diggers){
    
    data_digger ddig;
    CPHFReader * cphfr=nullptr;
    

    std::string options;
    std::vector<cphf_attribute> procAndFileList;

    for(auto& dItr : diggers){
        ddig = dItr;
        options=ddig.second;
        switch(ddig.first){
            case CPHF_READER_LSOF:  cphfr = new LSOFReader;
                break;
            case CPHF_READER_PROC:  cphfr = new ProcReader;
                break;
            case CPHF_READER_FILESTAT:  cphfr = new FileStatReader;
                break;
            default:    cphfr=nullptr; //in theory this should not be possible...
                        printf("ERROR: READER WITH ID '%d' NOT RECOGNIZE\n", ddig.first);
        }
        
        if (cphfr!=nullptr) {
            cphfr->createAttribMap(CPHFFile::attributeFlags);
            cphfr->read(ddig.second,procMap_);
            delete cphfr;
        }
    }
}

std::vector<cphf_file_ptr> ProcFileEntryBuilder::flattenMap(bool procs, bool files){
    
    proc_file_ptr pf;
    std::vector<cphf_file_ptr> builtList;
    for( auto& itr : procMap_){
        pf = std::dynamic_pointer_cast<ProcFile>(itr.second);
        if(procs){builtList.push_back(pf);}
        if(files){pf->flatten(builtList);}
    }
    return builtList;
}

