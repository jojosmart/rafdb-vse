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

#ifndef PRODUCEMESSAGESET_H
#define PRODUCEMESSAGESET_H

#include <string>
#include "third_party/kafka/Debug.h"
#include "third_party/kafka/Packet.h"
#include "third_party/kafka/MessageSet.h"
#include "third_party/kafka/WireFormatter.h"
#include "third_party/kafka/PacketWriter.h"
#include "third_party/kafka/ErrorHandler.h"

namespace LibKafka {

class ProduceMessageSet : public WireFormatter, public PacketWriter, public ErrorHandler
{
  public:

    int partition;
    int messageSetSize;
    MessageSet *messageSet;

    ProduceMessageSet(Packet *packet);
    ProduceMessageSet(int partition, int messageSetSize, MessageSet *messageSet, bool releaseArrays = false);
    ~ProduceMessageSet();

    unsigned char* toWireFormat(bool updatePacketSize = true);
    int getWireFormatSize(bool includePacketSize = false);

    void setCompression(int codec);

    bool hasErrorCode() { return false; } // no error code in this protocol structure

  private:

    bool hasCompression;
    bool releaseArrays;
};

std::ostream& operator<< (std::ostream& os, const ProduceMessageSet& pm);
inline bool operator==(const ProduceMessageSet& lhs, const ProduceMessageSet& rhs) { return ((lhs.partition==rhs.partition)&&(lhs.messageSetSize==rhs.messageSetSize)); }
inline bool operator!=(const ProduceMessageSet& lhs, const ProduceMessageSet& rhs) { return !operator==(lhs,rhs); }

}; // namespace LibKafka

#endif /* PRODUCEMESSAGESET_H */
