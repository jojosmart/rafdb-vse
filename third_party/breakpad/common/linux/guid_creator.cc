// Copyright (c) 2006, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "third_party/breakpad/common/linux/guid_creator.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

//
// GUIDGenerator
//
// This class is used to generate random GUID.
// Currently use random number to generate a GUID since Linux has
// no native GUID generator. This should be OK since we don't expect
// crash to happen very offen.
//
class GUIDGenerator {
 public:
  GUIDGenerator() {
    srandom(time(NULL));
  }

  static u_int32_t BytesToUInt32(const u_int8_t bytes[]) {
    return ((u_int32_t) bytes[0]
            | ((u_int32_t) bytes[1] << 8)
            | ((u_int32_t) bytes[2] << 16)
            | ((u_int32_t) bytes[3] << 24));
  }

  static void UInt32ToBytes(u_int8_t bytes[], u_int32_t n) {
    bytes[0] = n & 0xff;
    bytes[1] = (n >> 8) & 0xff;
    bytes[2] = (n >> 16) & 0xff;
    bytes[3] = (n >> 24) & 0xff;
  }

  bool CreateGUID(GUID *guid) const {
    guid->data1 = random();
    guid->data2 = (u_int16_t)(random());
    guid->data3 = (u_int16_t)(random());
    UInt32ToBytes(&guid->data4[0], random());
    UInt32ToBytes(&guid->data4[4], random());
    return true;
  }
};

// Guid generator.
const GUIDGenerator kGuidGenerator;

bool CreateGUID(GUID *guid) {
  return kGuidGenerator.CreateGUID(guid);
}

// Parse guid to string.
bool GUIDToString(const GUID *guid, char *buf, int buf_len) {
  // Should allow more space the the max length of GUID.
  assert(buf_len > kGUIDStringLength);
  int num = snprintf(buf, buf_len, kGUIDFormatString,
                     guid->data1, guid->data2, guid->data3,
                     GUIDGenerator::BytesToUInt32(&(guid->data4[0])),
                     GUIDGenerator::BytesToUInt32(&(guid->data4[4])));
  if (num != kGUIDStringLength)
    return false;

  buf[num] = '\0';
  return true;
}
