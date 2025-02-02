
#ifndef RAFDB_GLOBAL_H_
#define RAFDB_GLOBAL_H_

#include <string>
#include <third_party/boost/boost/variant.hpp>
#include <vector>
namespace rafdb {
    
    typedef boost::variant<int64_t, double, std::string> Variant;

    enum DB_WRITE_OPERATION {
        CREATE_DB = 0,
        DELETE_DB = 1,
        PUSH_RECORD = 2,
        DEL_RECORD = 3,
        DEL_RECORD_WS = 4,
        DB_SCAN_REPAIR = 5
    }

    typedef struct NodeInfo{
        std::string ip;
        int port;
        NodeInfo()
        {}
        NodeInfo(const std::string ip_p,const int port_p):ip(ip_p),port(port_p) {
        }
    }NodeInfo;

    typedef struct SyncData{
        std::vector<Variant> param_vec;
        int param_ct;
        DB_WRITE_OPERATION op;
        SyncData() {
            param_ct = 0;
        }
        SyncData(const SyncData& sync_data) {
            param_vec = sync_data.param_vec;
            op = sync_data.op;
            param_ct = sync_data.param_ct;
        }

    }SyncData;

    typedef struct SyncDataWithConn{
        SyncData sync_data;
        NodeInfo node_info;
        SyncDataWithConn(
                const SyncData& sync_data_param,
                const std::string ip_p,
                const int port_p):node_info(ip_p,port_p), sync_data(sync_data_param) {
        }
    }SyncDataWithConn;
    
}

#endif
