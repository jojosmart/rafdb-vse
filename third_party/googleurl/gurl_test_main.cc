// Copyright 2007, Google Inc.
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

#include <string>

#include "testing/gtest/include/gtest/gtest.h"
#include "unicode/putil.h"
#include "unicode/udata.h"

#define ICU_UTIL_DATA_SHARED 1
#define ICU_UTIL_DATA_STATIC 2

#ifndef ICU_UTIL_DATA_IMPL

#define ICU_UTIL_DATA_IMPL ICU_UTIL_DATA_FILE

#endif  // ICU_UTIL_DATA_IMPL

bool InitializeICU() {
#if (ICU_UTIL_DATA_IMPL == ICU_UTIL_DATA_SHARED)
  // We expect to find the ICU data module alongside the current module.
  // Because the module name is ASCII-only, "A" API should be safe.
  HMODULE module = LoadLibraryA(ICU_UTIL_DATA_SHARED_MODULE_NAME);
  if (!module)
    return false;

  FARPROC addr = GetProcAddress(module, ICU_UTIL_DATA_SYMBOL);
  if (!addr)
    return false;

  UErrorCode err = U_ZERO_ERROR;
  udata_setCommonData(reinterpret_cast<void*>(addr), &err);
  return err == U_ZERO_ERROR;
#elif (ICU_UTIL_DATA_IMPL == ICU_UTIL_DATA_STATIC)
  // Mac bundles the ICU data in.
  return true;
#elif (ICU_UTIL_DATA_IMPL == ICU_UTIL_DATA_FILE)
  // We expect to find the ICU data module alongside the current module.
  u_setDataDirectory(".");
  // Only look for the packaged data file;
  // the default behavior is to look for individual files.
  UErrorCode err = U_ZERO_ERROR;
  udata_setFileAccess(UDATA_ONLY_PACKAGES, &err);
  return err == U_ZERO_ERROR;
#endif
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  InitializeICU();

  return RUN_ALL_TESTS();
}
