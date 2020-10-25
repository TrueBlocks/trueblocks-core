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
#include "tracefilter.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTraceFilter, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextTracefilterChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextTracefilterChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CTraceFilter::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["tracefilter_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextTracefilterChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTracefilterChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTraceFilter*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CTraceFilter::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextTracefilterChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "after") {
                return uint_2_Str(after);
            }
            break;
        case 'c':
            if (fieldName % "count") {
                return uint_2_Str(count);
            }
            break;
        case 'f':
            if (fieldName % "fromBlock") {
                return uint_2_Str(fromBlock);
            }
            if (fieldName % "fromAddress" || fieldName % "fromAddressCnt") {
                size_t cnt = fromAddress.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += ("\"" + fromAddress[i] + "\"");
                    retS += ((i < cnt - 1) ? ",\n" + indent() : "\n");
                }
                return retS;
            }
            break;
        case 't':
            if (fieldName % "toBlock") {
                return uint_2_Str(toBlock);
            }
            if (fieldName % "toAddress" || fieldName % "toAddressCnt") {
                size_t cnt = toAddress.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += ("\"" + toAddress[i] + "\"");
                    retS += ((i < cnt - 1) ? ",\n" + indent() : "\n");
                }
                return retS;
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
bool CTraceFilter::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "after") {
                after = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'c':
            if (fieldName % "count") {
                count = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'f':
            if (fieldName % "fromBlock") {
                fromBlock = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "fromAddress") {
                string_q str = fieldValue;
                while (!str.empty()) {
                    fromAddress.push_back(str_2_Addr(nextTokenClear(str, ',')));
                }
                return true;
            }
            break;
        case 't':
            if (fieldName % "toBlock") {
                toBlock = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "toAddress") {
                string_q str = fieldValue;
                while (!str.empty()) {
                    toAddress.push_back(str_2_Addr(nextTokenClear(str, ',')));
                }
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTraceFilter::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTraceFilter::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> fromBlock;
    archive >> toBlock;
    archive >> fromAddress;
    archive >> toAddress;
    archive >> after;
    archive >> count;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTraceFilter::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << fromBlock;
    archive << toBlock;
    archive << fromAddress;
    archive << toAddress;
    archive << after;
    archive << count;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTraceFilterArray& array) {
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
CArchive& operator<<(CArchive& archive, const CTraceFilterArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTraceFilter::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CTraceFilter, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CTraceFilter, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTraceFilter, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CTraceFilter, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CTraceFilter, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CTraceFilter, "fromBlock", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CTraceFilter, "toBlock", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CTraceFilter, "fromAddress", T_ADDRESS | TS_ARRAY, ++fieldNum);
    ADD_FIELD(CTraceFilter, "toAddress", T_ADDRESS | TS_ARRAY, ++fieldNum);
    ADD_FIELD(CTraceFilter, "after", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CTraceFilter, "count", T_UNUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTraceFilter, "schema");
    HIDE_FIELD(CTraceFilter, "deleted");
    HIDE_FIELD(CTraceFilter, "showing");
    HIDE_FIELD(CTraceFilter, "cname");

    builtIns.push_back(_biCTraceFilter);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTracefilterChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CTraceFilter* tra = reinterpret_cast<const CTraceFilter*>(dataPtr);
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, tra);
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
bool CTraceFilter::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTraceFilter& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const string_q CTraceFilter::getStringAt(const string_q& fieldName, size_t i) const {
    if (fieldName % "fromAddress" && i < fromAddress.size())
        return (fromAddress[i]);
    if (fieldName % "toAddress" && i < toAddress.size())
        return (toAddress[i]);
    return "";
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_TRACEFILTER = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
