//
// Copyright (C) 2016 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see http://www.gnu.org/licenses/.
//

#include "inet/linklayer/ieee80211/mac/duplicateremoval/LegacyDuplicateRemoval.h"
#include "RecipientMacDataService.h"

namespace inet {
namespace ieee80211 {

Define_Module(RecipientMacDataService);

void RecipientMacDataService::initialize()
{
    duplicateRemoval = new LegacyDuplicateRemoval();
    basicReassembly = new BasicReassembly();
}

Packet *RecipientMacDataService::defragment(Packet *dataOrMgmtFrame)
{
    Packet *result = basicReassembly->addFragment(dataOrMgmtFrame);
    if (auto completeFrame = std::dynamic_pointer_cast<Ieee80211DataOrMgmtFrame>(result->peekHeader<Ieee80211Frame>()))
        return result;
    else
        return nullptr;
}

std::vector<Packet*> RecipientMacDataService::dataOrMgmtFrameReceived(Packet *packet, const Ptr<Ieee80211DataOrMgmtFrame>& frame)
{
    if (duplicateRemoval && duplicateRemoval->isDuplicate(frame))
        return std::vector<Packet*>();
    Packet *defragmentedFrame = nullptr;
    if (basicReassembly) { // FIXME: defragmentation
        defragmentedFrame = defragment(packet);
    }
    return defragmentedFrame != nullptr ? std::vector<Packet*>({defragmentedFrame}) : std::vector<Packet*>();
}

std::vector<Packet*> RecipientMacDataService::dataFrameReceived(Packet *dataPacket, const Ptr<Ieee80211DataFrame>& dataFrame)
{
    return dataOrMgmtFrameReceived(dataPacket, dataFrame);
}

std::vector<Packet*> RecipientMacDataService::managementFrameReceived(Packet *mgmtPacket, const Ptr<Ieee80211ManagementFrame>& mgmtFrame)
{
    return dataOrMgmtFrameReceived(mgmtPacket, mgmtFrame);
}

std::vector<Packet*> RecipientMacDataService::controlFrameReceived(Packet *controlPacket, const Ptr<Ieee80211Frame>& controlFrame)
{
    return std::vector<Packet*>(); // has nothing to do
}

RecipientMacDataService::~RecipientMacDataService()
{
    delete duplicateRemoval;
    delete basicReassembly;
}

} /* namespace ieee80211 */
} /* namespace inet */
