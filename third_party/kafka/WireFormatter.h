///////////////////////////////////////////////////////////////////////////
//
// libkafka - C/C++ client for Apache Kafka v0.8+
//
// David Tompkins -- 8/8/2013
// tompkins@adobe_dot_com
//
///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2013 Adobe Systems Incorporated. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
///////////////////////////////////////////////////////////////////////////

#ifndef WIRE_FORMATTER_H
#define WIRE_FORMATTER_H

#include "third_party/kafka/Debug.h"

namespace LibKafka {

class WireFormatter
{
  public:

    virtual unsigned char* toWireFormat(bool updatePacketSize = true) = 0;
    virtual int getWireFormatSize(bool includePacketSize = true) = 0;
};

}; // namespace LibKafka

#endif /* WIRE_FORMATTER_H */
