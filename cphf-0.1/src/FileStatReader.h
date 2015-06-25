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


#ifndef FILESTATREADER_H
#define FILESTATREADER_H

#include "CPHFReader.h"
#include "ProcFile.h"
#include "UnlinkedFile.h"

#include <pwd.h>
#include <grp.h>

#include <sys/types.h>
#include <functional>   // std::bind

#include <fcntl.h>
#include <time.h>

static const std::string FILESTAT_OPTS_DEFAULT = "niugMamcdsp";


//static const std::string FILESTAT_OPTS_DEFAULT = "niUuGgMamcdsp";

class FileStatReader : public CPHFReader{
    
    public:
        FileStatReader(){};
        ~FileStatReader()=default;
    
    private:
        enum filestat_ops{  FS_OPT_NAME          =   'n',
                            FS_OPT_INODE         =   'i',
                            FS_OPT_UNAME         =   'U',
                            FS_OPT_UID           =   'u',
                            FS_OPT_GNAME         =   'G',
                            FS_OPT_GID           =   'g',
                            FS_OPT_MODE          =   'M',
                            FS_OPT_ACCESS_T      =   'a',
                            FS_OPT_MOD_T         =   'm',
                            FS_OPT_STAT_CHANGE_T =   'c',
                            FS_OPT_DEVICE        =   'd',
                            FS_OPT_SIZE          =   's',
                            FS_OPT_PERMS         =   'p'};
    
        struct file_info{   std::string     path;
                            struct stat     *stat;
                            struct passwd   *pw;
                            struct group    *gr;};
    
        std::string formPath(unsigned int pid, unsigned long long fid);
        std::string readFileName(std::string path);
        std::string readMode(mode_t mode);
        std::unordered_map<int,std::string> fsFlags_;
        void readFileStatAttributes(std::string options,
                                    unsigned int fid, unsigned int pid,
                                    std::shared_ptr<UnlinkedFile> ulf); //fileStatReaderFuncs
        cphf_attribute processOpiton(char option, struct file_info * info);
        void addReader(unsigned int atribID, std::string attrib,
                       std::shared_ptr<UnlinkedFile> proc,
                       std::map<int, std::function<void(void)>>& procReaderFuncs);
        void do_read(std::string options, std::map<int, cphf_file_ptr>& procMap);
        void do_createAttribMap(std::unordered_map<int,std::string>& attribFlags);
};

#endif
