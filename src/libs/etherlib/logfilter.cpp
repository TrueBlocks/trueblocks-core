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
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "logfilter.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CLogFilter, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextLogfilterChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextLogfilterChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CLogFilter::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["logfilter_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextLogfilterChunk, this);
}

//---------------------------------------------------------------------------
string_q nextLogfilterChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CLogFilter*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CLogFilter::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextLogfilterChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "blockHash") {
                return hash_2_Str(blockHash);
            }
            break;
        case 'e':
            if (fieldName % "emitters" || fieldName % "emittersCnt") {
                size_t cnt = emitters.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += ("\"" + emitters[i] + "\"");
                    retS += ((i < cnt - 1) ? ",\n" + indentStr() : "\n");
                }
                return retS;
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
bool CLogFilter::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "blockHash") {
                blockHash = str_2_Hash(fieldValue);
                return true;
            }
            break;
        case 'e':
            if (fieldName % "emitters") {
                string_q str = fieldValue;
                while (!str.empty()) {
                    emitters.push_back(str_2_Addr(nextTokenClear(str, ',')));
                }
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
void CLogFilter::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CLogFilter::Serialize(CArchive& archive) {
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
    archive >> emitters;
    archive >> topics;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CLogFilter::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << fromBlock;
    archive << toBlock;
    archive << blockHash;
    archive << emitters;
    archive << topics;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CLogFilter::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CLogFilter copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CLogFilterArray& array) {
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
CArchive& operator<<(CArchive& archive, const CLogFilterArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CLogFilter::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CLogFilter, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CLogFilter, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CLogFilter, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CLogFilter, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CLogFilter, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CLogFilter, "fromBlock", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CLogFilter, "toBlock", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CLogFilter, "blockHash", T_HASH | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CLogFilter, "emitters", T_ADDRESS | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CLogFilter, "topics", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CLogFilter, "schema");
    HIDE_FIELD(CLogFilter, "deleted");
    HIDE_FIELD(CLogFilter, "showing");
    HIDE_FIELD(CLogFilter, "cname");

    builtIns.push_back(_biCLogFilter);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextLogfilterChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CLogFilter* log = reinterpret_cast<const CLogFilter*>(dataPtr);
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

// EXISTING_CODE
// EXISTING_CODE

//---------------------------------------------------------------------------
bool CLogFilter::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CLogFilter& log) {
    log.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CLogFilter& log) {
    log.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CLogFilter& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const string_q CLogFilter::getStringAt(const string_q& fieldName, size_t i) const {
    if (fieldName % "emitters" && i < emitters.size())
        return (emitters[i]);
    if (fieldName % "topics" && i < topics.size())
        return topic_2_Str(topics[i]);
    return "";
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_LOGFILTER =
    "[{FROMBLOCK}]\t"
    "[{TOBLOCK}]\t"
    "[{BLOCKHASH}]\t"
    "[{EMITTERS}]\t"
    "[{TOPICS}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
bool CLogFilter::wasEmittedBy(const address_t& test) const {
    for (auto e : emitters)
        if (e == test)
            return true;
    return false;
}

//---------------------------------------------------------------------------
bool CLogFilter::passes(const CLogEntry& log) {
    bool filteringEmitters = emitters.size() > 0;
    bool filteringTopics = topics.size() > 0;

    // If we're not filtering, it passes
    if (!filteringEmitters && !filteringTopics)
        return true;

    ASSERT(log.topics.size() > 0);
    bool emittedBy = wasEmittedBy(log.address);
    bool hasTopic = false;
    for (auto topic : topics) {
        if (topic == log.topics[0])
            hasTopic = true;
    }

    // If we're not filtering emitters or we are an emitter and, at the same
    // time, we are not filtering topics or have the topic, we pass, and
    // if we are both filtering emitters and topics and we have both, we pass
    return (!filteringEmitters || emittedBy) && (!filteringTopics || hasTopic);
}

//---------------------------------------------------------------------------
bool CLogFilter::isFastPath(void) const {
    if (!blockHash.empty())
        return false;
    if (emitters.empty() && topics.empty())
        return false;
    return true;
}

//---------------------------------------------------------------------------
string_q CLogFilter::toRPC(void) const {
    ostringstream out;

    if (!blockHash.empty())
        out << "\"blockHash\": \"" << blockHash << "\"";

    if (fromBlock != 0) {
        out << (out.str().empty() ? "" : ",");
        out << "\"fromBlock\":\"" << uint_2_Hex(fromBlock) << "\"";
    }

    if (toBlock != 0) {
        out << (out.str().empty() ? "" : ",");
        out << "\"toBlock\":\"" << uint_2_Hex(toBlock) << "\"";
    }

    if (emitters.size()) {
        out << (out.str().empty() ? "" : ",");
        out << "\"address\":[";
        size_t cnt = 0;
        for (auto addr : emitters) {
            if (cnt)
                out << ",";
            out << "\"" << addr << "\"";
            cnt++;
        }
        out << "]";
    }

    if (topics.size()) {
        out << (out.str().empty() ? "" : ",");
        out << "\"topics\":[";
        size_t cnt = 0;
        for (auto topic : topics) {
            if (cnt)
                out << ",";
            out << "\"" << topic << "\"";
            cnt++;
        }
        out << "]";
    }

    string_q ret = "[{" + out.str() + "}]";
    LOG4("Querying with ", ret);
    return ret;

    // A valid query for two addresses
    //
    // curl -X POST --data '{
    //    "id":8,
    //    "jsonrpc":"2.0",
    //    "method":"eth_getLogs",
    //    "params":[{
    //        "fromBlock":"0x3d37e0",
    //        "toBlock":"0x3d37e1",
    //        "address":["0x209c4784ab1e8183cf58ca33cb740efbf3fc18ef","0x8d12a197cb00d4747a1fe03395095ce2a5cc6819"]
    //        "topics":
    //        ["0x6effdda786735d5033bfad5f53e5131abcced9e52be6c507b62d639685fbed6d","0x6effdda786735d5033bfad5f53e5131abcced9e52be6c507b62d639685fbed6d"]
    //     }]
    //   }' -H "Content-Type: application/json" http://localhost:23456 | jq
    //
    //
    // fromBlock:   QUANTITY|TAG - (optional, default: "latest") Integer block number, or "latest" for the last mined
    // block or "pending", "earliest" for not yet mined transactions. toBlock:     QUANTITY|TAG - (optional, default:
    // "latest") Integer block number, or "latest" for the last mined block or "pending", "earliest" for not yet mined
    // transactions. address:     DATA|Array, 20 Bytes - (optional) Contract address or a list of addresses from which
    // logs should originate. topics:      Array of DATA, - (optional) Array of 32 Bytes DATA topics. Topics are
    // order-dependent. Each topic can also be an array of DATA with “or” options. blockhash:   DATA, 32 Bytes -
    // (optional, future) With the addition of EIP-234, blockHash will be a new filter option which restricts the logs
    // returned to the single block with the 32-byte hash blockHash. Using blockHash is equivalent to
    // fromBlock = toBlock = the block number with hash blockHash. If blockHash is present in in the
    // filter criteria, then neither fromBlock nor toBlock are allowed.
}
// EXISTING_CODE
}  // namespace qblocks
