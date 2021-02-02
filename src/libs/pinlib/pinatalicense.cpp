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
#include "pinatalicense.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPinataLicense, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextPinatalicenseChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextPinatalicenseChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CPinataLicense::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["pinatalicense_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextPinatalicenseChunk, this);
}

//---------------------------------------------------------------------------
string_q nextPinatalicenseChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CPinataLicense*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CPinataLicense::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextPinatalicenseChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "apiKey") {
                return apiKey;
            }
            break;
        case 's':
            if (fieldName % "secretKey") {
                return secretKey;
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
bool CPinataLicense::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "apiKey") {
                apiKey = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "secretKey") {
                secretKey = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CPinataLicense::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CPinataLicense::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> apiKey;
    archive >> secretKey;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPinataLicense::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << apiKey;
    archive << secretKey;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinataLicenseArray& array) {
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
CArchive& operator<<(CArchive& archive, const CPinataLicenseArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CPinataLicense::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CPinataLicense, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CPinataLicense, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CPinataLicense, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinataLicense, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinataLicense, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CPinataLicense, "apiKey", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinataLicense, "secretKey", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPinataLicense, "schema");
    HIDE_FIELD(CPinataLicense, "deleted");
    HIDE_FIELD(CPinataLicense, "showing");
    HIDE_FIELD(CPinataLicense, "cname");

    builtIns.push_back(_biCPinataLicense);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextPinatalicenseChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CPinataLicense* pin = reinterpret_cast<const CPinataLicense*>(dataPtr);
    if (pin) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, pin);
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
bool CPinataLicense::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CPinataLicense& pin) {
    pin.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinataLicense& pin) {
    pin.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CPinataLicense& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_PINATALICENSE =
    "[{APIKEY}]\t"
    "[{SECRETKEY}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
