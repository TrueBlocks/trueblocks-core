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
#include "transfer.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTransfer, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextTransferChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextTransferChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CTransfer::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["transfer_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextTransferChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTransferChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTransfer*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CTransfer::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextTransferChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "assetAddr") {
                return addr_2_Str(assetAddr);
            }
            if (fieldName % "assetSymbol") {
                return assetSymbol;
            }
            if (fieldName % "amount") {
                return bnu_2_Str(amount);
            }
            break;
        case 'b':
            if (fieldName % "blockNumber") {
                return uint_2_Str(blockNumber);
            }
            break;
        case 'd':
            if (fieldName % "date") {
                return date.Format(FMT_JSON);
            }
            if (fieldName % "decimals") {
                return uint_2_Str(decimals);
            }
            break;
        case 'e':
            if (fieldName % "encoding") {
                return encoding;
            }
            break;
        case 'l':
            if (fieldName % "logIndex") {
                return uint_2_Str(logIndex);
            }
            break;
        case 'p':
            if (fieldName % "priceSource") {
                return priceSource;
            }
            break;
        case 'r':
            if (fieldName % "recipient") {
                return addr_2_Str(recipient);
            }
            break;
        case 's':
            if (fieldName % "sender") {
                return addr_2_Str(sender);
            }
            if (fieldName % "spotPrice") {
                return double_2_Str(spotPrice, 5);
            }
            break;
        case 't':
            if (fieldName % "transactionIndex") {
                return uint_2_Str(transactionIndex);
            }
            if (fieldName % "transactionHash") {
                return hash_2_Str(transactionHash);
            }
            if (fieldName % "timestamp") {
                return ts_2_Str(timestamp);
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
bool CTransfer::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "assetAddr") {
                assetAddr = str_2_Addr(fieldValue);
                return true;
            }
            if (fieldName % "assetSymbol") {
                assetSymbol = fieldValue;
                return true;
            }
            if (fieldName % "amount") {
                amount = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'b':
            if (fieldName % "blockNumber") {
                blockNumber = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'd':
            if (fieldName % "date") {
                date = str_2_Date(fieldValue);
                return true;
            }
            if (fieldName % "decimals") {
                decimals = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'e':
            if (fieldName % "encoding") {
                encoding = fieldValue;
                return true;
            }
            break;
        case 'l':
            if (fieldName % "logIndex") {
                logIndex = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'p':
            if (fieldName % "priceSource") {
                priceSource = fieldValue;
                return true;
            }
            break;
        case 'r':
            if (fieldName % "recipient") {
                recipient = str_2_Addr(fieldValue);
                return true;
            }
            break;
        case 's':
            if (fieldName % "sender") {
                sender = str_2_Addr(fieldValue);
                return true;
            }
            if (fieldName % "spotPrice") {
                spotPrice = str_2_Double(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "transactionIndex") {
                transactionIndex = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "transactionHash") {
                transactionHash = str_2_Hash(fieldValue);
                return true;
            }
            if (fieldName % "timestamp") {
                timestamp = str_2_Ts(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTransfer::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTransfer::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> blockNumber;
    archive >> transactionIndex;
    archive >> logIndex;
    archive >> transactionHash;
    archive >> timestamp;
    archive >> date;
    archive >> sender;
    archive >> recipient;
    archive >> assetAddr;
    archive >> assetSymbol;
    archive >> decimals;
    archive >> amount;
    archive >> spotPrice;
    archive >> priceSource;
    archive >> encoding;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTransfer::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockNumber;
    archive << transactionIndex;
    archive << logIndex;
    archive << transactionHash;
    archive << timestamp;
    archive << date;
    archive << sender;
    archive << recipient;
    archive << assetAddr;
    archive << assetSymbol;
    archive << decimals;
    archive << amount;
    archive << spotPrice;
    archive << priceSource;
    archive << encoding;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTransfer::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CTransfer copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTransferArray& array) {
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
CArchive& operator<<(CArchive& archive, const CTransferArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTransfer::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CTransfer, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CTransfer, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransfer, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CTransfer, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CTransfer, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CTransfer, "blockNumber", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CTransfer, "transactionIndex", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CTransfer, "logIndex", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CTransfer, "transactionHash", T_HASH | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTransfer, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CTransfer, "date", T_DATE, ++fieldNum);
    ADD_FIELD(CTransfer, "sender", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTransfer, "recipient", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTransfer, "assetAddr", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTransfer, "assetSymbol", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTransfer, "decimals", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CTransfer, "amount", T_UINT256, ++fieldNum);
    ADD_FIELD(CTransfer, "spotPrice", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CTransfer, "priceSource", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTransfer, "encoding", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTransfer, "schema");
    HIDE_FIELD(CTransfer, "deleted");
    HIDE_FIELD(CTransfer, "showing");
    HIDE_FIELD(CTransfer, "cname");

    builtIns.push_back(_biCTransfer);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTransferChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CTransfer* tra = reinterpret_cast<const CTransfer*>(dataPtr);
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

// EXISTING_CODE
// EXISTING_CODE

//---------------------------------------------------------------------------
bool CTransfer::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CTransfer& tra) {
    tra.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTransfer& tra) {
    tra.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTransfer& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_TRANSFER =
    "[{BLOCKNUMBER}]\t"
    "[{TRANSACTIONINDEX}]\t"
    "[{LOGINDEX}]\t"
    "[{TRANSACTIONHASH}]\t"
    "[{TIMESTAMP}]\t"
    "[{DATE}]\t"
    "[{SENDER}]\t"
    "[{RECIPIENT}]\t"
    "[{ASSETADDR}]\t"
    "[{ASSETSYMBOL}]\t"
    "[{DECIMALS}]\t"
    "[{AMOUNT}]\t"
    "[{SPOTPRICE}]\t"
    "[{PRICESOURCE}]\t"
    "[{ENCODING}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// For each assset, we keep the last available balances in a map. When reconciling an
// asset for the current transaction, we need the asset's previous balance. We index
// into the previous balances using the accountedFor address and the asset's address.
string_q statementKey(const address_t& accountedFor, const address_t& assetAddr) {
    if (isZeroAddr(assetAddr) || isEtherAddr(assetAddr)) {
        return toLower(accountedFor + "-" + "_eth");
    }
    return toLower(accountedFor + "-" + assetAddr);
}
// EXISTING_CODE
}  // namespace qblocks
