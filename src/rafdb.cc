#include "include/rafdb.h"
#include "base/logging.h"
#include "base/hash.h"
#include "base/string_util.h"
#include "base/util.h"

DEFINE_string(rafdb_self, "", "rafdb self,192.168.11.12:1111:1");

namespace rafdb {

    RafDb::RafDb() {
        db_map_.clear();
        leader_id_ = 0;//no leader
        std::vector<std::string> tmp_v;
        SplitString(FLAGS_rafdb_self, ':', &tmp_v);
        ip_ = tmp_v[0];
        StringToInt(tmp_v[1], &port_);
        StringToInt(tmp_v[2], &self_id_);
        std::vector<std::string> tmp_v2;
        if (FLAGS_rafdb_list != "") {
            cluster_ip_list_ = FLAGS_rafdb_list;
            SplitString(FLAGS_rafdb_list, ',', &tmp_v2);
            for(int i=0;i<tmp_v2.size();i++) {
                std::string ip_tmp;
                int port_tmp;
                int id_tmp;
                GetIpPortId(ip_tmp,port_tmp,id_tmp,tmp_v2[i]);
                if (id_tmp != self_id_) {
                    //node_list except self
                    rafdb::NodeInfo tmp;
                    tmp.ip = ip_tmp;
                    tmp.port = port_tmp;
                    NodeList.push_back(tmp);
                }
            }
        }
        Init();

    }

    RafDb::~RafDb() {
        //TODO traverse dm_map_ and close 
    }

    void RafDb::Init() {
        accord_.reset(new Accord(this,ip_,port_));
        accord_->Start();//start accord thread
        manager_.reset(new Manager(this));
        manager_->Start();

    }


    void RafDb::Get(std::string &result, const std::string &dbname,
            const std::string &key) {
        result = "test";
        //result.clear();
        //leveldb::DB *db = NULL;
        //{
        //    base::hash_map<std::string, leveldb::DB*>::iterator it;
        //    base::MutexLock lock(&db_map_mutex_);
        //    it = db_map_.find(dbname);
        //    if (it == db_map_.end()) {
        //        return;
        //    }
        //    db = it->second;
        //}

        //leveldb::Status status = db->Get(roptions_, key, &result);
        //if (status.IsNotFound()) {
        //    result.clear();
        //    return;
        //}
        //if (!status.ok()) {
        //    LOG(ERROR) << "Get, dbname:" << dbname << " key:" << key;
        //    result.clear();
        //    return;
        //}
        return;
    }

    
    void RafDb::SendVote(const rafdb::Message& message) {
        _setMsg(message, MessageType::VOTEREQ)
    }

    void RafDb::ReplyVote(const rafdb::Message& message) {
        _setMsg(message, MessageType::VOTEREP)
    }

    void RafDb::SendHeartBeat(const rafdb::Message& message) {
        _setMsg(message, MessageType::HEARTREQ)
    }

    void RafDb::ReplyHeartBeat(const rafdb::Message& message) {
        _setMsg(message, MessageType::HEARTREP)
    }

    void RafDb::QueryLeaderId(const rafdb::Message& message) {
        _setMsg(message, MessageType::LEADERREQ)
    }

    void RafDb::ReplyLeaderId(const rafdb::Message& message) {
        _setMsg(message, MessageType::LEADERREP)
    }

    bool RafDb::IsHealthy() {
        return true;
    }

    bool RafDb::LSet(const std::string &dbname, const std::string &key,
            const std::string &value) {
        //if(Set(dbname,key,value)) {
        //  LKV *tmp_lkv = new LKV();
        //  tmp_lkv->dbname = dbname;
        //  tmp_lkv->key = key;
        //  tmp_lkv->value = value;
        //  lkv_queue_.Push(tmp_lkv);
        //  VLOG(5)<<"set success,push to queue";
        //  return true;
        //}  
        return false;
    }

//TODO
    void RafDb::VseClientVersion(std::string& _return) {
    }
    void RafDb::ClientEnumAllDbs(ResDbnameList& _return) {
    }
    void RafDb::ClientCreateDbV2(ErrInfoThr& _return, const std::string& dbname, const std::string& fields) {
    }
    void RafDb::ClientDeleteDb(ErrInfoThr& _return, const std::string& dbname) {
    }
    void RafDb::ClientGetDbRecordCount(ResIntInfo& _return, const std::string& dbname) {
    }
    void RafDb::ClientPushRecordV2(ResIntInfo& _return, const std::string& dbname, const std::string& fields, const std::string& values) {
    }
    void RafDb::ClientRetrieveRecordsV2(RetriveRecords& _return, const std::string& dbname, const std::string& fields, const std::string& wherestmt, const double min_sim, const int64_t max_rec) {
    }
    void RafDb::ClientDeleteRecord(ErrInfoThr& _return, const std::string& dbname, const int64_t index) {
    }
    void RafDb::ClientDeleteRecordWs(ErrInfoThr& _return, const std::string& dbname, const std::string& wherestmt) {
    }
    void RafDb::ClientDbScanRepair(ResIntInfo& _return, const std::string& dbname) {
    }
    void RafDb::ClientEnumDbColumns(ResIntInfo& _return, const std::string& dbname) {
    }

}
