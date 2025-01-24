#pragma once

#include <stdlib.h>
#include <stdint.h>


#ifdef __GNUC__
#define VSE_API __attribute__ ((visibility ("default")))
#else
#ifdef VSE_EXPORTS
#define VSE_API __declspec(dllexport)
#else
#define VSE_API __declspec(dllimport)
#endif
#endif

#ifdef __cplusplus
extern "C"{
#endif

typedef struct VseBuffer{
    char* data;
    size_t size;
}VseBuffer;

typedef struct ErrInfo{
    int code;
	char errmsg[256 - 4];
}ErrInfo;

 //sdk网络错误
#define VSE_ERROR_NETWORK   -12    

VSE_API void vsebuffer_free(VseBuffer* pbuf);


#ifdef __cplusplus
}
#endif