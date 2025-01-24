// Author:Zhouxiaoliang
// Date  : 2013-04-03 15:46:36
// File  : test.cc
// Brief :
#include <Python.h>
#include <string>
#include "base/hash.h"
#include "base/logging.h"

class TestFact {
  public:
    TestFact() {}
    ~TestFact() {}
    int fact(int n);
};

int TestFact::fact(int n) {
    if (n <= 1)
        return 1;
    else
        return n * (n - 1);
}
extern "C"
int fact(int n) {
    TestFact t;
    return t.fact(n);
}


// 64-bit hash for 64-bit platforms
uint64 MurmurHash64A(const void* key, int len, uint32 seed) {
  const uint64 m = 0xc6a4a7935bd1e995;
  const int r = 47;

  uint64 h = seed ^ (len * m);

  const uint64* data = (const uint64 *)key;
  const uint64* end = data + (len/8);

  while (data != end) {
    uint64 k = *data++;

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;
  }

  const uint8* data2 = (const uint8*)data;

  switch (len & 7) {
    case 7: h ^= static_cast<uint64>(data2[6]) << 48;
    case 6: h ^= static_cast<uint64>(data2[5]) << 40;
    case 5: h ^= static_cast<uint64>(data2[4]) << 32;
    case 4: h ^= static_cast<uint64>(data2[3]) << 24;
    case 3: h ^= static_cast<uint64>(data2[2]) << 16;
    case 2: h ^= static_cast<uint64>(data2[1]) << 8;
    case 1: h ^= static_cast<uint64>(data2[0]);
    h *= m;
  };

  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}

// extern "C"
// const char* Fingerprint(const char* str) {
//   uint64 r = MurmurHash64A(str,strlen(str), kFingerPrintSeed);
//   // return "aaaaa\0";
//   return str;
// }

// extern "C"
// uint64 Fingerprint(const char* str) {
//  return r = MurmurHash64A(str,strlen(str), kFingerPrintSeed);
// }

extern "C"
uint64 Fingerprint(const char* str) {
    return base::Fingerprint(str, strlen(str));
}



//// 1 c/cpp中的函数
// int my_c_function(const char *arg) {
//  int n = system(arg);
//  return n;
// }
//// 2 python 包装
// static PyObject * wrap_my_c_fun(PyObject *self, PyObject *args) {
//  const char * command;
//  int n;
// // 这句是把python的变量args转换成c的变量command
//  if (!PyArg_ParseTuple(args, "s", &command))
//    return NULL;
//  n = my_c_function(command);//调用c的函数
//  return Py_BuildValue("i", n);//把c的返回值n转换成python的对象
// }
//// 3 方法列表
// static PyMethodDef MyCppMethods[] = {
//    //MyCppFun1是python中注册的函数名，wrap_my_c_fun是函数指针
//    { "MyCppFun1", wrap_my_c_fun, METH_VARARGS, "Execute a shell command." },
//    { NULL, NULL, 0, NULL }
// };
//// 4 模块初始化方法
// PyMODINIT_FUNC initMyCppModule(void) {
//  //初始模块，把MyCppMethods初始到MyCppModule中
//  PyObject *m = Py_InitModule("MyCppModule", MyCppMethods);
//  if (m == NULL)
//    return;
// }

int main(int argc, char **argv) {
    char* str = "abcd\0";
    LOG(INFO) << Fingerprint(str);
    // subfood_test();
    // minshengdoc_handler_test();
    // split_test();
    // string_test();

    return 0;
}
