// Author:Zhouxiaoliang
#include "base/util.h"

namespace rafdb {

void GetIpPortId(std::string& ip_out,int& port_out,int& id_out,
            const std::string& ip_port_id) {
        std::vector<std::string> tmp_v;
        SplitString(ip_port_id, ':', &tmp_v);
        ip_out = tmp_v[0];
        int port;
        StringToInt(tmp_v[1], &port);
        port_out = port;
        int id;
        StringToInt(tmp_v[2], &id);
        id_out = id;
    }

}
