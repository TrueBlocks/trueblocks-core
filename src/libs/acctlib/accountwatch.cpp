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
#include "accountwatch.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAccountWatch, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextAccountwatchChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextAccountwatchChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAccountWatch::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextAccountwatchChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAccountwatchChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAccountWatch *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    if (fieldName % "qbis") {
        string_q str = fieldValue;
        return qbis.parseJson3(str);
    }
    if (fieldName % "balance") {
        qbis.endBal = qbis.begBal = str_2_Wei(fieldValue);
        return true;
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) { address = str_2_Addr(fieldValue); return true; }
            break;
        case 'b':
            if ( fieldName % "balanceHistory" ) {
                CBalanceHistory item;
                string_q str = fieldValue;
                while (item.parseJson3(str)) {
                    balanceHistory.push_back(item);
                    item = CBalanceHistory();  // reset
                }
                return true;
            }
            break;
        case 'c':
            if ( fieldName % "color" ) { color = fieldValue; return true; }
            break;
        case 'd':
            if ( fieldName % "deepScan" ) { deepScan = str_2_Bool(fieldValue); return true; }
            break;
        case 'f':
            if ( fieldName % "firstBlock" ) { firstBlock = str_2_Uint(fieldValue); return true; }
            break;
        case 'l':
            if ( fieldName % "lastBlock" ) { lastBlock = str_2_Uint(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "name" ) { name = fieldValue; return true; }
            if ( fieldName % "nodeBal" ) { nodeBal = str_2_Wei(fieldValue); return true; }
            break;
        case 'q':
            if ( fieldName % "qbis" ) { /* qbis = fieldValue; */ return false; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAccountWatch::finishParse() {
    // EXISTING_CODE
    bloom = makeBloom(address);
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::Serialize(CArchive& archive) {

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
    archive >> name;
    archive >> color;
    archive >> firstBlock;
    archive >> lastBlock;
    archive >> deepScan;
    archive >> qbis;
    archive >> balanceHistory;
    archive >> nodeBal;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << address;
    archive << name;
    archive << color;
    archive << firstBlock;
    archive << lastBlock;
    archive << deepScan;
    archive << qbis;
    archive << balanceHistory;
    archive << nodeBal;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAccountWatchArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAccountWatchArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAccountWatch::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    size_t fieldNum = 1000;
    ADD_FIELD(CAccountWatch, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountWatch, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAccountWatch, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAccountWatch, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CAccountWatch, "address", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CAccountWatch, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "color", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "firstBlock", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountWatch, "lastBlock", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountWatch, "deepScan", T_BOOL, ++fieldNum);
    ADD_FIELD(CAccountWatch, "qbis", T_OBJECT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "balanceHistory", T_OBJECT|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CAccountWatch, "nodeBal", T_WEI, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAccountWatch, "schema");
    HIDE_FIELD(CAccountWatch, "deleted");
    HIDE_FIELD(CAccountWatch, "showing");
    HIDE_FIELD(CAccountWatch, "cname");

    builtIns.push_back(_biCAccountWatch);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAccountwatchChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CAccountWatch *acc = reinterpret_cast<const CAccountWatch *>(dataPtr);
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
bool CAccountWatch::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CAccountWatch::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextAccountwatchChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) return addr_2_Str(address);
            break;
        case 'b':
            if ( fieldName % "balanceHistory" || fieldName % "balanceHistoryCnt" ) {
                size_t cnt = balanceHistory.size();
                if (endsWith(fieldName, "Cnt"))
                    return uint_2_Str(cnt);
                if (!cnt) return "";
                string_q retS;
                for (size_t i = 0 ; i < cnt ; i++) {
                    retS += balanceHistory[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 'c':
            if ( fieldName % "color" ) return color;
            break;
        case 'd':
            if ( fieldName % "deepScan" ) return int_2_Str(deepScan);
            break;
        case 'f':
            if ( fieldName % "firstBlock" ) return uint_2_Str(firstBlock);
            break;
        case 'l':
            if ( fieldName % "lastBlock" ) return uint_2_Str(lastBlock);
            break;
        case 'n':
            if ( fieldName % "name" ) return name;
            if ( fieldName % "nodeBal" ) return wei_2_Str(nodeBal);
            break;
        case 'q':
            if ( fieldName % "qbis" ) { expContext().noFrst=true; return qbis.Format(); }
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    string_q s;
    s = toUpper(string_q("qbis")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = qbis.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAccountWatch& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CAccountWatch::getObjectAt(const string_q& fieldName, size_t index) const {
    if ( fieldName % "qbis" )
        return &qbis;
    if ( fieldName % "balanceHistory" && index < balanceHistory.size() )
        return &balanceHistory[index];
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
string_q CAccountWatch::displayName(bool expand, bool useColor, bool terse, size_t w1, size_t w2) const {
    if (address == "others") {
        return padRight(name, w1 + w2 + 1);
    }

    if (terse) {
        uint64_t len = name.length();
        uint64_t need = 42 - len - 6;  // the six is for " (" and "...)"
        string_q ret;
//        if (useColor)
//            ret += color;
        if (expand)
            ret += "[000000000000";
        ret += extract(name, 0, 42-6);
        ret += " (" + extract(address, 0, need) + "...)";
        if (expand)
            ret += "]";
//        if (useColor)
//            ret += cOff;
        return ret;
    }
    return padRight(extract(name, 0, w1), w1) + " " + extract(address, 0, w2) + " ";
}

//-----------------------------------------------------------------------
biguint_t getNodeBal(CBalanceHistoryArray& history, const address_t& addr, blknum_t blockNum) {

    if (!startsWith(addr, "0x"))
        return 0;

    // This will return 'true' if the node has historical balances - we want to use those
    // balances if they are there
    if (nodeHasBalances())
        return getBalance(addr, blockNum, false);

    // If the history is empty, we can try to load the history from a file if it exists...
    string_q binaryFilename = "./balances/" + addr + ".bals.bin";
    if (history.size() == 0 && fileExists(binaryFilename) && fileSize(binaryFilename) > 0) {

        CArchive balCache(READING_ARCHIVE);
        if (balCache.Lock(binaryFilename, binaryReadOnly, LOCK_NOWAIT)) {
            blknum_t last = NOPOS;
            address_t lastA;
            while (!balCache.Eof()) {
                blknum_t bn;
                address_t addr1;
                biguint_t bal;
                balCache >> bn >> addr1 >> bal;
                if (addr == addr1) {
                    if (last != bn || bal != 0) {
                        CBalanceHistory newBal;
                        newBal.bn = bn;
                        newBal.balance = bal;
                        history.push_back(newBal);
                        last = bn;
                    }
                    cerr << "Loaded block  #" << bn << " at " << addr1 << "\r";
                } else {
                    cerr << "Loaded block  #" << bn << " at " << addr1 << "\r";
                }
                cerr.flush();
            }
        }
        cerr << "\n";
        cerr.flush();
    }

    // First, we try to find it using a binary search. Many times this will hit...
    CBalanceHistory search;
    search.bn = blockNum;
    const CBalanceHistoryArray::iterator it = find(history.begin(), history.end(), search);
    if (it != history.end())
        return it->balance;

    // ...if it doesn't hit, we need to find the most recent balance
    biguint_t ret = 0;
    for (size_t i = 0 ; i < history.size() ; i++) {
        // TODO(tjayrush): THIS IS A BUG HACK FIX - SEE ABOVE
        // We should be able to do >= just below, but if we do, we pick up a zero
        // balance as the last balance available for any account because the code
        // that generates the balance history includes a incorrect last duplicated value
        if (history[i].bn == blockNum)
            return history[i].balance;

        // ...If we've overshot, we report the previous balance
        if (history[i].bn > blockNum)
            return ret;

        ret = history[i].balance;
    }

    // We've run off the end of the array, return the most recent balance (if any)
    if (ret > 0)
        return ret;

    // We finally fall to the node in case we're near the head
    return getBalance(addr, blockNum, false);
}

//-----------------------------------------------------------------------
// This assumes there are valid watches. Caller is expected to check
void loadWatchList(const CToml& toml, CAccountWatchArray& watches, const string_q& key) {

    string_q watchStr = toml.getConfigStr("watches", key, "");

    CAccountWatch watch;
    while (watch.parseJson3(watchStr)) {
        // cleanup and add to list of watches
        watch.address = str_2_Addr(toLower(watch.address));
        watch.color   = convertColor(watch.color);
        watches.push_back(watch);
        watch = CAccountWatch();  // reset
    }
    return;
}
// EXISTING_CODE
}  // namespace qblocks

