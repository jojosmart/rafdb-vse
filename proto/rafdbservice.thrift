include "accord.thrift"

namespace cpp rafdb


struct VseBufferThr {
    1:string data,
    //size不包含\0
    2:i64 size
}

struct ErrInfoThr {
    1:i32 code,
    2:string errmsg
}

struct BufferArrayThr {
    1:i32 nct,
    2:list<VseBufferThr> buffers
}


//通用图搜库数据记录
struct DataRecordThr {
    1:i64 idx,    //record-index
    2:double sim,      //feature compare similarity
    3:list<BufferArrayThr> p_params
}

struct ResDbnameList {
    1:list<BufferArrayThr> result,
    2:ErrInfoThr error_info
}

struct ResIntInfo {
    1:i64 result,
    2:ErrInfoThr error_info
}

struct RetriveRecords {
    1:i64 count,
    2:list<DataRecordThr> data_records,
    3:ErrInfoThr error_info
}

service RafdbService {
//db interface
    string VseClientVersion();//获取VseClient组件版本号
    ResDbnameList ClientEnumAllDbs();//返回all数据库名列表
    ErrInfoThr ClientCreateDbV2(1:string dbname, 2:string fields);
    ErrInfoThr ClientDeleteDb(1:string dbname);
    ResIntInfo ClientGetDbRecordCount(1:string dbname);
    ResIntInfo ClientPushRecordV2(1:string dbname, 2:string fields, 3:string values);
    RetriveRecords ClientRetrieveRecordsV2(1:string dbname, 2:string fields, 3:string wherestmt, 4:double min_sim, 5:i64 max_rec);
    ErrInfoThr ClientDeleteRecord(1:string dbname, 2:i64 index);
    ErrInfoThr ClientDeleteRecordWs(1:string dbname, 2:string wherestmt);
    ResIntInfo ClientDbScanRepair(1:string dbname);
    ResIntInfo ClientEnumDbColumns(1:string dbname);


    bool IsHealthy();
    bool IsLeader();
    i32 GetLeaderId(); //for user
    string GetCluserIPList(); //for user,192.168.14.146:10020:1,192.168.14.146:10021:2,192.168.14.146:10022:3

//raft interface
    oneway void SendVote(1: accord.Message message);
    oneway void ReplyVote(1:  accord.Message message);
    oneway void SendHeartBeat(1:  accord.Message message);
    oneway void ReplyHeartBeat(1:  accord.Message message);
    oneway void QueryLeaderId(1:  accord.Message message);
    oneway void ReplyLeaderId(1:  accord.Message message);
   
}

