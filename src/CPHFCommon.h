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


#ifndef CPHFCOMMON_H
#define CPHFCOMMON_H

#include <sstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <utility>  //std::pair
#include <stdio.h>  //printf
#include <memory>   //shared_ptr used for std::vector<CPHFFile*>
                    //      to prevent memory leaks

#include <linux/version.h>
#include <unistd.h>


#define CPHF_SUCCESS 1
#define CPHF_ERROR -1

//#define BUFFERSIZE  1024


       
#define CPHF_FORMAT_DATE_TIME "%a %b %d, %Y %T %Z"
#define CPHF_FORMAT_TIME  "%d:%02d:%02d"
//attriute pairs take the form: CPHF_ID, value
using cphf_attribute = std::pair<int, std::string>;



//List of attribute IDs and Labels



// lsof defined

enum cphf_ids{  CPHF_ID_PID         =   5,
                CPHF_ID_FD          =   105,
                CPHF_ID_LOCK		=   130,
                CPHF_ID_CMD         =   0,
                CPHF_ID_FILENAME    =   2,
                CPHF_ID_DEVICE      =   6,
                CPHF_ID_INODE		=   8,
                CPHF_ID_PROCGID     =   10,
                CPHF_ID_PPID        =   15,
                CPHF_ID_LOGINNAME   =   20,
                CPHF_ID_UID         =   25,
                CPHF_ID_UGID_NAME   =   30,
                CPHF_ID_UGID        =   35,
                CPHF_ID_FILEMODE    =   39,
                CPHF_ID_PSTATE      =   40,
                CPHF_ID_FLAGS       =   41,
                CPHF_ID_PERMS       =   42,
                CPHF_ID_PSTART_TIME =   45,
                CPHF_ID_ACCESS_T    =   46,
                CPHF_ID_MOD_T       =   47,
                CPHF_ID_STAT_CHANGE_T=  48,
                CPHF_ID_PCPU_TIME   =   50,
                CPHF_ID_RAW_DEVICE  =   54,
                CPHF_ID_FILESIZE    =   57,
                CPHF_ID_PVMS        =   60,
                CPHF_ID_PRSS        =   65,
                CPHF_ID_TEXT_REGION =   70,
                CPHF_ID_DATA_REGION =   75,
                CPHF_ID_STACK_REGION=   80,
                CPHF_ID_ESP         =   85,
                CPHF_ID_EPI         =   90};


const std::string	CPHF_LABEL_PID              = "Process ID (PID)";
const std::string	CPHF_S_LABEL_PID            =   "PID";

const std::string	CPHF_LABEL_FD               =   "File Descriptor";
const std::string	CPHF_S_LABEL_FD             =   "FD";


const std::string   CPHF_LABEL_LOCK             =   "Lock (r|w|u)";
const std::string	CPHF_S_LABEL_LOCK           =   "FLOCK";

//  /roc defined

const std::string	CPHF_LABEL_CMD              =   "Command";
const std::string	CPHF_S_LABEL_CMD            =   "CMD";


const std::string	CPHF_LABEL_FILENAME         =   "Filename";  //fid
const std::string	CPHF_S_LABEL_FILENAME       =   "FNAME";

const std::string   CPHF_LABEL_DEVICE           =   "Device";
const std::string	CPHF_S_LABEL_DEVICE         =   "DEVID";



const std::string	CPHF_LABEL_INODE            =   "Inode";     //fid
const std::string	CPHF_S_LABEL_INODE          =   "INODE";

const std::string	CPHF_LABEL_PROCGID          =   "Process Group ID (PGID)";
const std::string	CPHF_S_LABEL_PGID           =   "PGID";

const std::string	CPHF_LABEL_PPID             =   "Parent PID";
const std::string	CPHF_S_LABEL_PPID           =   "PPID";

const std::string   CPHF_LABEL_LOGINNAME        =   "User Name";
const std::string	CPHF_S_LABEL_LOGINNAME      =   "UNAME";

const std::string   CPHF_LABEL_UID              =   "User ID (UID)";
const std::string	CPHF_S_LABEL_UID            =   "UID";

const std::string   CPHF_LABEL_UGID_NAME        =   "User Group Name";
const std::string	CPHF_S_LABEL_UGID_NAME      =   "UGN";

const std::string   CPHF_LABEL_UGID             =   "User Group ID (UGID)";
const std::string	CPHF_S_LABEL_UGID           =   "UGID";

const std::string	CPHF_LABEL_FILEMODE         =   "File Mode"; //fid
const std::string	CPHF_S_LABEL_FILEMODE       =   "MODE";

const std::string	CPHF_LABEL_PSTATE           =   "Current State";
const std::string	CPHF_S_LABEL_PSTATE         =   "STATE";

const std::string	CPHF_LABEL_FLAGS            =   "File Flags";
const std::string	CPHF_S_LABEL_FLAG           =   "FFLAG";

const std::string   CPHF_LABEL_PERMS            =   "Permissions";
const std::string	CPHF_S_LABEL_PERMS          =   "PERM";

const std::string   CPHF_LABEL_PSTART_TIME      =   "Start Time";
const std::string	CPHF_S_LABEL_PSTART_TIME    =   "STRTT";

const std::string   CPHF_LABEL_ACCESS_T         =   "Last Accessed";//fid
const std::string	CPHF_S_LABEL_ACCESS_T       =   "ACCT";

const std::string   CPHF_LABEL_MOD_T            =   "Last Modified"; //fid
const std::string	CPHF_S_LABEL_MOD_T          =   "MODT";

const std::string   CPHF_LABEL_STAT_CHANGE_T    =   "Last Status Change";//fid
const std::string	CPHF_S_LABEL_STAT_CHAGE_T   =   "STATT";


const std::string   CPHF_LABEL_PCPU_TIME        =   "CPU Time";
const std::string	CPHF_S_LABEL_PCUP_TIME      =   "CPUT";

const std::string   CPHF_LABEL_RAW_DEVICE       =   "Raw Device";
const std::string	CPHF_S_LABEL_RAW_DEVICE     =   "RDEV";

const std::string   CPHF_LABEL_FILESIZE         =   "File Size (bytes)"; //fid
const std::string	CPHF_S_LABEL_FILESIZE       =   "FSIZE";

const std::string   CPHF_LABEL_PVMS             =   "Virtual Memory (bytes)";
const std::string   CPHF_S_LABEL_PVMS           =   "VMS";

const std::string   CPHF_LABEL_PRSS             =   "Resident Set (pages)";
const std::string	CPHF_S_LABEL_PRSS           =   "RSS";

const std::string	CPHF_LABEL_TEXT_REGION      =   "Text Region";
const std::string	CPHF_S_LABEL_TEXT_REGION    =   "TEXTR";

const std::string   CPHF_LABEL_DATA_REGION      =   "Data Region";
const std::string   CPHF_S_LABEL_DATA_REGION    =   "DATAR";

const std::string	CPHF_LABEL_STACK_REGION     =   "Stack";
const std::string	CPHF_S_LABEL_STACK_REGION   =   "STCKR";

const std::string	CPHF_LABEL_ESP              =   "Stack Pointer"; //not explicitly implemented
const std::string	CPHF_S_LABEL_ESP            =   "STCKP";

const std::string	CPHF_LABEL_EPI              =   "Instruction Pointer";
const std::string	CPHF_S_LABEL_EPI            =   "INSTP";

#endif
