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
#include "newproposalinwei.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CNewProposalInWei, CTransaction);

//---------------------------------------------------------------------------
static string_q nextNewproposalinweiChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextNewproposalinweiChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CNewProposalInWei::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextNewproposalinweiChunk, this);
}

//---------------------------------------------------------------------------
string_q nextNewproposalinweiChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CNewProposalInWei *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CNewProposalInWei::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "beneficiary" ) { beneficiary = str_2_Addr(fieldValue); return true; }
            break;
        case 'j':
            if ( fieldName % "JobDescription" ) { JobDescription = fieldValue; return true; }
            break;
        case 't':
            if ( fieldName % "transactionBytecode" ) { transactionBytecode = toLower(fieldValue); return true; }
            break;
        case 'w':
            if ( fieldName % "weiAmount" ) { weiAmount = str_2_Wei(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CNewProposalInWei::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CNewProposalInWei::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CTransaction::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> beneficiary;
    archive >> weiAmount;
    archive >> JobDescription;
    archive >> transactionBytecode;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CNewProposalInWei::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << beneficiary;
    archive << weiAmount;
    archive << JobDescription;
    archive << transactionBytecode;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CNewProposalInWeiArray& array) {
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
CArchive& operator<<(CArchive& archive, const CNewProposalInWeiArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CNewProposalInWei::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CNewProposalInWei, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewProposalInWei, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CNewProposalInWei, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CNewProposalInWei, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CNewProposalInWei, "beneficiary", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CNewProposalInWei, "weiAmount", T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewProposalInWei, "JobDescription", T_TEXT, ++fieldNum);
    ADD_FIELD(CNewProposalInWei, "transactionBytecode", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CNewProposalInWei, "schema");
    HIDE_FIELD(CNewProposalInWei, "deleted");
    HIDE_FIELD(CNewProposalInWei, "showing");
    HIDE_FIELD(CNewProposalInWei, "cname");

    builtIns.push_back(_biCNewProposalInWei);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextNewproposalinweiChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CNewProposalInWei *newp = reinterpret_cast<const CNewProposalInWei *>(dataPtr);
    if (newp) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, newp);
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
bool CNewProposalInWei::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CNewProposalInWei::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextNewproposalinweiChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "beneficiary" ) return addr_2_Str(beneficiary);
            break;
        case 'j':
            if ( fieldName % "JobDescription" ) return JobDescription;
            break;
        case 't':
            if ( fieldName % "transactionBytecode" ) return transactionBytecode;
            break;
        case 'w':
            if ( fieldName % "weiAmount" ) return bnu_2_Str(weiAmount);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CNewProposalInWei& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

