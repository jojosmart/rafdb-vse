// Copyright 2014 iminer. All Rights Reserved.
// Author:Zhouxiaoliang
#ifndef COMMON_MD5_H__
#define COMMON_MD5_H__

#include <stdint.h>

typedef uint32_t u32;
typedef uint8_t u8;

struct MD5Context {
  u32 buf[4];
  u32 bits[2];
  u8 in[64];
};

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

void MD5Init(struct MD5Context *ctx);

void MD5Update(struct MD5Context *ctx, unsigned char const *buf, unsigned len);

void MD5Final(unsigned char digest[16], struct MD5Context *ctx);

#ifdef __cplusplus
}
#endif

#endif  // COMMON_MD5_H__
