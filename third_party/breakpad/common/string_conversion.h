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

// string_conversion.h: Conversion between different UTF-8/16/32 encodings.

#ifndef COMMON_STRING_CONVERSION_H__
#define COMMON_STRING_CONVERSION_H__

#include <string>
#include <vector>
#include "third_party/breakpad/google_breakpad/common/breakpad_types.h"

namespace google_breakpad {
  
using std::vector;

// Convert |in| to UTF-16 into |out|.  Use platform byte ordering.  If the
// conversion failed, |out| will be zero length.
void UTF8ToUTF16(const char *in, vector<u_int16_t> *out);

// Convert at least one character (up to a maximum of |in_length|) from |in|
// to UTF-16 into |out|.  Return the number of characters consumed from |in|.
// Any unused characters in |out| will be initialized to 0.  No memory will
// be allocated by this routine.
int UTF8ToUTF16Char(const char *in, int in_length, u_int16_t out[2]);

// Convert |in| to UTF-16 into |out|.  Use platform byte ordering.  If the
// conversion failed, |out| will be zero length.
void UTF32ToUTF16(const wchar_t *in, vector<u_int16_t> *out);

// Convert |in| to UTF-16 into |out|.  Any unused characters in |out| will be
// initialized to 0.  No memory will be allocated by this routine.
void UTF32ToUTF16Char(wchar_t in, u_int16_t out[2]);

// Convert |in| to UTF-8.  If |swap| is true, swap bytes before converting.
std::string UTF16ToUTF8(const vector<u_int16_t> &in, bool swap);

}  // namespace google_breakpad

#endif  // COMMON_STRING_CONVERSION_H__
