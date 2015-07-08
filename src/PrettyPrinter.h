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


#ifndef PRETTYPRINTER_H
#define PRETTYPRINTER_H

#include "ProcFile.h"
#include "UnlinkedFile.h"


class PrettyPrinter{
    
    public:
        static void printFiles(std::vector<std::shared_ptr<CPHFFile>>& printQueue);
    private:
        PrettyPrinter(void)= default;
        ~PrettyPrinter(void)= default;
};


#endif
