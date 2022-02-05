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
#include "chain.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CChain, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextChainChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextChainChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CChain::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["chain_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextChainChunk, this);
}

//---------------------------------------------------------------------------
string_q nextChainChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CChain*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CChain::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextChainChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "apiProvider") {
                return apiProvider;
            }
            break;
        case 'c':
            if (fieldName % "chain") {
                return chain;
            }
            if (fieldName % "chainId") {
                return uint_2_Str(chainId);
            }
            break;
        case 'l':
            if (fieldName % "localExplorer") {
                return localExplorer;
            }
            break;
        case 'p':
            if (fieldName % "pinGateway") {
                return pinGateway;
            }
            break;
        case 'r':
            if (fieldName % "rpcProvider") {
                return rpcProvider;
            }
            if (fieldName % "remoteExplorer") {
                return remoteExplorer;
            }
            break;
        case 's':
            if (fieldName % "symbol") {
                return symbol;
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
bool CChain::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "apiProvider") {
                apiProvider = fieldValue;
                return true;
            }
            break;
        case 'c':
            if (fieldName % "chain") {
                chain = fieldValue;
                return true;
            }
            if (fieldName % "chainId") {
                chainId = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'l':
            if (fieldName % "localExplorer") {
                localExplorer = fieldValue;
                return true;
            }
            break;
        case 'p':
            if (fieldName % "pinGateway") {
                pinGateway = fieldValue;
                return true;
            }
            break;
        case 'r':
            if (fieldName % "rpcProvider") {
                rpcProvider = fieldValue;
                return true;
            }
            if (fieldName % "remoteExplorer") {
                remoteExplorer = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "symbol") {
                symbol = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CChain::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CChain::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> chain;
    archive >> chainId;
    archive >> symbol;
    archive >> rpcProvider;
    archive >> apiProvider;
    archive >> remoteExplorer;
    archive >> localExplorer;
    archive >> pinGateway;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CChain::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << chain;
    archive << chainId;
    archive << symbol;
    archive << rpcProvider;
    archive << apiProvider;
    archive << remoteExplorer;
    archive << localExplorer;
    archive << pinGateway;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CChain::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CChain copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CChainArray& array) {
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
CArchive& operator<<(CArchive& archive, const CChainArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CChain::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CChain, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CChain, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CChain, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CChain, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CChain, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CChain, "chain", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CChain, "chainId", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CChain, "symbol", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CChain, "rpcProvider", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CChain, "apiProvider", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CChain, "remoteExplorer", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CChain, "localExplorer", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CChain, "pinGateway", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CChain, "schema");
    HIDE_FIELD(CChain, "deleted");
    HIDE_FIELD(CChain, "showing");
    HIDE_FIELD(CChain, "cname");

    builtIns.push_back(_biCChain);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextChainChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CChain* cha = reinterpret_cast<const CChain*>(dataPtr);
    if (cha) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, cha);
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
bool CChain::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CChain& cha) {
    cha.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CChain& cha) {
    cha.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CChain& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_CHAIN = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
