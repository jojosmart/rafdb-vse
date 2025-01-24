// Author:Zhouxiaoliang

#include "base/yr.h"
#include "base/logging.h"
#include "base/bloom_filter.h"

using base::bloom::Bloom;
using base::bloom::FNV;
using base::bloom::FNVHash;

typedef uint64 BType;

namespace {

struct UserId {
  unsigned long high;
  unsigned long low;
};

}

int main() {
 Bloom<UserId, BType> b(1000000, 0.1, FNV<BType>::INIT, &FNVHash);
 uint8 bloom_storage[b.getBytesNumber()];
 memset(bloom_storage, 0, b.getBytesNumber());

 b.setBitStorage(bloom_storage, b.getBytesNumber());

 UserId uid;
 for (int i = 1; i < 1001; ++i) {
   uid.high = i % 100;
   uid.low = i % 200;
   bool f_fill = b.fillBitSet(&uid);
   CHECK(b.isElementPresent(&uid));
   if (!f_fill)
     break;
 }

 Bloom<UserId, BType> b1(1000000, 0.1, FNV<BType>::INIT, &FNVHash);
 b1.setBitStorage(bloom_storage, b.getBytesNumber());

 for (int i = 1; i < 1001; ++i) {
   uid.high = i % 100;
   uid.low = i % 200;
   CHECK(b.isElementPresent(&uid));
 }

 return 0;
}
