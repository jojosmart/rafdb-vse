
#ifndef RAFDB_MANAGER_H_
#define RAFDB_MANAGER_H_

#include <string>
#include <vector>

#include "base/hash_tables.h"
#include "base/mutex.h"
#include "base/thread.h"
#include "base/scoped_ptr.h"
#include "rafdb.h"
#include "base/thrift.h"
#include "base/logging.h"
#include "proto/gen-cpp/RafdbService.h"
//#include "ts/base/crawl_config.h"
#include "sync.h"
#include "global.h"


namespace rafdb {

class RafDb;
class Sync;
class Manager :public base::Thread  {
  public:
    explicit Manager(RafDb* rafdb_p)
                         :rafdb_(rafdb_p)
    {
      Init(rafdb_p);
    }
    void Init(RafDb* rafdb_p);
    virtual ~Manager() 
    {
    }
  protected:
    virtual void Run();

  private:
    RafDb* rafdb_;
    Sync *sync_;
    DISALLOW_COPY_AND_ASSIGN(Manager);
};
}





#endif
