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


#ifndef PROCFILE_H
#define PROCFILE_H

#include "UnlinkedFile.h"

class ProcFile : public CPHFFile{

    public:
        ProcFile()=default;
        virtual ~ProcFile(void)=default;
        void addFileAttributes(std::function<void(unsigned int fid,
                                                  unsigned int pid,
                                                  std::shared_ptr<UnlinkedFile>ulf)>func);
        std::shared_ptr<UnlinkedFile> addFile(unsigned long long fd,
                                              std::shared_ptr<UnlinkedFile> ulf);
        void flatten(std::vector<std::shared_ptr<CPHFFile>>& builtList);
    
    private:
        unsigned int pid=0;
        unsigned int ppid=0;
        std::vector<cphf_attribute> attributes;
        std::map<int, ProcFile*> childProcs;    //not yet in use
        ProcFile * parentProc;                  //not yet in use
        std::map<int, std::shared_ptr<UnlinkedFile>> ulFiles;
        void do_addAttribute(unsigned int fileid, std::string value);
        void do_printAttributes(std::function<void(std::string label,
                                                   std::string value)>prettyprinter);

};

using proc_file_ptr = std::shared_ptr<ProcFile>;

#endif


