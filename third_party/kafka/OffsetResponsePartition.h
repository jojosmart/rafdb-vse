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

#ifndef OFFSETRESPONSEPARTITION_H
#define OFFSETRESPONSEPARTITION_H

#include <string>
#include "third_party/kafka/Debug.h"
#include "third_party/kafka/Packet.h"
#include "third_party/kafka/MessageSet.h"
#include "third_party/kafka/WireFormatter.h"
#include "third_party/kafka/PacketWriter.h"
#include "third_party/kafka/ErrorHandler.h"

namespace LibKafka {

class OffsetResponsePartition : public WireFormatter, public PacketWriter, public ErrorHandler
{
  public:

    int partition;
    short int errorCode;
    int offsetArraySize;
    long int *offsetArray;

    OffsetResponsePartition(Packet *packet);
    OffsetResponsePartition(int partition, short int errorCode, int offsetArraySize, long int *offsetArray, bool releaseArrays = false);
    ~OffsetResponsePartition();

    unsigned char* toWireFormat(bool updatePacketSize = true);
    int getWireFormatSize(bool includePacketSize = false);

    bool hasErrorCode();

  private:

    bool releaseArrays;
};

std::ostream& operator<< (std::ostream& os, const OffsetResponsePartition& frm);
inline bool operator==(const OffsetResponsePartition& lhs, const OffsetResponsePartition& rhs) { return ((lhs.partition==rhs.partition)&&(lhs.errorCode==rhs.errorCode)&&(lhs.offsetArraySize==rhs.offsetArraySize)); }
inline bool operator!=(const OffsetResponsePartition& lhs, const OffsetResponsePartition& rhs) { return !operator==(lhs,rhs); }

}; // namespace LibKafka

#endif /* OFFSETRESPONSEPARTITION_H */
