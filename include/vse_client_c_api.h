#pragma once
#include <stdlib.h>
#include <stdint.h>
#include "vse_c_common.h"

#ifdef __cplusplus
extern "C"{
#endif

/*
	数据记录绑定字段说明：
  字段定义： uuid(hex), pts(i64), utc-ts(i64), frameUri(s128), imgUri(s128)
  格式要求：uuid指定值需为标准uuid-hex-string；frameUri与imgUri可以为任意合理字符串，长度不得超过127字节。
*/


typedef struct BufferArray{
	size_t nct;
	VseBuffer buffers[0];
}BufferArray;


/*
  通用图搜库数据记录
*/
typedef struct DataRecord{
	int64_t idx;    //record-index
	float sim;      //feature compare similarity
	BufferArray* p_params;
}DataRecord;


/*
	释放SDK分配的BufferArray
*/
VSE_API void bufferarray_free(BufferArray* pBufferArray);


typedef struct BufferDataCbReceiver{
	//接收buffer个数的回调函数，可以为nullptr
	void(*getBufferCount)(uint32_t ,BufferDataCbReceiver*);	
	//(多次调用)接收一个个buffer数据的回调函数，不可为nullptr
	void(*getBufferData)(uint32_t, const char*, uint32_t,BufferDataCbReceiver*);
}BufferDataCbReceiver;


typedef struct DataRecordCbr{
	int64_t idx;    //record-index
	float sim;      //feature compare similarity
	BufferDataCbReceiver* params_cbr;
}DataRecordCbr;


/*
	Vse向量搜索引擎访问API类
*/
struct VseClient;

/**
 * 获取VseClient组件版本号
 * @return 返回版本号字符串，形式如“v1.2.3@Sep 10 2021”  
*/
VSE_API const char* vse_client_version();

/**
* 配置连接地址
* @param ip 目标特征搜索引擎地址
* @param port 目标特征搜索引擎端口号
* @param pei 错误信息结构体实例指针。如果API执行正常，则pei->code=0; 出现异常，则code非0，errmsg中填充异常信息。  
* @return 成功返回实例指针，失败返回nullptr。如果失败可以使用errno/GetLastError()获取系统级出错码
*/
VSE_API VseClient* client_init(const char* ip, int port, ErrInfo* pei);

VSE_API void client_free(VseClient* ph);

VSE_API void client_set_conn_tmo(VseClient* ph, uint32_t tmo_ms);

/**
* 列举全部数据库
* @return 返回的数据库名列表
* 
*/
VSE_API BufferArray* client_enum_all_dbs(VseClient* ph, ErrInfo* pei);

VSE_API void client_enum_all_dbs_cbr(VseClient* ph, BufferDataCbReceiver* cbr, ErrInfo* pei);

/**
* 创建数据库(注意，数据库名需要满足一般变量命名标准。在一套引擎内库名唯一。)
* @param dbname 数据库名
* @param fields 本库的用户扩展字段定义。语法为： 列名称1 数据类型, 列名称2 数据类型, ...
            列名称定义需要满足通用变量名定义标准：字母/数字开头，中间字符支持下划线
            用户可以定义0~64个字段。支持的‘数据类型’有：
                - 整形: i8,i16,i32,i64; 分别对应8-64位整形量，其中i8为无符号，i16-i64为有符号
                - 浮点: real/float; 均对应32位浮点量
                - 字符串: s8,s16,s32,s64,s128,s256; 分别对应了最大长度为8-256字节的字符串，注意长度需要考虑charset与encoding，不是“字符数”
            1个典型例子：
                bid i32, sid i32, path s256, ts i64, fv real
* 
*/
VSE_API void client_create_db_v2(VseClient* ph, const char* dbname, const char* fields, ErrInfo* pei);

/**
* 删除数据库(注意：执行删除操作前需要确保数据库未使用，即所有相关操作完成)
* @param dbname 数据库名
* 
*/
VSE_API void client_delete_db(VseClient* ph, const char* dbname, ErrInfo* pei);

/**
* 获取数据库记录数
* @param dbname 数据库名
* @return 返回的库记录数
* 
*/
VSE_API int64_t client_get_db_record_count(VseClient* ph, const char* dbname, ErrInfo* pei);

/**
* 插入记录（特征&绑定数据）
* @param dbname 数据库名
* @param feat 标准化特征数据
* @param fields 绑定参数的字段名列表，格式为： 列名称1,列名称2,... 注意列名称必须是create_db_v2传入的列名的子集
* @param values 基于前面fields指定列名称的顺序一一对应的值列表，格式为: 值1,值2, ... 注意，字符串值需要在外侧添加引号（支持单引号或者双引号，但必须配对），数值类型不加引号
* @return 图像索引uid值(>=0，库表级全局唯一)
		备注：
            a) push_record_v2执行时，传入的fields与values必须在数量与数据类型上匹配，否则会报错。
            b) fields指定字段顺序，不一定严格遵循建库时传入的字段名顺序，可以打乱。
            c) fields与values可以是建库时指定字段的子集，即可以不传全部字段的数据。在此情况下，忽略的字段数据默认填0，即数值数据为0，字符串为空串。
            d) values中指定的字符串字面量支持有限的转义字符(为传统c风格转义字符的子集)，有: \\'，\\"，\\\\，\\a，\\b，\\t，\\n，\\r，\\0，\\xNN（其中NN为两个16进制表示的字符，\x31表ASCII值为0x31的一个字符）
            e) values中指定的数字量，支持常规c风格十六进制表示(0x12345ab)与科学计数法表示(1e-5)
         有关fields和values的例子(与前面create_db_v2的例子对应)：
           fields:  bid,sid,ts,path
           values:  0x1743,-9750074,918789578467867834,"my test string"  
* 
*/
VSE_API int64_t client_push_record_v2(VseClient* ph, const char* dbname, VseBuffer* feat, const char* fields, const char* values, ErrInfo* pei);

/**
* 查询(满足条件的)记录数量
* @param dbname 数据库名
* @param wherestmt wherec语句 格式：[uuid]&-[uuid]...&,[video_pts_start]&|[video_pts_end]&,[realtime_utc_start]&|[realtime_utc_end]
* @return 记录数
* 
*/
//VSE_API int64_t client_select_record_count(VseClient* ph, const char* dbname, const char* wherestmt, ErrInfo* pei);


/**
* 特征检索
* @param dbname 数据库名
* @param feat 待检索的标准化特征数据（需FeatureTransformer.transform后的结果，非原生特征数据！）
* @param fields 检索时需要获得返回值的字段名列表，需要为建库字段名的子集。
* @param where_stmt 基于字段过滤的条件语句。详情见下
* @param min_sim 检索最小相似度限定（0-1.0）
* @param pRecs 检索返回记录数据存储数组。需要预分配。
* @param max_rec  最大返回结果数（一般为pRecs数组上限）
* @return 查询结果数(<=max_rec)
	成功检索将基于相似度排序、通过min_sim和max_rec双重限定后截断的结果返回。返回结果填充入pRecs里对应的成员中。每条记录有：
			:记录uid：同push_record返回值,delete_record参数值。为系统内分配的记录唯一id
            :相似度值：值域为[0,1.0]的相似度得分值。对于完全相同的向量相似度为1，完全不相同的向量相似度为0
            :参数列表：格式为list[str]的返回字段数据。与fields参数指定的字段一一对应。
	
	备注: where_stmt条件语句的支持能力有：
          - 支持逻辑运算符（not，and，or）与括号嵌套（任意层）
          - 支持x种比较运算符：
            - 基础比较运算：>, <, =, !=, >=, <=
            - 范围比较：<>, ><；分别表示IN-range，NOT-in-range。
            -   如：temp<>(11.2, 50)等价于temp>11.2 and temp<50，但效率更高
            - 集合比较：IN
            -   如：flag in (‘1’,’3’,’a’,’b’)等价于flag=‘1’ or flag=‘3’ or flag=‘a’ or flag=‘b’，但效率更高
        例子：
            bid>10 and path='daddy\\x23' or (fv in (2.2,5,7e3) and not ts<>(22.3,0x555) ) and sid=12
* 
*/
VSE_API int client_retrieve_records_v2(VseClient* ph, const char* dbname, VseBuffer* feat, const char* fields, const char* wherestmt, float min_sim,
	DataRecord* pRecs, uint32_t max_rec, ErrInfo* pei);

VSE_API int client_retrieve_records_cbr_v2(VseClient* ph, const char* dbname, VseBuffer* feat, const char* fields, const char* wherestmt, float min_sim,
	DataRecordCbr* pRecs, uint32_t max_rec, ErrInfo* pei);


/**
* select方式查询图像
* @param dbname 数据库名
* @param where_stmt where语句	
* @param pRecs 检索返回记录数据存储数组。需要预分配。
* @param max_rec  最大返回结果数（一般为pRecs数组上限）
* @return 查询结果数(<=max_rec)
* 
*/
//VSE_API int client_select_records(VseClient* ph, const char* dbname, const char* wherestmt, DataRecord* pRecs, uint32_t max_rec, ErrInfo* pei);


/**
* 根据索引删除记录
* @param dbname 数据库名
* @param index 索引值
* 
*/
VSE_API void client_delete_record(VseClient* ph, const char* dbname, int64_t index, ErrInfo* pei);

/**
* 根据条件删除记录
* @param dbname 数据库名
* @param wherestmt where语句
* 
*/
VSE_API void client_delete_record_ws(VseClient* ph, const char* dbname, const char* wherestmt, ErrInfo* pei);

VSE_API int client_db_scan_repair(VseClient* ph, const char* dbname, ErrInfo* pei);


VSE_API int client_enum_db_columns(VseClient* ph, const char* dbname, ErrInfo* pei);
#ifdef __cplusplus
} ///////////////////////////
#endif
/*
注:
    1、对于采用第三方原生特征的用户而言，需要使用FeatureTransformer.transform()执行特征转换，将原生特征转为标准化特征以后，才能用于VseClient的相关API。
    2、对于直接采用博云特征提取服务的应用场合，ExtractorCli.extract_feature返回的特征数据直接即为标准化特征，可以直接给VseClient使用。

使用说明：
-- 对于VseClient >>
	操作流程：实例声明 -> 初始化（set_net_addr） 
		-> [建库（create_db）//（如果库已经创建，建议调用一次访问API打开，如get_db_rec_ct）] -> 访问（增/删/改/查，API调用可并行）-> 删库（delete_db。注意删库前需保证没有对当前库未完成的访问操作）
		-> 实例析构
	并行性：对于VseClient实例，除了初始化方法set_net_addr为非线程安全函数，其余所有库操作函数均为线程安全，支持多线程并行调用。每个函数本身为阻塞执行（发出命令数据等待返回）。

*/