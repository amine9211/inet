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

#include "inet/linklayer/ieee80211/mac/fragmentation/Defragmentation.h"
#include "inet/linklayer/ieee80211/mac/Ieee80211Frame_m.h"

namespace inet {
namespace ieee80211 {

Register_Class(Defragmentation);

Packet *Defragmentation::defragmentFrames(std::vector<Packet *> *fragmentFrames)
{
    auto frame = new Packet();
    for (auto fragmentFrame : *fragmentFrames) {
        fragmentFrame->popHeader<Ieee80211DataOrMgmtFrame>();
        frame->append(fragmentFrame->peekData());
    }
    return frame;
}

} // namespace ieee80211
} // namespace inet

