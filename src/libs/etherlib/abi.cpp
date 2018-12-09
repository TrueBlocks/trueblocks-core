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
#include "abi.h"
#include "node.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAbi, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextAbiChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextAbiChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAbi::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextAbiChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAbiChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAbi *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAbi::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) { address = str_2_Addr(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "interfaces" ) {
                CFunction item;
                string_q str = fieldValue;
                while (item.parseJson3(str)) {
                    interfaces.push_back(item);
                    item = CFunction();  // reset
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
void CAbi::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAbi::Serialize(CArchive& archive) {

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
    archive >> interfaces;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAbi::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << address;
    archive << interfaces;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAbiArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAbiArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAbi::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    size_t fieldNum = 1000;
    ADD_FIELD(CAbi, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAbi, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAbi, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAbi, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CAbi, "address", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CAbi, "interfaces", T_OBJECT|TS_ARRAY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAbi, "schema");
    HIDE_FIELD(CAbi, "deleted");
    HIDE_FIELD(CAbi, "showing");
    HIDE_FIELD(CAbi, "cname");

    builtIns.push_back(_biCAbi);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAbiChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CAbi *abi = reinterpret_cast<const CAbi *>(dataPtr);
    if (abi) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, abi);
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
bool CAbi::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CAbi& abi) {
    abi.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAbi& abi) {
    abi.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
string_q CAbi::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextAbiChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) return addr_2_Str(address);
            break;
        case 'i':
            if ( fieldName % "interfaces" || fieldName % "interfacesCnt" ) {
                size_t cnt = interfaces.size();
                if (endsWith(fieldName, "Cnt"))
                    return uint_2_Str(cnt);
                if (!cnt) return "";
                string_q retS;
                for (size_t i = 0 ; i < cnt ; i++) {
                    retS += interfaces[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAbi& item) {
    // EXISTING_CODE
    if (sizeof(item) != 0) {  // always true, but we do this to avoid a warning
        for (auto interface : item.interfaces) {
            os << interface.Format() << "\n";
        }
        return os;
    }
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CAbi::getObjectAt(const string_q& fieldName, size_t index) const {
    if ( fieldName % "interfaces" && index < interfaces.size() )
        return &interfaces[index];
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
    bool visitABI(const qblocks::string_q& path, void *data) {
        if (!endsWith(path, ".json"))
            return true;
        CAbi *abi = (CAbi*)data;
        if (!abi->loadAbiFromFile(path, true))
            return false;
        return true;
    }

    //---------------------------------------------------------------------------
    bool CAbi::loadAbiKnown(const string_q& which) {
        if (which == "all")
            return forEveryFileInFolder(configPath("known_abis/*"), visitABI, this);
        return loadAbiFromFile(configPath("known_abis/" + which + ".json"), true);
    }

    //---------------------------------------------------------------------------
    bool CAbi::loadAbiByAddress(address_t addrIn) {
        string_q addr = toLower(addrIn);
        string_q fileName = blockCachePath("abis/" + addr + ".json");
        return loadAbiFromFile(fileName, false);
    }

    //---------------------------------------------------------------------------
    bool CAbi::loadAbiFromFile(const string_q& fileName, bool builtIn) {
        if (!fileExists(fileName))
            return false;
        string_q contents;
        asciiFileToString(fileName, contents);
        return loadAbiFromString(contents, builtIn);
    }

    //---------------------------------------------------------------------------
    bool CAbi::loadAbiFromString(const string_q& in, bool builtIn) {
        string_q contents = in;
        CFunction func;
        while (func.parseJson3(contents)) {
            func.isBuiltIn = builtIn;
            interfaces.push_back(func);
            func = CFunction();  // reset
        }
        sort(interfaces.begin(), interfaces.end());
        return interfaces.size();
    }

    //-----------------------------------------------------------------------
    bool CAbi::addIfUnique(const string_q& addr, CFunction& func, bool decorateNames) {
        if (func.name.empty())  // && func.type != "constructor")
            return false;

        for (auto f : interfaces) {
            if (f.encoding == func.encoding)
                return false;

            // different encoding same name means a duplicate function name in the code. We won't build with
            // duplicate function names, so we need to modify the incoming function. We do this by appending
            // the first four characters of the contract's address.
            if (decorateNames && f.name == func.name && !f.isBuiltIn) {
                func.origName = func.name;
                func.name += (startsWith(addr, "0x") ? extract(addr, 2, 4) : extract(addr, 0, 4));
            }
        }

        interfaces.push_back(func);
        return true;
    }

    //-----------------------------------------------------------------------
    bool CAbi::loadAbiAndCache(const address_t& addr, bool raw, bool silent, bool decNames) {

        string_q results;
        string_q fileName = blockCachePath("abis/" + addr + ".json");
        string_q localFile("./" + addr + ".json");
        if (fileExists(localFile)) {
            cerr << "Local file copied to cache\n";
            copyFile(localFile, fileName);
        }

        string_q dispName = substitute(fileName, blockCachePath(""), "$CACHE/");
        if (fileExists(fileName) && !raw) {

            if (verbose) {
                cerr << "Reading ABI for address " << addr << " from " << (isTestMode() ? "--" : "cache") << "\r";
                cerr.flush();
            }
            asciiFileToString(fileName, results);

        } else {
            if (verbose) {
                cerr << "Reading ABI for address " << addr << " from " << (isTestMode() ? "--" : "EtherScan") << "\r";
                cerr.flush();
            }
            string_q url = string_q("http:/")
                                    + "/api.etherscan.io/api?module=contract&action=getabi&address="
                                    + addr;
            results = substitute(urlToString(url), "\\", "");
            if (!contains(results, "NOTOK")) {

                // Clear the RPC wrapper
                if (!isTestMode()) {
                    if (verbose)
                        cerr << results << endl;
                    cerr << "Caching abi in " << dispName << endl;
                }
                replace(results, "\"result\":\"", "<extract>");
                replaceReverse(results, "\"}", "</extract>");
                results = snagFieldClear(results, "extract", "");
                establishFolder(fileName);
                stringToAsciiFile(fileName, results);

            } else if (contains(toLower(results), "source code not verified")) {

                if (!silent) {
                    cerr << "\n";
                    cerr << cRed << "Warning: " << cOff;
                    cerr << "Failed to grab the ABI. Etherscan returned:\n\n\t";
                    cerr << cTeal << results << cOff << "\n\n";
                    cerr << "However, the ABI may actually be present on EtherScan. QBlocks will use it if\n";
                    cerr << "you copy and paste the ABI json to this file:\n\n\t";
                    cerr << cTeal << localFile << cOff << "\n\n";
                    quickQuitHandler(0);
                }

            } else {

                if (!silent) {
                    cerr << "Etherscan returned " << results << "\n";
                    cerr << "Could not grab ABI for " + addr + " from etherscan.io.\n";
                    quickQuitHandler(0);
                }

                // TODO(tjayrush): If we store the ABI here even if empty, we won't have to get it again, but then
                // what happens if user later posts the ABI? Need a 'refresh' option or clear cache option
                establishFolder(fileName);
                stringToAsciiFile(fileName, "[]");
            }
        }

        CFunction func;
        ostringstream os;
        while (func.parseJson3(results)) {
            if (addIfUnique(addr, func, decNames))
                func.doExport(os);
            func = CFunction();  // reset
        }
        return !os.str().empty();
    }
    //----------------------------------------------------------------------------
    inline unsigned char hex2Ascii(char *str) {
        unsigned char c;
        c =  (unsigned char)((str[0] >= 'A' ? ((str[0]&0xDF)-'A')+10 : (str[0]-'0')));
        c *= 16;
        c = (unsigned char)(c + (str[1] >= 'A' ? ((str[1]&0xDF)-'A')+10 : (str[1]-'0')));
        return c;
    }

    //----------------------------------------------------------------------------
    inline string_q hex2String(const string_q& inHex) {
        string_q ret, in = startsWith(inHex, "0x") ? extract(inHex, 2) : inHex;
        while (!in.empty()) {
            string_q nibble = extract(in, 0, 2);
            in = extract(in, 2);
            ret += (char)hex2Ascii((char*)nibble.c_str());  // NOLINT
        }
        return ret;
    }

    //------------------------------------------------------------------------------
#define old_grabPart(a, b)       trimLeading(extract((a), 64*(b), 64), '0')
#define old_toBigNum2(a, b)      string_q(bnu_2_Str(str_2_Wei("0x" + old_grabPart(a, b))).c_str())
#define old_grabBigNum(a, b)     strtoull(old_grabPart(a, b).c_str(), NULL, 16)
#define old_toAddr(a, b)         "0x" + padLeft(old_grabPart(a, b), 40, '0')
#define old_toBigNum3(a, b)      padNum3(old_grabBigNum(a, b))
#define old_toBoolean(a, b)      (old_grabBigNum(a, b) ? "true" : "false")
#define old_toBytes(a, b)        extract((a), 64*(b), 64)
    inline string_q parseTheInput(const string_q& paramsIn, size_t nItems, string_q *types) {

        string_q params = substitute(paramsIn, "0x", "");

        string_q ret;
        for (size_t item = 0 ; item < nItems ; item++) {
            string_q t = types[item];
            bool isDynamic = (t == "string" || t == "bytes" || contains(t, "[]"));
            string_q val;

                 if ( t == "address"                    )   val =          old_toAddr     (params, item);  // NOLINT
            else if ( t == "bool"                       )   val =          old_toBoolean  (params, item);
            else if ( t == "uint3"                      )   val =          old_toBigNum3  (params, item);
            else if ( contains(t, "int") &&   !isDynamic)   val =          old_toBigNum2  (params, item);
            else if ( contains(t, "bytes") && !isDynamic)   val =          old_toBytes    (params, item);
            else if (!isDynamic                         )   val = "unknown type: " + t;
            else {
                ASSERT(isDynamic);
                size_t start = str_2_Uint(old_toBigNum2(params, item)) / (size_t)32;
                size_t len = old_grabBigNum(params, start);
                if (len == NOPOS)
                    len = params.length()-start;
                if (t == "string") {
                    string_q ss1 = extract(params, (start+1) * 64, len * 2);
                    string_q ss2 = hex2String(ss1);
                    ss2 = substitute(ss2, "\n", "\\n");
                    ss2 = substitute(ss2, "\r", "");
                    ss2 = substitute(ss2, "\"", "\\\"");
                    val += ss2;
                } else {
                    val = "0x" + extract(params, (start+1) * 64, len * 2);
                }
            }
            ret += ("|" + val);
        }

        return "\"" + trim(ret, '|') + "\"";
    }

    //---------------------------------------------------------------------------
    inline string_q decodeRLP(const string_q& name, const string_q& input, size_t nItems, string_q *items) {
        string_q quote = "\"";
        string_q params = input;
        string_q result = parseTheInput(params, nItems, items);
        result = substitute(result, "|", "\", \"");
        return quote + name + quote + ", " + result;
    }

    //---------------------------------------------------------------------------
    inline bool decodeRLP2(const string_q& name, CParameterArray& interfaces, const string_q& input) {
        string_q items[256];
        size_t nItems = 0;
        for (auto param : interfaces)
            items[nItems++] = param.type;
        string_q decoded = substitute(decodeRLP(name, input, nItems, items), "\"", "");
        nextTokenClear(decoded, ',');
        for (size_t j = 0 ; j < interfaces.size(); j++)
            interfaces[j].value = trim(nextTokenClear(decoded, ','));
        return true;
    }

    //-----------------------------------------------------------------------
    bool CAbi::articulateTransaction(CTransaction *p) const {

        if (!p)
            return false;

        // articulate the events, so we can return with a fully articulated object
        for (size_t i = 0 ; i < p->receipt.logs.size() ; i++)
            articulateLog(&p->receipt.logs[i]);

        // articulate the traces, so we can return with a fully articulated object
        for (size_t i = 0 ; i < p->traces.size() ; i++)
            articulateTrace(&p->traces[i]);

        if (p->input.length() >= 10 || p->input == "0x") {
            string_q encoding = extract(p->input, 0, 10);
            string_q params   = extract(p->input, 10);
            for (auto interface : interfaces) {
                if (encoding % interface.encoding) {
                    p->articulatedTx = CFunction(interface);
                    return decodeRLP2(interface.name, p->articulatedTx.inputs, params);
                }
            }
        }
        return false;
    }

    //-----------------------------------------------------------------------
    bool CAbi::articulateLog(CLogEntry *p) const {

        if (!p)
            return false;

        size_t nTops = p->topics.size();
        if (nTops > 0) {  // the '0'th topic is the event signature
            string_q data = extract(p->data, 2);
            string_q params;
            bool first = true;
            for (auto t : p->topics) {
                if (!first)
                    params += extract(topic_2_Str(t),2);
                first = false;
            }
            params += data;

            for (auto interface : interfaces) {
                if (topic_2_Str(p->topics[0]) % interface.encoding) {
                    p->articulatedLog = CFunction(interface);
                    return decodeRLP2(interface.name, p->articulatedLog.inputs, params);
                }
            }
        }
        return false;
    }

    //-----------------------------------------------------------------------
    bool CAbi::articulateTrace(CTrace *p) const {

        if (!p)
            return false;

        if (p->action.input.length() >= 10 || p->action.input == "0x") {
            string_q encoding = extract(p->action.input, 0, 10);
            string_q params   = extract(p->action.input, 10);
            for (auto interface : interfaces) {
                if (encoding % interface.encoding) {
                    p->articulatedTrace = CFunction(interface);
                    return decodeRLP2(interface.name, p->articulatedTrace.inputs, params);
                }
            }
        }
        return false;
    }

    //-----------------------------------------------------------------------
    bool CAbi::articulateOutputs(const string_q& encoding, const string_q& params, CFunction& ret) const {
        for (auto interface : interfaces) {
            if (encoding % interface.encoding) {
                ret = CFunction(interface);
                return decodeRLP2(interface.name, ret.outputs, params);
            }
        }
        return true;
    }

// EXISTING_CODE
}  // namespace qblocks
