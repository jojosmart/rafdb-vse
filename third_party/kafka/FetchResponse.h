//////////////////////////////////////////////////////////////////
//                                                              //
// libkafka - C/C++ client for Apache Kafka v0.8+               //
//                                                              //
// David Tompkins -- 8/8/2013                                   //
// http://dt.org/                                               //
//                                                              //
// Copyright (c) 2013 by David Tompkins.                        //
//                                                              //
//////////////////////////////////////////////////////////////////
//                                                              //
// This program is free software; you can redistribute it       //
// and/or modify it under the terms of the GNU General Public   //
// License as published by the Free Software Foundation.        //
//                                                              //
// This program is distributed in the hope that it will be      //
// useful, but WITHOUT ANY WARRANTY; without even the implied   //
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      //
// PURPOSE. See the GNU General Public License for more details //
//                                                              //
// You should have received a copy of the GNU General Public    //
// License along with this program; if not, write to the Free   //
// Software Foundation, Inc., 59 Temple Place, Suite 330,       //
// Boston, MA 02111-1307 USA                                    //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef FETCH_RESPONSE_H
#define FETCH_RESPONSE_H

#include <string>
#include "third_party/kafka/Response.h"
#include "third_party/kafka/TopicNameBlock.h"
#include "third_party/kafka/FetchResponsePartition.h"

namespace LibKafka {

class FetchResponse : public Response
{
  public:

    int fetchResponseTopicArraySize;
    TopicNameBlock<FetchResponsePartition> **fetchResponseTopicArray;

    FetchResponse(unsigned char *buffer, bool releaseBuffer = false);
    FetchResponse(int correlationId, int fetchResponseTopicArraySize, TopicNameBlock<FetchResponsePartition> **fetchResponseTopicArray, bool releaseArrays = false);
    ~FetchResponse();

    unsigned char* toWireFormat(bool updatePacketSize = true);
    int getWireFormatSize(bool includePacketSize = true);

    bool hasErrorCode();

  private:

    bool releaseArrays;
};

std::ostream& operator<< (std::ostream& os, const FetchResponse& fr);

}; // namespace LibKafka

#endif /* FETCH_RESPONSE_H */
