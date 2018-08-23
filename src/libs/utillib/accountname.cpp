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
#include "accountname.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAccountName, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextAccountnameChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextAccountnameChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAccountName::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextAccountnameChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAccountnameChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAccountName *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAccountName::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "addr" ) { addr = fieldValue; return true; }
            break;
        case 'd':
            if ( fieldName % "description" ) { description = fieldValue; return true; }
            break;
        case 'n':
            if ( fieldName % "name" ) { name = fieldValue; return true; }
            break;
        case 's':
            if ( fieldName % "symbol" ) { symbol = fieldValue; return true; }
            if ( fieldName % "source" ) { source = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAccountName::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAccountName::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> symbol;
    archive >> name;
    archive >> addr;
    archive >> source;
    archive >> description;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAccountName::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << symbol;
    archive << name;
    archive << addr;
    archive << source;
    archive << description;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAccountNameArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAccountNameArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAccountName::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    size_t fieldNum = 1000;
    ADD_FIELD(CAccountName, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountName, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAccountName, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAccountName, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CAccountName, "symbol", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "addr", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "source", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "description", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAccountName, "schema");
    HIDE_FIELD(CAccountName, "deleted");
    HIDE_FIELD(CAccountName, "showing");
    HIDE_FIELD(CAccountName, "cname");

    builtIns.push_back(_biCAccountName);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAccountnameChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CAccountName *acc = reinterpret_cast<const CAccountName *>(dataPtr);
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, acc);
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
bool CAccountName::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CAccountName::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextAccountnameChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "addr" ) return addr;
            break;
        case 'd':
            if ( fieldName % "description" ) return description;
            break;
        case 'n':
            if ( fieldName % "name" ) return name;
            break;
        case 's':
            if ( fieldName % "symbol" ) return symbol;
            if ( fieldName % "source" ) return source;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAccountName& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
CAccountName::CAccountName(string_q& strIn) {
    if (countOf(strIn, '\t') == 2) {
        // previous format
        source = strIn;
        addr = toLower(nextTokenClear(source, '\t'));
        name = nextTokenClear(source, '\t');
    } else {
        description = strIn;
        symbol = nextTokenClear(description, '\t');
        name = nextTokenClear(description, '\t');
        addr = toLower(nextTokenClear(description, '\t'));
        source = nextTokenClear(description, '\t');
    }
}

//---------------------------------------------------------------------------
bool CAccountName::Match(const string_q& s1, const string_q& s2, const string_q& s3, bool matchCase, bool all) const {

    string_q theAddr   = addr;
    string_q theName   = name + " " + symbol;
    string_q theSource = source;

    bool m11 = (matchCase ? contains(theAddr  , s1) : contains(toLower(theAddr)  , toLower(s1)));
    bool m12 = (matchCase ? contains(theName  , s1) : contains(toLower(theName)  , toLower(s1)));
    bool m13 = (matchCase ? contains(theSource, s1) : contains(toLower(theSource), toLower(s1)));
    bool m2  = (matchCase ? contains(theName  , s2) : contains(toLower(theName)  , toLower(s2)));
    bool m3  = (matchCase ? contains(theSource, s3) : contains(toLower(theSource), toLower(s3)));

    if (!s1.empty() && !s2.empty() && !s3.empty())
        return m11 && m2 && m3;  // all three must match

    if (!s1.empty() && !s2.empty())
        return m11 && m2;  // addr and name must both match

    if (s1.empty())
        return false;  // nothing matches

    // We have only s1
    return (all ? m11 || m12 || m13 : m11 || m12);
}
// EXISTING_CODE
}  // namespace qblocks

