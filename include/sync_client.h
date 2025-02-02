
#ifndef RAFDB_RAFDB_SYNC_H_
#define RAFDB_RAFDB_SYNC_H_

#include <string>
#include <memory>
#include "base/scoped_ptr.h"
#include "base/mutex.h"
#include "base/thrift.h"
#include "base/string_util.h"
#include "proto/gen-cpp/RafdbService.h"
#include "global.h"


namespace rafdb {


class SyncClient {
 public:
  SyncClient(rafdb::NodeInfo node_info):connected_(false) {
    host_ = node_info.ip;
    port_= node_info.port;
    SStringPrintf(&server_, "%s:%d", node_info.ip.c_str(), node_info.port);
  }
  ~SyncClient() {
  }
  bool IsConnected() {
    return Connect();
  }
  const std::string& host() const {
    return host_;
  }
  const int& port() const {
    return port_;
  }
  const std::string& server() const {
    return server_;
  }
 bool Exec(const SyncData& sync_data) {
    base::MutexLock lock(&mutex_);
    try {
      if(Connect()) {
          //TODO 根据op类型,rpc
          switch (sync_data.op) {
              case CREATE_DB:
                VLOG(3)<<"CREATE_DB SYNC";
                return true;
              case DELETE_DB:
                VLOG(3)<<"DELETE_DB SYNC";
                return true;
              case PUSH_RECORD:
                VLOG(3)<<"PUSH_RECORD SYNC";
                return true;
              case DEL_RECORD:
                VLOG(3)<<"DEL_RECORD SYNC";
                return true;
              case DEL_RECORD_WS:
                VLOG(3)<<"DEL_RECORD_WS SYNC";
                return true;
              case DB_SCAN_REPAIR:
                VLOG(3)<<"DB_SCAN_REPAIR SYNC";
                return true;
              default:
                VLOG(3)<<"UNKNOWN OPERATION";
                return false;
          }
         
        //return service_->Set(dbname,key,value); 
      }
      connected_ = false;
      return false;
    } catch(...) {
      connected_ = false;
      return false;
    }
  }

 private:
  bool Connect();
  std::string host_;
  int port_;
  bool connected_;
  std::string server_;
  base::Mutex mutex_;
  boost::shared_ptr<TSocket> socket_;
  boost::shared_ptr<TFramedTransport> transport_;
  boost::shared_ptr<TBinaryProtocol> protocol_;
  boost::shared_ptr<RafdbServiceClient> service_;
  //std::shared_ptr<TSocket> socket_;
  //std::shared_ptr<TFramedTransport> transport_;
  //std::shared_ptr<TBinaryProtocol> protocol_;
  //std::shared_ptr<RafdbServiceClient> service_;



  DISALLOW_COPY_AND_ASSIGN(SyncClient);
};

}  // namespace rafdb

#endif 
