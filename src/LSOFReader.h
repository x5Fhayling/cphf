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


#ifndef LSOFREADER_H
#define LSOFREADER_H

#include "CPHFReader.h"
#include "UnlinkedFile.h"
#include "ProcFile.h"


const static std::string LSOF_OPTS_DEFAULT = "pfg";

class LSOFReader : public CPHFReader{

    public:
        LSOFReader(void) = default;
        ~LSOFReader(void)= default;

    private:
        void do_read(std::string options, std::map<int, cphf_file_ptr>& procMap);
        void do_createAttribMap(std::unordered_map<int,std::string>& attribFlags);
        void getFilesInfo(char ** argv, std::map<int, cphf_file_ptr>& procMap);
        void bufferedRead(int& stdout_pipe, std::map<int, cphf_file_ptr>& procMap);
    
        enum lsof_flags{LSOF_ID_FD    = 'f',
                        LSOF_ID_PID   = 'p',
                        LSOF_ID_FLAGS = 'G'};
 };


#endif


