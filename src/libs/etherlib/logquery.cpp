/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "logquery.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CLogQuery, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextLogqueryChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextLogqueryChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CLogQuery::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["logquery_fmt"] : fmtIn);
    if (fmt.empty()) {
        if (expContext().exportFmt == YAML1) {
            toYaml(ctx);
        } else {
            toJson(ctx);
        }
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextLogqueryChunk, this);
}

//---------------------------------------------------------------------------
string_q nextLogqueryChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CLogQuery*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CLogQuery::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextLogqueryChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "addresses" || fieldName % "addressesCnt") {
                size_t cnt = addresses.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += ("\"" + addresses[i] + "\"");
                    retS += ((i < cnt - 1) ? ",\n" + indentStr() : "\n");
                }
                return retS;
            }
            break;
        case 'b':
            if (fieldName % "blockHash") {
                return hash_2_Str(blockHash);
            }
            break;
        case 'f':
            if (fieldName % "fromBlock") {
                return uint_2_Str(fromBlock);
            }
            break;
        case 't':
            if (fieldName % "toBlock") {
                return uint_2_Str(toBlock);
            }
            if (fieldName % "topics" || fieldName % "topicsCnt") {
                size_t cnt = topics.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += ("\"" + topic_2_Str(topics[i]) + "\"");
                    retS += ((i < cnt - 1) ? ",\n" + indentStr() : "\n");
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
bool CLogQuery::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "addresses") {
                string_q str = fieldValue;
                while (!str.empty()) {
                    addresses.push_back(str_2_Addr(nextTokenClear(str, ',')));
                }
                return true;
            }
            break;
        case 'b':
            if (fieldName % "blockHash") {
                blockHash = str_2_Hash(fieldValue);
                return true;
            }
            break;
        case 'f':
            if (fieldName % "fromBlock") {
                fromBlock = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "toBlock") {
                toBlock = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "topics") {
                string_q str = fieldValue;
                while (!str.empty()) {
                    topics.push_back(str_2_Topic(nextTokenClear(str, ',')));
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
void CLogQuery::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CLogQuery::Serialize(CArchive& archive) {
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
    archive >> blockHash;
    archive >> addresses;
    archive >> topics;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CLogQuery::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << fromBlock;
    archive << toBlock;
    archive << blockHash;
    archive << addresses;
    archive << topics;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CLogQueryArray& array) {
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
CArchive& operator<<(CArchive& archive, const CLogQueryArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CLogQuery::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CLogQuery, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CLogQuery, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CLogQuery, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CLogQuery, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CLogQuery, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CLogQuery, "fromBlock", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CLogQuery, "toBlock", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CLogQuery, "blockHash", T_HASH | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CLogQuery, "addresses", T_ADDRESS | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CLogQuery, "topics", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CLogQuery, "schema");
    HIDE_FIELD(CLogQuery, "deleted");
    HIDE_FIELD(CLogQuery, "showing");
    HIDE_FIELD(CLogQuery, "cname");

    builtIns.push_back(_biCLogQuery);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextLogqueryChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CLogQuery* log = reinterpret_cast<const CLogQuery*>(dataPtr);
    if (log) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, log);
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
bool CLogQuery::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CLogQuery& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const string_q CLogQuery::getStringAt(const string_q& fieldName, size_t i) const {
    if (fieldName % "addresses" && i < addresses.size())
        return (addresses[i]);
    if (fieldName % "topics" && i < topics.size())
        return topic_2_Str(topics[i]);
    return "";
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_LOGQUERY =
    "[{FROMBLOCK}]\t"
    "[{TOBLOCK}]\t"
    "[{BLOCKHASH}]\t"
    "[{ADDRESSES}]\t"
    "[{TOPICS}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
string_q CLogQuery::toRPC(void) const {
    ostringstream fmt;
    if (blockHash.empty()) {
        if (fromBlock)
            fmt << "[\"fromBlock\": \"{FROMBLOCK}\",]";
        if (toBlock)
            fmt << "[\"toBlock\": \"{TOBLOCK}\",]";
    } else {
        fmt << "[\"blockHash\": \"{BLOCKHASH}\",]";
    }
    if (addresses.size()) {
        fmt << "[";
        size_t cnt = 0;
        for (auto addr : addresses) {
            if (cnt)
                fmt << ",";
            fmt << "\"" << addr << "\"";
            cnt++;
        }
        fmt << "],";
    }
    if (topics.size()) {
        fmt << "[";
        size_t cnt = 0;
        for (auto topic : topics) {
            if (cnt)
                fmt << ",";
            fmt << "\"" << topic << "\"";
            cnt++;
        }
        fmt << "],";
    }
    string_q f = "[{" + trim(Format(fmt.str()), ',') + "}]";
    return f;

    //    -//-------------------------------------------------------------------------
    //    -bool queryRawLogs(string_q& results, hash_t hash, const address_t& addr, const CTopicArray& topics) {
    //        -    string_q data = "[{\"blockHash\":\"[HASH]\",\"address\":\"[ADDR]\"}]";
    //        -    replace(data, "[HASH]", hash);
    //        -    if (addr.empty())
    //            -        replace(data, ",\"address\":\"[ADDR]\"", "");
    //        -    else
    //            -        replace(data, "[ADDR]", addr_2_Str(addr));
    //        -    results = callRPC("eth_getLogs", data, true);
    //        -    return true;
    //        -}
    //    -
    //    -//-------------------------------------------------------------------------
    //    -bool queryRawLogs(string_q& results, uint64_t fromBlock, uint64_t toBlock, const address_t& addr,
    //                       -                  const CTopicArray& topics) {
    //        -    string_q data = "[{\"fromBlock\":\"[START]\",\"toBlock\":\"[STOP]\",\"address\":\"[ADDR]\"}]";
    //        -    replace(data, "[START]", uint_2_Hex(fromBlock));
    //        -    replace(data, "[STOP]", uint_2_Hex(toBlock));
    //        -    if (addr.empty())
    //            -        replace(data, ",\"address\":\"[ADDR]\"", "");
    //        -    else
    //            -        replace(data, "[ADDR]", addr_2_Str(addr));
    //        -    results = callRPC("eth_getLogs", data, true);
    //        -    return true;
    //        -}    return "[]";
}
// fromBlock:   QUANTITY|TAG - (optional, default: "latest") Integer block number, or "latest" for the last mined block
// or "pending", "earliest" for not yet mined transactions. toBlock:     QUANTITY|TAG - (optional, default: "latest")
// Integer block number, or "latest" for the last mined block or "pending", "earliest" for not yet mined transactions.
// address:     DATA|Array, 20 Bytes - (optional) Contract address or a list of addresses from which logs should
// originate. topics:      Array of DATA, - (optional) Array of 32 Bytes DATA topics. Topics are order-dependent. Each
// topic can also be an array of DATA with “or” options. blockhash:   DATA, 32 Bytes - (optional, future) With the
// addition of EIP-234, blockHash will be a new filter option which restricts the logs returned to the single
//              block with the 32-byte hash blockHash. Using blockHash is equivalent to fromBlock = toBlock = the block
//              number with hash blockHash. If blockHash is present in in the filter criteria, then neither fromBlock
//              nor toBlock are allowed.
// EXISTING_CODE
}  // namespace qblocks
