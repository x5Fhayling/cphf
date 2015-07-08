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


#include "PrettyPrinter.h"

void PrettyPrinter::printFiles(std::vector<std::shared_ptr<CPHFFile>>& printQueue){
    
    ProcFile* proc;
    UnlinkedFile* ulfile;
    
    for(auto &it : printQueue){
        
        if((proc = dynamic_cast<ProcFile*>(it.get()))){
            
            printf("%s\n",std::string(80,'=').c_str());
                proc->printAttributes(
                     [](std::string label, std::string value)->void{
                     printf("%-23s  %s\n", label.c_str(), value.c_str());
                }
            );
            printf("\n");
            
        }else if((ulfile = dynamic_cast<UnlinkedFile*>(it.get()))){
            
            ulfile->printAttributes(
                [](std::string label, std::string value)->void{
                    printf("\t%-25s  %s\n", label.c_str(), value.c_str());
                }
            );
            
            printf("\n");
        }
    }
}
