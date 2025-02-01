
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

    typedef struct SyncDataStructure{
        std::vector<Variant> param_vec;
        int param_ct;
        DB_WRITE_OPERATION op;
        SyncDataStructure() {
            param_ct = 0;
        }
    }SyncDataStructure;

    typedef struct LKV_SYNC{
        std::string dbname;
        std::string key;
        std::string value;
        NodeInfo node_info;
        LKV_SYNC(const std::string dbname_p,
                const std::string key_p,
                const std::string value_p,
                const std::string ip_p,
                const int port_p):node_info(ip_p,port_p) {
            dbname = dbname_p;
            key = key_p;
            value = value_p;
        }
    }LKV_SYNC;

    
}

#endif
