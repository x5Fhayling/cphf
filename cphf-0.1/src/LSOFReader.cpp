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


#include "LSOFReader.h"

/*
 
 This class is completly focused on calling and reading in lsof -F +aL11
 which outputs various vields in the from of:
 <field_prefix><field_value>/n
 A buffered read is performed and strings of these fully formed prefixes and
 values are passed to an UnlikedFiles object for processing.
 
*/


void LSOFReader::do_createAttribMap(std::unordered_map<int,std::string>& attribFlags){
    attribFlags.insert(cphf_attribute(CPHF_ID_FD,    CPHF_LABEL_FD));
    attribFlags.insert(cphf_attribute(CPHF_ID_FLAGS, CPHF_LABEL_FLAGS));
    attribFlags.insert(cphf_attribute(CPHF_ID_PID,   CPHF_LABEL_PID));
}

void LSOFReader::do_read(std::string options, std::map<int, cphf_file_ptr>& procMap){

    const char * argv[5];
    argv[0]="lsof";
    argv[1]="-F";
    argv[2]=options.c_str();
    argv[3]="+aL1";
    argv[4]=NULL;
    
    getFilesInfo(const_cast<char**>(reinterpret_cast<const char**>(argv)), procMap);

}



void LSOFReader::getFilesInfo(char ** argv, std::map<int, cphf_file_ptr>& procMap){
    int pid;
    int stdout_pipe[2];
    if(pipe(stdout_pipe)==0){
    
    	pid =fork();
    
    	if(pid==-1){
        	printf("ERROR: Could not fork process.\n");
        
    	}else if(pid==0){
        	//Child process sends STDOUT back to parent
       	 	//thus the read end of the pipe must be closed
        	close(stdout_pipe[0]);
        	dup2(stdout_pipe[1], STDOUT_FILENO );
        
        	execvp(argv[0], argv);
        
        	close(stdout_pipe[1]);
        	exit(1);
    	}else{
        
        	close(stdout_pipe[1]);
           		 bufferedRead(stdout_pipe[0], procMap);
       		close(stdout_pipe[0]);
    	}	
    }else{
	printf("ERROR: could not form pipe for LSOFReader\n");
    }
}

void LSOFReader::bufferedRead(int& stdout_pipe, std::map<int, cphf_file_ptr>& procMap){
    
    //shred pointers are used
    cphf_file_ptr ulf;
    proc_file_ptr pf;
    
    unsigned long long txt_fd_counter = ULF_TXT_FD+1;
    
    //id and newproc are to make cases LSOF_ID_PID and LSOF_ID_FD code easier to read
    unsigned long long pid;
    unsigned long long fid;
    std::pair<int, cphf_file_ptr> newproc;

    
    //this buffersize is massive overkill. Need to find a saner number... 
    char charbuff[1024];
    char lsofid;
    std::stringstream strbuff;

    FILE* fin;
    fin = fdopen (stdout_pipe, "r");

    while(fscanf(fin,"%c%s\n",&lsofid,charbuff) >0){
    
        // using lsof until I can sort out how to best handle the file flags
        
        switch(lsofid){
            case LSOFReader::LSOF_ID_FD:    strbuff << charbuff;
                                if(strbuff.str()=="txt"){
                                    fid = txt_fd_counter;
                                    txt_fd_counter++;
                                }else{
                                    fid = atoi(charbuff);
                                }
                                ulf = pf->addFile(fid,
                                                  unlinked_file_ptr(new UnlinkedFile(pid, fid)));
                break;
            case LSOF_ID_FLAGS: ulf->addAttribute(CPHF_ID_FLAGS, strbuff.str());
                break;
            case LSOF_ID_PID:   strbuff << charbuff;
                                pid = atoi(charbuff);
                                newproc = std::make_pair(pid, cphf_file_ptr(new ProcFile()));
                                pf = std::dynamic_pointer_cast<ProcFile>(procMap.insert(newproc).first->second);
                                pf->addAttribute(CPHF_ID_PID, strbuff.str());
                break;
        }
  
        
        strbuff.clear();
        strbuff.str("");
        
    }
   fclose(fin);
}
