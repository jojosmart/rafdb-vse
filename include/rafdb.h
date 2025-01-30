
#ifndef RAFDB_RAFDB_H_
#define RAFDB_RAFDB_H_

#include <time.h>
#include <stdexcept>
#include <fstream>

#include <vector>
#include <map>
#include <string>
#include <set>
#include "third_party/jsoncpp/include/json.h"
#include "manager.h"
#include "accord.h"
#include "peer.h"
//#include "ts/base/crawl_config.h"

#include "base/logging.h"
#include "base/thrift.h"
#include "base/thread.h"
#include "base/mutex.h"
#include "base/concurrent_queue.h"
#include "base/flags.h"
#include "base/scoped_ptr.h"
#include "base/hash_tables.h"
#include "base/basictypes.h"
//#include "third_party/leveldb/db.h"
#include "proto/gen-cpp/RafdbService.h"
#include "global.h"

DECLARE_string(rafdb_self);
namespace rafdb {

    class IteratorChecker;
    class Manager;
    class Accord;

    class RafDb : virtual public rafdb::RafdbServiceIf {
        public:
            RafDb();
            virtual ~RafDb();
            void GetIpPortId(std::string& ip_out,int& port_out,int& id_out,
                    const std::string& ip_port_id);
            bool IsHealthy();
            bool IsLeader() {
                base::MutexLock lock(&leader_mutex_);
                if (self_id_ == leader_id_)
                    return true;
                else
                    return false;
            }
            int32_t GetLeaderId() {
                base::MutexLock lock(&leader_mutex_);
                return leader_id_;
            }
            bool SelfHealthy() {
                base::MutexLock lock(&mutex_);
                //TODO
                return true;
            }

            void GetCluserIPList(std::string& _return) {
                _return = cluster_ip_list_;
            }

           void VseClientVersion(std::string& _return);
           void ClientEnumAllDbs(ResDbnameList& _return);
           void ClientCreateDbV2(ErrInfoThr& _return, const std::string& dbname, const std::string& fields);
           void ClientDeleteDb(ErrInfoThr& _return, const std::string& dbname);
           void ClientGetDbRecordCount(ResIntInfo& _return, const std::string& dbname);
           void ClientPushRecordV2(ResIntInfo& _return, const std::string& dbname, const std::string& fields, const std::string& values);
           void ClientRetrieveRecordsV2(RetriveRecords& _return, const std::string& dbname, const std::string& fields, const std::string& wherestmt, const double min_sim, const int64_t max_rec);
           void ClientDeleteRecord(ErrInfoThr& _return, const std::string& dbname, const int64_t index);
           void ClientDeleteRecordWs(ErrInfoThr& _return, const std::string& dbname, const std::string& wherestmt);
           void ClientDbScanRepair(ResIntInfo& _return, const std::string& dbname);
           void ClientEnumDbColumns(ResIntInfo& _return, const std::string& dbname);


            void SendVote(const rafdb::Message& message);
            void ReplyVote(const rafdb::Message& message);
            void SendHeartBeat(const rafdb::Message& message);
            void ReplyHeartBeat(const rafdb::Message& message);
            void QueryLeaderId(const rafdb::Message& message);
            void ReplyLeaderId(const rafdb::Message& message);
            std::vector<NodeInfo> NodeList; 
        private:
            //friend class IteratorChecker;
            friend class Manager;
            friend class Accord;
            friend class Peer;
            scoped_ptr<Manager> manager_;
            scoped_ptr<Accord> accord_;
            void SetLeaderId(int leader_id) {
                base::MutexLock lock(&leader_mutex_);
                VLOG(3) << "leveldb server leader_id is set " << leader_id;
                leader_id_ = leader_id;
            }
            void Init();

            int32_t it_id_;
            base::Mutex leader_mutex_;
            base::Mutex mutex_;
            base::Mutex leader_data_sync_;
            base::ConcurrentQueue<Message> message_queue_;
            base::ConcurrentQueue<LKV*> lkv_queue_;
            int leader_id_;
            int self_id_;
            std::string ip_;
            int port_;
            std::string cluster_ip_list_;

            DISALLOW_COPY_AND_ASSIGN(RafDb);
    };
//    class IteratorChecker : public base::Thread {
//        public:
//            explicit IteratorChecker(RafDb* rafdb) : rafdb_(rafdb) { }
//            virtual ~IteratorChecker() { }
//
//        protected:
//            virtual void Run();
//
//        private:
//            RafDb* rafdb_;
//
//            DISALLOW_COPY_AND_ASSIGN(IteratorChecker);
//    };
//
//
//}  // namespace rafdb

#endif 
