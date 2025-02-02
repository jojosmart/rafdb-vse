
#include "include/manager.h"
namespace {
  //2ms
  const int kFlushInterval = 2000; //us
}


namespace rafdb {

  void Manager::Init(RafDb*rafdb_p) {
    sync_=new Sync(rafdb_p);
  }

  void Manager::Run() {
    while ( true ) {
      if (rafdb_->GetLeaderId() > 0 && rafdb_->GetLeaderId() == rafdb_->self_id_) { // is leader
        SyncData* tmp = NULL;
        rafdb_->lkv_queue_.Pop(tmp);
        if (tmp == NULL) {
          continue;
        }
        for(std::vector<rafdb::NodeInfo>::iterator iter=rafdb_->NodeList.begin(); iter != rafdb_->NodeList.end();iter++)
        {
          SyncDataWithConn* sync_data_conn = new SyncDataWithConn(*tmp,iter->ip,iter->port);
          sync_->push(sync_data_conn);
        }
        delete tmp;
      }
      else {
        usleep(kFlushInterval);
      }

    }
  }
}
