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

#include "inet/linklayer/ieee80211/mac/fragmentation/Fragmentation.h"
#include "inet/linklayer/ieee80211/mac/sequencenumberassignment/NonQoSSequenceNumberAssignment.h"
#include "OriginatorMacDataService.h"

namespace inet {
namespace ieee80211 {

Define_Module(OriginatorMacDataService);

void OriginatorMacDataService::initialize()
{
    sequenceNumberAssigment = new NonQoSSequenceNumberAssignment();
    fragmentationPolicy = check_and_cast<IFragmentationPolicy*>(getSubmodule("fragmentationPolicy"));
    fragmentation = new Fragmentation();
}

void OriginatorMacDataService::assignSequenceNumber(const Ptr<Ieee80211DataOrMgmtFrame>& frame)
{
    sequenceNumberAssigment->assignSequenceNumber(frame);
}

OriginatorMacDataService::Fragments *OriginatorMacDataService::fragmentIfNeeded(Packet *frame)
{
    auto fragmentSizes = fragmentationPolicy->computeFragmentSizes(frame);
    if (fragmentSizes.size() != 0) {
        auto fragmentFrames = fragmentation->fragmentFrame(frame, fragmentSizes);
        return fragmentFrames;
    }
    return nullptr;
}

OriginatorMacDataService::Fragments* OriginatorMacDataService::extractFramesToTransmit(PendingQueue *pendingQueue)
{
    if (pendingQueue->isEmpty())
        return nullptr;
    else {
        // if (msduRateLimiting)
        //    txRateLimitingIfNeeded();
        Packet *packet = pendingQueue->pop();
        if (sequenceNumberAssigment) {
            auto frame = packet->removeHeader<Ieee80211DataOrMgmtFrame>();
            assignSequenceNumber(frame);
            packet->insertHeader(frame);
        }
        // if (msduIntegrityAndProtection)
        //    frame = protectMsduIfNeeded(frame);
        Fragments *fragments = nullptr;
        if (fragmentationPolicy)
            fragments = fragmentIfNeeded(packet);
        if (!fragments)
            fragments = new Fragments({packet});
        // if (mpduEncryptionAndIntegrity)
        //    fragments = encryptMpduIfNeeded(fragments);
        // if (mpduHeaderPlusCrc)
        //    fragments = mpduCrcFooBarIfNeeded(fragments);
        return fragments;
    }
}

OriginatorMacDataService::~OriginatorMacDataService()
{
    delete sequenceNumberAssigment;
    delete fragmentation;
}


} /* namespace ieee80211 */
} /* namespace inet */
