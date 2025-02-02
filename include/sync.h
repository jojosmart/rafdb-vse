#ifndef RAFDB_SYNC_H_
#define RAFDB_SYNC_H_


#include <vector>
#include <map>
#include <string>
#include <stdlib.h>
#include "base/logging.h"
#include "base/callback.h"
#include "base/string_split.h"
#include "base/thread_pool.h"
#include "base/concurrent_queue.h"
#include "manager.h"
#include "sync_client.h"



namespace {

    const int sync_thread_num=32;
    const int for_fail_thread_num=32;
    const int queue_top_size=100000000;
    const std::string cache_dir="cache_local_dir";
    typedef base::ConcurrentQueue<std::string> Queue;

}

namespace rafdb{
class RafDb;
class Sync
{
    public:
        void push(SyncDataWithConn* sync_data_conn);
        Sync(RafDb *rafdb);
        ~Sync();
    private:
        void sync_process(SyncDataWithConn* sync_data_conn);
        //bool get_node_status(const rafdb::NodeInfo &info);
        //void update_node_status(const rafdb::NodeInfo &info,int status);

        //base::Mutex mutex_;
        //base::Mutex flag_mutex_;
        //base::hash_map<rafdb::NodeInfo,int> node_status_map;
        //std::map< std::string, int > node_status_map;
        std::map< std::string, SyncClient*> node_set_map;
        //std::map< std::string, int > fail_time_map;
        //std::map< std::>
        std::string toIpPort(const rafdb::NodeInfo &info);
        
        //static Sync *m_pInstance;
        //std::string cache_dir;
        base::ThreadPool *sync_pool;
        //base::ThreadPool *for_fail_pool;
        RafDb *rafdb_;
        //Queue fail_queue;
        //DISALLOW_COPY_AND_ASSIGN(Sync);
        
};

 
}

#endif
