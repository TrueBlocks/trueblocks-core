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
#include "appearance.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAppearance, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextAppearanceChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextAppearanceChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CAppearance::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["appearance_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextAppearanceChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAppearanceChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAppearance*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CAppearance::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextAppearanceChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    if (fieldName % "tc") {
        if (tc < 10)
            return (expContext().exportFmt == YAML1 || expContext().exportFmt == JSON1 || expContext().exportFmt == API1
                        ? "\"\""
                        : "");
        return uint_2_Str(tc - 10);
    }
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "addr") {
                return addr_2_Str(addr);
            }
            break;
        case 'b':
            if (fieldName % "bn") {
                return uint_2_Str(bn);
            }
            break;
        case 'r':
            if (fieldName % "reason") {
                return reason;
            }
            break;
        case 't':
            if (fieldName % "tx") {
                return uint_2_Str(tx);
            }
            if (fieldName % "tc") {
                return uint_2_Str(tc);
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
bool CAppearance::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "addr") {
                addr = str_2_Addr(fieldValue);
                return true;
            }
            break;
        case 'b':
            if (fieldName % "bn") {
                bn = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'r':
            if (fieldName % "reason") {
                reason = fieldValue;
                return true;
            }
            break;
        case 't':
            if (fieldName % "tx") {
                tx = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "tc") {
                tc = str_2_Uint(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAppearance::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAppearance::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> bn;
    archive >> tx;
    archive >> tc;
    archive >> addr;
    archive >> reason;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAppearance::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << bn;
    archive << tx;
    archive << tc;
    archive << addr;
    archive << reason;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAppearance::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CAppearance copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAppearanceArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAppearanceArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAppearance::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CAppearance, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CAppearance, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAppearance, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CAppearance, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CAppearance, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CAppearance, "bn", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CAppearance, "tx", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CAppearance, "tc", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CAppearance, "addr", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAppearance, "reason", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAppearance, "schema");
    HIDE_FIELD(CAppearance, "deleted");
    HIDE_FIELD(CAppearance, "showing");
    HIDE_FIELD(CAppearance, "cname");

    builtIns.push_back(_biCAppearance);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAppearanceChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CAppearance* app = reinterpret_cast<const CAppearance*>(dataPtr);
    if (app) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, app);
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
bool CAppearance::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CAppearance& app) {
    app.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAppearance& app) {
    app.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAppearance& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_APPEARANCE =
    "[{BN}]\t"
    "[{TX}]\t"
    "[{TC}]\t"
    "[{ADDR}]\t"
    "[{REASON}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
bool CUniqueState::insertUnique(const CAppearance& _value) {
    if (addrOnlyMap) {
        CAddressOnlyAppearanceMap::iterator it = addrOnlyMap->find(_value);
        if (it == addrOnlyMap->end()) {  // not found
            it = addrOnlyMap->insert(make_pair(_value, true)).first;
            if (func)
                if (!(*func)(it->first, data))
                    return false;
        }
        return it->second;
    }

    CAddressTxAppearanceMap::iterator it = addrTxMap->find(_value);
    if (it == addrTxMap->end()) {  // not found
        it = addrTxMap->insert(make_pair(_value, true)).first;
        if (func)
            if (!(*func)(it->first, data))
                return false;
    }
    return it->second;
}

//---------------------------------------------------------------------------
bool accumulateAddresses(const CAppearance& item, void* data) {
    if (isZeroAddr(item.addr))
        return true;
    CUniqueState* state = (CUniqueState*)data;  // NOLINT
    CAppearance search(item.bn, item.tx, item.tc, item.addr, item.reason);
    return state->insertUnique(search);  // NOLINT
}

//---------------------------------------------------------------------------
bool CBlock::forEveryUniqueAppearanceInBlock(APPEARANCEFUNC func, TRANSFUNC traceFilter, void* data) {
    if (!func)
        return false;
    CUniqueState state(func, data, false);
    return forEveryAppearanceInBlock(accumulateAddresses, traceFilter, &state);
}

//---------------------------------------------------------------------------
bool CBlock::forEveryUniqueAppearanceInBlockPerTx(APPEARANCEFUNC func, TRANSFUNC traceFilter, void* data) {
    if (!func)
        return false;
    CUniqueState state(func, data, true);
    return forEveryAppearanceInBlock(accumulateAddresses, traceFilter, &state);
}

//---------------------------------------------------------------------------
bool isPotentialAddr(biguint_t test, address_t& addrOut) {
    addrOut = "";

    // smallest address we find
    static const biguint_t small = str_2_Wei("0x00000000000000ffffffffffffffffffffffffff");
    // largest address we find
    static const biguint_t large = str_2_Wei("0x010000000000000000000000000000000000000000");
    if (test <= small || test >= large)
        return false;

    addrOut = bnu_2_Hex(test).c_str();
    // Totally a heuristic that can't really be supported, but a good probability that this isn't an address
    if (endsWith(addrOut, "00000000"))
        return false;

    if (addrOut.length() < 40)
        addrOut = padLeft(addrOut, 40, '0');
    addrOut = extract(addrOut, addrOut.length() - 40, 40);
    addrOut = toLower("0x" + addrOut);

    return true;
}

//---------------------------------------------------------------------------
bool potentialAddr(APPEARANCEFUNC func, void* data, const CAppearance& item, const string_q& potList) {
    if (!func)
        return false;

    // Pull out 32-byte chunks and check to see if they are addresses
    address_t addr;
    for (size_t s = 0; s < potList.length() / 64; s++) {
        biguint_t test = str_2_Wei("0x" + extract(potList, s * 64, 64));
        if (isPotentialAddr(test, addr)) {
            CAppearance it(item);
            it.addr = addr;
            if (!(*func)(it, data))
                return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------
bool foundOne(APPEARANCEFUNC func, void* data, blknum_t bn, blknum_t tx, blknum_t tc, const address_t& addr,
              const string_q& reason) {  // NOLINT
    CAppearance item(bn, tx, tc, addr, reason);
    return (*func)(item, data);
}

//---------------------------------------------------------------------------
bool foundPot(APPEARANCEFUNC func, void* data, blknum_t bn, blknum_t tx, blknum_t tc, const string_q& potList,
              const string_q& reason) {  // NOLINT
    CAppearance item(bn, tx, tc, "", reason);
    return potentialAddr(func, data, item, potList);
}

//----------------------------------------------------------------
int findAddresses(const void* v1, const void* v2) {
    const CIndexedAddress* at1 = (CIndexedAddress*)v1;  // NOLINT
    const CIndexedAddress* at2 = (CIndexedAddress*)v2;  // NOLINT
    for (size_t i = 0; i < 20; i++) {
        int ret = at1->bytes[i] - at2->bytes[i];
        if (ret)
            return ret;
    }
    return 0;
}
// EXISTING_CODE
}  // namespace qblocks
