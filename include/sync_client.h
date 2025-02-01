
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
 bool Set(const std::string dbname,
          const std::string &key,
          const std::string &value) {
    base::MutexLock lock(&mutex_);
    try {
      if(Connect()) {
        return service_->Set(dbname,key,value); 
      }
      VLOG(3)<<"set ldb fail";
      connected_ = false;
      return false;
    } catch(...) {
      VLOG(3)<<"set ldb fail";
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
