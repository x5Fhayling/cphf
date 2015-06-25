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


#ifndef CPHFFILES_H
#define CPHFFILES_H

#include "CPHFCommon.h"

class CPHFFile{

	public:
        void addAttribute(unsigned int fileid, std::string value){
            do_addAttribute(fileid, value);
        }
        void printAttributes(std::function<void(std::string, std::string)> prettyprinter){
            do_printAttributes(prettyprinter);
        }
        static std::unordered_map<int,std::string> attributeFlags;
    
        virtual ~CPHFFile(void){}
        CPHFFile(CPHFFile const &) = delete;
        CPHFFile & operator=(CPHFFile const &) = delete;

    protected:
        CPHFFile(){};

	private:
        virtual void do_addAttribute(unsigned int fileid, std::string value)=0;
        virtual void do_printAttributes(std::function<void(std::string,
                                                           std::string)>prettyprinter)=0;

};

using cphf_file_ptr = std::shared_ptr<CPHFFile>;

#endif
