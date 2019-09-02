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
#include "apispec.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CApiSpec, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextApispecChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextApispecChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CApiSpec::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["apispec_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextApispecChunk, this);
}

//---------------------------------------------------------------------------
string_q nextApispecChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CApiSpec *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CApiSpec::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'h':
            if ( fieldName % "headers" ) { headers = fieldValue; return true; }
            break;
        case 'm':
            if ( fieldName % "method" ) { method = fieldValue; return true; }
            break;
        case 'u':
            if ( fieldName % "uri" ) { uri = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CApiSpec::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CApiSpec::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> method;
    archive >> uri;
    archive >> headers;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CApiSpec::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << method;
    archive << uri;
    archive << headers;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CApiSpecArray& array) {
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
CArchive& operator<<(CArchive& archive, const CApiSpecArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CApiSpec::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CApiSpec, "schema")) return;

    size_t fieldNum = 1000;
    ADD_FIELD(CApiSpec, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CApiSpec, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CApiSpec, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CApiSpec, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CApiSpec, "method", T_TEXT, ++fieldNum);
    ADD_FIELD(CApiSpec, "uri", T_TEXT, ++fieldNum);
    ADD_FIELD(CApiSpec, "headers", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CApiSpec, "schema");
    HIDE_FIELD(CApiSpec, "deleted");
    HIDE_FIELD(CApiSpec, "showing");
    HIDE_FIELD(CApiSpec, "cname");

    builtIns.push_back(_biCApiSpec);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextApispecChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CApiSpec *api = reinterpret_cast<const CApiSpec *>(dataPtr);
    if (api) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, api);
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
bool CApiSpec::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CApiSpec& api) {
    api.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CApiSpec& api) {
    api.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
string_q CApiSpec::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextApispecChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'h':
            if ( fieldName % "headers" ) return headers;
            break;
        case 'm':
            if ( fieldName % "method" ) return method;
            break;
        case 'u':
            if ( fieldName % "uri" ) return uri;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CApiSpec& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_APISPEC =
"";

//---------------------------------------------------------------------------
// EXISTING_CODE
bool CApiSpec::sendData(const string_q& data) {

    //TODO(tjayrush): global data
    CURLcode ret = CURLE_SEND_ERROR;
    CURL *curl_handle = curl_easy_init();
    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, method.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_URL, uri.c_str());
        struct curl_slist *curl_headers = NULL;
        curl_headers = curl_slist_append(curl_headers, "cache-control: no-cache");
        if (curl_headers) {
            CStringArray heads;
            explode(heads, headers, '|');
            for (auto header : heads)
                curl_headers = curl_slist_append(curl_headers, header.c_str());
            curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, curl_headers);
            curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data.c_str());

            ret = curl_easy_perform(curl_handle);
            curl_slist_free_all(curl_headers);
        }
        curl_easy_cleanup(curl_handle);
    }
    return (ret == CURLE_OK);
}

//---------------------------------------------------------------------------
string_q CApiSpec::getData(const string_q& params) {
    cerr << "Not implemented.\n";
    return "Not implemented";
}
// EXISTING_CODE
}  // namespace qblocks

