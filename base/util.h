// Author:Zhouxiaoliang

#ifndef BASE_UTIL_H_ 
#define BASE_UTIL_H_ 

#include "base/string_util.h"
namespace rafdb {
    void GetIpPortId(std::string& ip_out,int& port_out,int& id_out,
            const std::string& ip_port_id); 

    std::string toIpPort(const NodeInfo &info);

}

#endif
