#include "include/sync.h"



namespace rafdb{

    Sync::Sync(RafDb *rafdb) {
        if (NULL == rafdb)
            VLOG(3)<<"*rafdb==NULL";
        rafdb_=rafdb;
        node_status_map.clear();
        sync_pool= new base::ThreadPool(sync_thread_num);
        sync_pool->StartWorkers();
        if (NULL == sync_pool)
            VLOG(3)<<"sync_pool create fail";
        if (rafdb_->NodeList.empty())
            VLOG(3)<<"NodeList is Empty";
        for(std::vector<rafdb::NodeInfo>::iterator iter=rafdb_->NodeList.begin(); iter != rafdb_->NodeList.end();iter++)
        {
            std::string ip_port=toIpPort(*iter);
            //node_status_map[ip_port] = ALIVE;
            rafdb::SyncClient *dbSync=new SyncClient(*iter);
            node_set_map[ip_port]=dbSync;
            VLOG(3)<<"put in map : "<<ip_port;

        }
    }

    //LKV_SYNC *Sync::create_lkv(const std::string &ip_port,const std::string &dbname,const std::string &key,const std::string &value) {
    //    size_t end_key_pos = ip_port.find_first_of(":");
    //    if (end_key_pos == std::string::npos)
    //    {   
    //        VLOG(3)<<"can't parser the key : "<<ip_port;
    //    }
    //    std::string ip(ip_port, 0, end_key_pos);
    //    std::string port(ip_port, end_key_pos+1, ip_port.size() - end_key_pos);
    //    LKV_SYNC *lkv=new LKV_SYNC(dbname,key,value,ip,atoi(port.c_str()));    
    //    return lkv;

    //}
    Sync::~Sync() {
        if (NULL!=sync_pool)
        {    
            delete sync_pool;
            sync_pool=NULL;
        }

        for (std::map< std::string,SyncClient * >::iterator iter=node_set_map.begin();iter!=node_set_map.end();iter++)
        {
            if (NULL!=iter->second)
            {    
                delete iter->second;
                node_set_map[iter->first]=NULL;
            }
        }
    }

    void Sync::sync_process(SyncDataWithConn* sync_data_conn) {
        std::string ip_port=toIpPort(sync_data_conn->node_info);
        if (node_set_map.find(ip_port)!=node_set_map.end()) {
            if(node_set_map[ip_port]->Exec(sync_data_conn->sync_data)) {
                VLOG(6)<<"sync success,ip_port"<<ip_port;
            }
            else {    
                VLOG(6)<<"sync fail,ip_port"<<ip_port;
            }
        }
        delete sync_data_conn;
    }


    
    void Sync::push(SyncDataWithConn* sync_data_conn)
    {
        VLOG(6)<<"push in sync_pool data : "<<sync_data_conn->sync_data.op;
        sync_pool->Add(base::NewOneTimeCallback(this,&Sync::sync_process,sync_data_conn));
        VLOG(6)<<"push in sync_pool complete";
    }


    //bool Sync::get_node_status(const rafdb::NodeInfo &info) {
    //    base::MutexLock lock(&mutex_);
    //    std::string ip_port=toIpPort(info);
    //    if (node_status_map.find(ip_port)==node_status_map.end())
    //    {
    //        VLOG(3)<<" ip is not in node_status_map : "<<info.ip;
    //        return false;
    //    }
    //    if (DEAD == node_status_map[ip_port]) {
    //        VLOG(3)<<" ip is not in node_status_map : "<<info.ip;
    //        return false;
    //    }
    //    return true;
    //}


    //void Sync::update_node_status(const rafdb::NodeInfo &info,int status) {
    //    VLOG(6)<<"come in update_node_status";
    //    if (status==0||status==1)
    //    {
    //        base::MutexLock lock(&mutex_);
    //        std::string ip_port=toIpPort(info);
    //        if (node_status_map.find(ip_port) == node_status_map.end())
    //        {
    //            VLOG(3)<<"the ip_port is not in map";
    //            return;
    //        }
    //        node_status_map[ip_port]=status;
    //    }    
    //    else
    //        VLOG(3)<<"the status is not support";
    //}

    std::string Sync::toIpPort(const rafdb::NodeInfo &info) {
        std::string ip=info.ip;
        int port=info.port;
        std::string ip_port;
        SStringPrintf(&ip_port,"%s:%d",ip.c_str(),port);
        return ip_port;
    }

}
