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


#ifndef CPHFREADER_H
#define CPHFREADER_H

#include "CPHFCommon.h"
#include "CPHFFile.h"

#include <fstream>
#include <string.h>
#include <sys/stat.h>


using cphf_file_ptr = std::shared_ptr<CPHFFile>;
using attribute_reader = std::pair<unsigned int, std::function<void(void)>>;

struct minor_dev_range{
    dev_t min;
    dev_t max;
    
    bool operator < (const struct minor_dev_range& lhs) const{
        return ((lhs.min < min) && (lhs.max < min));
    }
};

class CPHFReader{
    public:
        void read(std::string options, std::map<int, cphf_file_ptr>& procMap){
            do_read(options, procMap);
        };
        void createAttribMap(std::unordered_map<int,std::string>& attribFlags){
            do_createAttribMap(attribFlags);
        }
        virtual ~CPHFReader(void)=default;
        CPHFReader(CPHFReader const &) = delete;
        CPHFReader & operator=(CPHFReader const &) = delete;
    
    protected:
        CPHFReader(){};
        std::string format(unsigned int value);
        void buildDeviceMap(std::map<dev_t, std::map<struct minor_dev_range, std::string>>& deviceMap);
    
        static std::map<dev_t, std::map<struct minor_dev_range, std::string>> deviceMap;
        std::string readTime(time_t t, std::string format);
        std::string readETime(time_t t, std::string format);
        std::string readDevice(struct stat * info);
        std::string readPerms(mode_t mode);

    private:
        const dev_t FS_CHARCTER_DEVICE   =  12;
        virtual void do_read(std::string options, std::map<int, cphf_file_ptr>& procMap)=0;
        virtual void do_createAttribMap(std::unordered_map<int,std::string>& attribFlags)=0;

};


#endif
