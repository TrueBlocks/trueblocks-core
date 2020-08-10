/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#include <algorithm>
#include "rewardreconcilation.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CRewardReconcilation, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextRewardreconcilationChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextRewardreconcilationChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CRewardReconcilation::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["rewardreconcilation_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextRewardreconcilationChunk, this);
}

//---------------------------------------------------------------------------
string_q nextRewardreconcilationChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CRewardReconcilation*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CRewardReconcilation::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextRewardreconcilationChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "blockNumber") {
                return uint_2_Str(blockNumber);
            }
            if (fieldName % "baseReward") {
                return wei_2_Str(baseReward);
            }
            break;
        case 't':
            if (fieldName % "timestamp") {
                return ts_2_Str(timestamp);
            }
            break;
        case 'u':
            if (fieldName % "uncAddReward") {
                return wei_2_Str(uncAddReward);
            }
            if (fieldName % "uncleReward") {
                return wei_2_Str(uncleReward);
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CRewardReconcilation::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "blockNumber") {
                blockNumber = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "baseReward") {
                baseReward = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "timestamp") {
                timestamp = str_2_Ts(fieldValue);
                return true;
            }
            break;
        case 'u':
            if (fieldName % "uncAddReward") {
                uncAddReward = str_2_Wei(fieldValue);
                return true;
            }
            if (fieldName % "uncleReward") {
                uncleReward = str_2_Wei(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CRewardReconcilation::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CRewardReconcilation::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> blockNumber;
    archive >> timestamp;
    archive >> baseReward;
    archive >> uncAddReward;
    archive >> uncleReward;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CRewardReconcilation::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockNumber;
    archive << timestamp;
    archive << baseReward;
    archive << uncAddReward;
    archive << uncleReward;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CRewardReconcilationArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0; i < count; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CRewardReconcilationArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CRewardReconcilation::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CRewardReconcilation, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CRewardReconcilation, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CRewardReconcilation, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CRewardReconcilation, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CRewardReconcilation, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CRewardReconcilation, "blockNumber", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CRewardReconcilation, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CRewardReconcilation, "baseReward", T_WEI, ++fieldNum);
    ADD_FIELD(CRewardReconcilation, "uncAddReward", T_WEI, ++fieldNum);
    ADD_FIELD(CRewardReconcilation, "uncleReward", T_WEI, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CRewardReconcilation, "schema");
    HIDE_FIELD(CRewardReconcilation, "deleted");
    HIDE_FIELD(CRewardReconcilation, "showing");
    HIDE_FIELD(CRewardReconcilation, "cname");

    builtIns.push_back(_biCRewardReconcilation);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextRewardreconcilationChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CRewardReconcilation* rew = reinterpret_cast<const CRewardReconcilation*>(dataPtr);
    if (rew) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, rew);
                // EXISTING_CODE
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CRewardReconcilation::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CRewardReconcilation& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_REWARDRECONCILATION =
    "[{BLOCKNUMBER}]\t"
    "[{TIMESTAMP}]\t"
    "[{BASEREWARD}]\t"
    "[{UNCADDREWARD}]\t"
    "[{UNCLEREWARD}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
CRewardReconcilation::CRewardReconcilation(blknum_t bn, const address_t& blockMiner) {
    blockNumber = bn;
    baseReward = getBlockReward(bn, false);
    uncAddReward = getBlockReward(bn, true) - baseReward;
    uint64_t count = getUncleCount(bn);
    for (size_t i = 0 ; i < count ; i++) {
        CBlock uncle;
        getUncle(uncle, bn, i);
        uncleReward += getUncleReward(bn, uncle.blockNumber);
    }
}
// EXISTING_CODE
}  // namespace qblocks
