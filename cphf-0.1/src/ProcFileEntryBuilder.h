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



#ifndef PROCFILEENTRYBUILDER_H
#define PROCFILEENTRYBUILDER_H

#include "LSOFReader.h"
#include "CPHFFile.h"
#include "ProcReader.h"
#include "FileStatReader.h"

#include <sys/resource.h>
#include <limits.h>

using data_digger = std::pair<int, std::string>;
using data_digger_list = std::vector<data_digger>;

enum reader_ids{CPHF_READER_LSOF     = 0,
                CPHF_READER_PROC     = 1,
                CPHF_READER_FILESTAT = 2};

class ProcFileEntryBuilder{

    public:
        ProcFileEntryBuilder(void){};
        ~ProcFileEntryBuilder(void)= default;
    
        void build(data_digger_list diggers);
        std::vector<cphf_file_ptr> flattenMap(bool procs, bool files);
    
    private:    
        ProcFileEntryBuilder(ProcFileEntryBuilder const&)=default;
        ProcFileEntryBuilder& operator=(ProcFileEntryBuilder const&)=default;

        std::unordered_map<int,std::string> attributeFlags_;
        std::map<int, cphf_file_ptr> procMap_;
};


#endif



