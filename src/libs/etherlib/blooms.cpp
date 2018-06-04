/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "blooms.h"

namespace qblocks {

    //-------------------------------------------------------------------------
    SFString formatBloom(const SFBloom& b1, bool bits) {
        SFString ret;
        if (bits) {
            ret = "\tbits: " + asStringU(bitsTwiddled(b1));
            SFString s1 = bloom2Bits(b1).Substitute("0", ".");
            for (uint32_t i=0;i<16;i++) {
                SFString m1;
                for (uint32_t j=0;j<128;j=j+10) {
                    m1 += s1.substr(i*128, 128).substr(j,10) + " ";
                }
                ret += ("\n\t" + m1);
            }
        } else {
            ret = "\tbits: " + asStringU(bitsTwiddled(b1));
            SFString s1 = bloom2Bytes(b1).Substitute("0x", "").Substitute("0", ".");
            for (uint32_t i=0;i<4;i++) {
                SFString m1;
                for (uint32_t j=0;j<128;j=j+10) {
                    m1 += s1.substr(i*128, 128).substr(j,10) + " ";
                }
                ret += ("\n\t" + m1);
            }
        }
        return ret;
    }

    //-------------------------------------------------------------------------
    bool compareBlooms(const SFBloom& b1, const SFBloom& b2, SFString& str) {
        if (verbose > 2) {
            str = "\n\tbits1: " + asStringU(bitsTwiddled(b1)) + " bits2: " + asStringU(bitsTwiddled(b2));
            SFString s1 = bloom2Bits(b1).Substitute("0", ".");
            SFString s2 = bloom2Bits(b2).Substitute("0", ".");
            for (uint32_t i=0;i<16;i++) {
                SFString m1, m2;
                for (uint32_t j=0;j<128;j=j+10) {
                    m1 += s1.substr(i*128, 128).substr(j,10) + " ";
                    m2 += s2.substr(i*128, 128).substr(j,10) + " ";
                }
                str += ("\n\t" + cRed + m1 + cOff + "\n\t" + m2);
            }
        } else if (verbose > 1) {
            str = "\n\tbits: " + asStringU(bitsTwiddled(b1)) + " " + asStringU(bitsTwiddled(b2));
            SFString s1 = bloom2Bytes(b1).Substitute("0x", "").Substitute("0", ".");
            SFString s2 = bloom2Bytes(b2).Substitute("0x", "").Substitute("0", ".");
            for (uint32_t i=0;i<4;i++) {
                SFString m1, m2;
                for (uint32_t j=0;j<128;j=j+10) {
                    m1 += s1.substr(i*128, 128).substr(j,10) + " ";
                    m2 += s2.substr(i*128, 128).substr(j,10) + " ";
                }
                str += ("\n\t" + cRed + m1 + cOff + "\n\t" + m2 + "\n");
            }
        }
        return isBloomHit(b1, b2);
    }

    //----------------------------------------------------------------------------------
    bool addAddrToBloom(const SFAddress& addr, SFBloomArray& blooms, uint32_t maxBits) {
        // Initialize if not already
        if (blooms.getCount() == 0)
            blooms[0] = 0;
        uint32_t cnt = blooms.getCount();
        blooms[cnt - 1] = joinBloom(blooms[cnt - 1], makeBloom(addr));
        if (bitsTwiddled(blooms[cnt - 1]) > maxBits) {
            blooms[cnt] = 0; // start a new bloom
            return true;
        }
        return false;
    }

    //-----------------------------------------------------------------------
    SFString reportBloom(const SFBloomArray& blooms) {
        SFString ret;
        for (uint32_t i = 0; i < blooms.getCount(); i++) {
            uint64_t bits = bitsTwiddled(blooms[i]);
            if (bits) {
                ret += asStringU(bits);
                if (i < blooms.getCount()-1)
                    ret += ",";
            }
        }
        if (ret.empty())
            return ret;
        return cYellow + ret + cOff;
    }

}  // namespace qblocks

