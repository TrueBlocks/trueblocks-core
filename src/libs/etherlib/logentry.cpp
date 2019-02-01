/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
#include "logentry.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CLogEntry, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextLogentryChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextLogentryChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CLogEntry::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    string_q fmt = fmtIn;
    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextLogentryChunk, this);
}

//---------------------------------------------------------------------------
string_q nextLogentryChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CLogEntry *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CLogEntry::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    if (pReceipt)
        if (((CReceipt*)pReceipt)->setValueByName(fieldName, fieldValue))  // NOLINT
            return true;
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) { address = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "articulatedLog" ) { /* articulatedLog = fieldValue; */ return false; }
            break;
        case 'd':
            if ( fieldName % "data" ) { data = fieldValue; return true; }
            break;
        case 'l':
            if ( fieldName % "logIndex" ) { logIndex = str_2_Uint(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "topics" ) {
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
void CLogEntry::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CLogEntry::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> address;
    archive >> data;
    archive >> logIndex;
    archive >> topics;
//    archive >> articulatedLog;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CLogEntry::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << address;
    archive << data;
    archive << logIndex;
    archive << topics;
//    archive << articulatedLog;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CLogEntryArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0 ; i < count ; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CLogEntryArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CLogEntry::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CLogEntry, "schema")) return;

    size_t fieldNum = 1000;
    ADD_FIELD(CLogEntry, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CLogEntry, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CLogEntry, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CLogEntry, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CLogEntry, "address", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CLogEntry, "data", T_TEXT, ++fieldNum);
    ADD_FIELD(CLogEntry, "logIndex", T_NUMBER, ++fieldNum);
    ADD_FIELD(CLogEntry, "topics", T_OBJECT|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CLogEntry, "articulatedLog", T_OBJECT, ++fieldNum);
    HIDE_FIELD(CLogEntry, "articulatedLog");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CLogEntry, "schema");
    HIDE_FIELD(CLogEntry, "deleted");
    HIDE_FIELD(CLogEntry, "showing");
    HIDE_FIELD(CLogEntry, "cname");

    builtIns.push_back(_biCLogEntry);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextLogentryChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CLogEntry *log = reinterpret_cast<const CLogEntry *>(dataPtr);
    if (log) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
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
bool CLogEntry::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CLogEntry& log) {
    log.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CLogEntry& log) {
    log.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
string_q CLogEntry::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextLogentryChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) return addr_2_Str(address);
            if ( fieldName % "articulatedLog" ) { expContext().noFrst=true; return articulatedLog.Format(); }
            break;
        case 'd':
            if ( fieldName % "data" ) return data;
            break;
        case 'l':
            if ( fieldName % "logIndex" ) return uint_2_Str(logIndex);
            break;
        case 't':
            if ( fieldName % "topics" || fieldName % "topicsCnt" ) {
                size_t cnt = topics.size();
                if (endsWith(fieldName, "Cnt"))
                    return uint_2_Str(cnt);
                if (!cnt) return "";
                string_q retS;
                for (size_t i = 0 ; i < cnt ; i++) {
                    retS += ("\"" + topic_2_Str(topics[i]) + "\"");
                    retS += ((i < cnt - 1) ? ",\n" + indent() : "\n");
                }
                return retS;
            }
            break;
    }

    // EXISTING_CODE
    // See if this field belongs to the item's container
    if (fieldName != "cname") {
        ret = nextReceiptChunk(fieldName, pReceipt);
        if (contains(ret, "Field not found"))
            ret = "";
        if (!ret.empty())
            return ret;
    }
    // EXISTING_CODE

    string_q s;
    s = toUpper(string_q("articulatedLog")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = articulatedLog.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CLogEntry& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CLogEntry::getObjectAt(const string_q& fieldName, size_t index) const {
    if ( fieldName % "articulatedLog" )
        return &articulatedLog;
    return NULL;
}

//---------------------------------------------------------------------------
const string_q CLogEntry::getStringAt(const string_q& fieldName, size_t i) const {
    if ( fieldName % "topics" && i < topics.size() )
        return topic_2_Str(topics[i]);
    return "";
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

