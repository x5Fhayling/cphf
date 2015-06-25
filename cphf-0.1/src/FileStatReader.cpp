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


#include "FileStatReader.h"

///////////////////////////////////////////////////////////////////////////////
//  Functions called by interface: do_createAttribMap, do_read.
//  See CPHFReader.h for more info
///////////////////////////////////////////////////////////////////////////////

void FileStatReader::do_createAttribMap(std::unordered_map<int,std::string>& attribFlags){
    attribFlags.insert(cphf_attribute(CPHF_ID_FILENAME,      CPHF_LABEL_FILENAME));
    attribFlags.insert(cphf_attribute(CPHF_ID_INODE,         CPHF_LABEL_INODE));
    attribFlags.insert(cphf_attribute(CPHF_ID_UID,           CPHF_LABEL_UID));
    attribFlags.insert(cphf_attribute(CPHF_ID_UGID,          CPHF_LABEL_UGID));
    attribFlags.insert(cphf_attribute(CPHF_ID_FILEMODE,      CPHF_LABEL_FILEMODE));
    attribFlags.insert(cphf_attribute(CPHF_ID_ACCESS_T,      CPHF_LABEL_ACCESS_T));
    attribFlags.insert(cphf_attribute(CPHF_ID_MOD_T,         CPHF_LABEL_MOD_T));
    attribFlags.insert(cphf_attribute(CPHF_ID_STAT_CHANGE_T, CPHF_LABEL_STAT_CHANGE_T));
    attribFlags.insert(cphf_attribute(CPHF_ID_DEVICE,        CPHF_LABEL_DEVICE));
    attribFlags.insert(cphf_attribute(CPHF_ID_PID,           CPHF_LABEL_PID));
    attribFlags.insert(cphf_attribute(CPHF_ID_FILESIZE,      CPHF_LABEL_FILESIZE));
    attribFlags.insert(cphf_attribute(CPHF_ID_PERMS,         CPHF_LABEL_PERMS));
    
}

void FileStatReader::do_read(std::string options, std::map<int, cphf_file_ptr>& procMap){
    std::stringstream strbuff;
    FileStatReader *fsr = new FileStatReader;
    
    std::function<void(unsigned int, unsigned int, std::shared_ptr<UnlinkedFile>)> func;
    
    using namespace std::placeholders;
    func = std::bind(&FileStatReader::readFileStatAttributes, fsr, options, _1, _2, _3);
    
    for(auto& procMap_itr : procMap){
        proc_file_ptr proc = std::dynamic_pointer_cast<ProcFile>(procMap_itr.second);
        proc->addFileAttributes(func);
    }
    
    delete fsr;
    
}



std::string FileStatReader::formPath(unsigned int pid, unsigned long long fid){
    
    std::stringstream strbuff;
    strbuff << "/proc/" << pid;
    
    if (fid > ULF_TXT_FD) {
        strbuff << "/exe";
    }else{
        strbuff << "/fd/" << fid;
    }
    
    return strbuff.str();
}

std::string FileStatReader::readFileName(std::string path){
    //the /proc/ files only psudo links so lstat won't work
    //ergo you can't us lstat to get the length of the filename
    long long filenameLength = 1024;
   
    
    std::stringstream strbuff;
    std::string filenamestr="";
    char * filename = nullptr;
    
    filename = reinterpret_cast<char*>(malloc(filenameLength+1));
    memset(filename, 0, filenameLength);
   
    //buffer is too small so doulble it and try again...
    while(filename!=nullptr &&
          readlink(path.c_str(), filename, filenameLength) == filenameLength){
        filenameLength = filenameLength << 2;
        filename = reinterpret_cast<char*>(realloc(filename, filenameLength+1));
        memset(filename, 0, filenameLength);
    }

    if (filename!=nullptr) {
        //readlink doesn't null terminate the string so
        //so we have to do it ourselves
        filename[filenameLength] = '\0';
        filenamestr = std::string(filename);
        free(filename);
    }
    
    return filenamestr;
}


cphf_attribute FileStatReader::processOpiton(char option, struct file_info * info){
 
    std::string attrib;
    int attribID;
    
    switch(option){
        case FS_OPT_NAME:   attrib   = readFileName(info->path);
                            attribID = CPHF_ID_FILENAME;
            break;
        case FS_OPT_INODE:  attrib   = format(info->stat->st_ino);
                            attribID = CPHF_ID_INODE;
            break;
        case FS_OPT_UID:    attrib   = format(info->stat->st_uid);
                            attribID = CPHF_ID_UID;
            break;
        case FS_OPT_UNAME: ;//to be implemented
            break;
        case FS_OPT_GID:    attrib   = format(info->stat->st_gid);
                            attribID = CPHF_ID_UGID;
            break;
        case FS_OPT_GNAME: ;//to be implemented
            break;
        case FS_OPT_MODE:   attrib   = readMode(info->stat->st_mode);
                            attribID = CPHF_ID_FILEMODE;
            break;
        case FS_OPT_ACCESS_T:attrib   = readTime(info->stat->st_atime,
                                                 CPHF_FORMAT_DATE_TIME);
                             attribID = CPHF_ID_ACCESS_T;
            break;
        case FS_OPT_MOD_T:  attrib   = readTime(info->stat->st_mtime,
                                                CPHF_FORMAT_DATE_TIME);
                            attribID = CPHF_ID_MOD_T;
            break;
        case FS_OPT_STAT_CHANGE_T:  attrib   = readTime(info->stat->st_ctime,
                                                        CPHF_FORMAT_DATE_TIME);
                                    attribID = CPHF_ID_STAT_CHANGE_T;
            break;
        case FS_OPT_DEVICE: attrib   = readDevice(info->stat);
                            attribID = CPHF_ID_DEVICE;
            break;
        case FS_OPT_SIZE:   attrib   = format(info->stat->st_size);
                            attribID = CPHF_ID_FILESIZE;
            break;
        case FS_OPT_PERMS:  attrib   = readPerms(info->stat->st_mode);
                            attribID = CPHF_ID_PERMS;
            break;
        default:    printf("ERROR: proc option %c not recognized\n",option);
    }
    return cphf_attribute(attribID, attrib);
}
void FileStatReader::readFileStatAttributes(std::string options,
                                            unsigned int fid, unsigned int pid,
                                            std::shared_ptr<UnlinkedFile> ulf){   //fileStatReaderFuncs
    
    std::map<int, std::function<void(void)>> fileStatFuncs;
    std::string attrib;
    struct file_info * info;
    info = new struct file_info;
    info->stat = new struct stat;
 
    info->path = formPath(pid, fid);

    cphf_attribute attributePair;
    if(stat(info->path.c_str(), info->stat) != CPHF_ERROR){
        
        for(size_t i=0; i< options.length(); i++){
            attributePair=processOpiton(options[i], info);
            addReader(attributePair.first, attributePair.second, ulf, fileStatFuncs);
        }
        
        for(auto & fileStatFuncs_itr : fileStatFuncs){
            fileStatFuncs_itr.second();
        }
    }
    delete info->stat;
    delete info;
}

std::string FileStatReader::readMode(mode_t mode){
    std::string modeStr;
    
    switch (mode & S_IFMT) {
        case S_IFREG:   modeStr = "Regular File";
            break;
        case S_IFDIR:   modeStr = "Directory";
            break;
        case S_IFCHR:   modeStr = "Character Device";
            break;
        case S_IFBLK:   modeStr = "Block Device";
            break;
        case S_IFIFO:   modeStr = "FIFO (named pipe)";
            break;
        case S_IFLNK:   modeStr = "Symbolic Link";
            break;
        case S_IFSOCK:  modeStr = "Socket";
            break;
        default:        modeStr = "UNKNOWN";
            break;
    }

    return modeStr;
}

// adds readers to the reader map.  Because the order of the outputted fields is dependent
// upon when they are added as an attribute all attributes are added to a map and that map is
// iterated through once all attributes for a proc have been collected


void FileStatReader::addReader(unsigned int atribID, std::string attrib,
                               std::shared_ptr<UnlinkedFile> proc,
                               std::map<int, std::function<void(void)>>& procReaderFuncs){

    attribute_reader ar;
    ar = std::make_pair(atribID, std::bind(&UnlinkedFile::addAttribute, proc, atribID, attrib));
    procReaderFuncs.insert(ar);
    
}
