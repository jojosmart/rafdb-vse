
#include <string>

#include "base/at_exit.h"
#include "base/flags.h"
#include "base/thrift.h"
#include "base/logging.h"
#include "base/util.h"
#include "file/file.h"
#include "include/rafdb.h"
#include "third_party/thrift/include/server/TThreadedServer.h"
#include "third_party/thrift/include/server/TServer.h"
#include "third_party/thrift/include/server/TSimpleServer.h"
#include "third_party/thrift/include/server/TThreadPoolServer.h"
#include "third_party/thrift/include/server/TNonblockingServer.h"


using apache::thrift::server::TNonblockingServer;
using apache::thrift::server::TThreadPoolServer;
using apache::thrift::concurrency::PosixThreadFactory;
using apache::thrift::protocol::TProtocolFactory;
using apache::thrift::protocol::TBinaryProtocolFactory;
using apache::thrift::transport::TTransportFactory;
using apache::thrift::transport::TBufferedTransportFactory;
using apache::thrift::TProcessor;


DEFINE_int32(thread_num, 1000, "num of thrift server threads");
DEFINE_int32(logfile, "./log/rafdb.log", "num of thrift server threads");


int main(int argc, char **argv) {
  base::AtExitManager exit_manager;
  base::ParseCommandLineFlags(&argc, &argv, true);

  logging::InitLogging(FLAGS_logfile.c_str(), logging::LOG_ONLY_TO_FILE,
      logging::DONT_LOCK_LOG_FILE, logging::DELETE_OLD_LOG_FILE);

  //int pid = getpid();
  //LOG(INFO) << "pid:" << pid;
  //file::File::WriteStringToFile(IntToString(pid), FLAGS_pidfile);
  boost::shared_ptr<rafdb::RafDb> handler(new rafdb::RafDb());
  boost::shared_ptr<TProcessor> processor(
      new rafdb::RafdbServiceProcessor(handler));
//  boost::shared_ptr<TServerTransport> serverTransport(
//      new TServerSocket(FLAGS_leveldbd_port));
//  boost::shared_ptr<TTransportFactory> transportFactory(
//      new TFramedTransportFactory());
//  boost::shared_ptr<TProtocolFactory> protocolFactory(
//      new TBinaryProtocolFactory());
//  TThreadedServer server(processor, serverTransport,
//      transportFactory, protocolFactory);
//  LOG(INFO) << "start listening on port:" << FLAGS_leveldbd_port;
//  server.serve();
//
  std::string ip;
  int port;
  int id;
  rafdb::GetIpPortId(ip,port,id,FLAGS_rafdb_self);
  base::ThriftNonBlockingServerMutiThread<rafdb::RafDb,
    rafdb::RafdbServiceProcessor>
    server(true, port, handler.get(), FLAGS_thread_num);
  LOG(INFO) << "start listening on port:" << port;
  server.Start();
  server.Join();

  
  return 0;
}
