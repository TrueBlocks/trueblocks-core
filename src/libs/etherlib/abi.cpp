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

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["abi_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

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
    // only do this once
    if (HAS_FIELD(CAbi, "schema")) return;

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
        return forEveryFileInFolder(configPath("known_abis/""*"), visitABI, this);
    return loadAbiFromFile(configPath("known_abis/" + which + ".json"), true);
}

//---------------------------------------------------------------------------
bool CAbi::loadAbiByAddress(address_t addrIn) {
    if (isZeroAddr(addrIn))
        return false;
    string_q addr = toLower(addrIn);
    string_q fileName = getCachePath("abis/" + addr + ".json");
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

    if (isZeroAddr(addr))
        return false;

    string_q results;
    string_q fileName = getCachePath("abis/" + addr + ".json");

    string_q localFile("./" + addr + ".json");
    if (fileExists(localFile) && localFile != fileName) {
        cerr << "Local file copied to cache\n";
        copyFile(localFile, fileName);
    }

    string_q dispName = substitute(fileName, getCachePath(""), "$BLOCK_CACHE/");
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
                if (isContractAt(addr)) {
                    cerr << "\n";
                    cerr << cRed << "Warning: " << cOff;
                    cerr << "Failed to grab the ABI. Etherscan returned:\n\n\t";
                    cerr << cTeal << results << cOff << "\n\n";
                    cerr << "However, the ABI may actually be present on EtherScan. QBlocks will use it if\n";
                    cerr << "you copy and paste the ABI json to this file:\n\n\t";
                    cerr << cTeal << localFile << cOff << "\n\n";
                    quickQuitHandler(0);
                } else {
                    cerr << "\n";
                    cerr << cRed << "Warning: " << cOff;
                    cerr << "The address you specified (" << addr << ") does not\n";
                    cerr << "\t does not appear to be a smart contract, therefore there is no ABI. Quitting...\n\n";
                    quickQuitHandler(0);
                }
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

//-----------------------------------------------------------------------------------------
string_q params_2_Str(CParameterArray& interfaces) {
    string_q ret;
    for (auto p : interfaces) {
        if (!ret.empty())
            ret += ", ";
        ret += p.value;
    }
    return trim(ret);
}

//------------------------------------------------------------------------------------------------
void loadType(CParameterArray& ar, const string_q& type) {
    CParameter p;
    p.type = type;
    ar.push_back(p);
}

//------------------------------------------------------------------------------------------------
size_t decodeTheData(CParameterArray& interfaces, const CStringArray& dataArray, size_t& readIndex) {

    for (size_t q = 0 ; q < interfaces.size() ; q++) {

        CParameter *pPtr = &interfaces[q];
        string_q type = pPtr->type;

        bool isBaseType = (type.find("[") == string::npos);
        if (isBaseType) {

            if (type.find("bool") != string::npos) {

                // sugar
                CParameterArray tmp;
                loadType(tmp, "uint256");
                decodeTheData(tmp, dataArray, readIndex);
                pPtr->value = ((tmp[0].value == "1") ? "true" : "false");

            } else if (type.find("address") != string::npos) {

                // sugar
                CParameterArray tmp;
                loadType(tmp, "uint160");
                decodeTheData(tmp, dataArray, readIndex);
                pPtr->value = "0x" + padLeft(toLower(bnu_2_Hex(str_2_BigUint(tmp[0].value))), 40, '0');

            } else if (type.find("uint") != string::npos) {

                size_t bits = str_2_Uint(substitute(type, "uint", ""));
                pPtr->value = bnu_2_Str(str_2_BigUint("0x" + dataArray[readIndex++], bits));

            } else if (type.find("int") != string::npos) {

                size_t bits = str_2_Uint(substitute(type, "int", ""));
                pPtr->value = bni_2_Str(str_2_BigInt("0x" + dataArray[readIndex++], bits));

            } else if (type == "string" || type == "bytes") {

                // Strings and bytes are dynamic sized. The fixed size part resides at readIndex and points to
                // start of string. Start of string is length of string. Start of string + 1 is the string
                string_q result;
                uint64_t dataStart = (str_2_Uint("0x" + dataArray[readIndex++]) / 32);
                uint64_t nBytes = str_2_Uint("0x" + dataArray[dataStart]);
                size_t nWords = (nBytes / 32) + 1;
                if (nWords <= dataArray.size()) { // some of the data sent in may be bogus, so we protext ourselves
                    for (size_t w = 0 ; w < nWords ; w++) {
                        size_t pos = dataStart + 1 + w;
                        if (pos < dataArray.size())
                            result += dataArray[pos].substr(0, nBytes * 2);  // at most 64
                        nBytes -= 32;
                    }
                }
                pPtr->value = (type == "string" ? hex_2_Str("0x" + result) : "0x" + result);

            } else if (type.find("bytes") != string::npos) {

                // bytes1 through bytes32 are fixed length
                pPtr->value = "0x" + dataArray[readIndex++];

            } else {

                cerr << "Unknown type: " << type << " in decodeTheData" << endl;

            }

        } else {

            ASSERT(!isBaseType);
            if (type.find("[") == type.find("[]")) { // the first bracket is a dynamic array

                size_t found = type.find('[');
                string baseType = type.substr(0, found);
                if (baseType == "bytes") baseType = "bytes32";
                if (found + 2 != type.size())
                    baseType += type.substr(found + 2);

                uint64_t dataStart = (str_2_Uint("0x" + dataArray[readIndex++]) / 32);
                uint64_t nItems = str_2_Uint("0x" + dataArray[dataStart]);

                CParameterArray tmp;
                for (size_t i = 0; i < nItems; i++)
                    loadType(tmp, baseType);

                size_t tPtr = dataStart + 1;
                decodeTheData(tmp, dataArray, tPtr);
                pPtr->value = "[" + params_2_Str(tmp) + "]";

            } else if (type.find("]") != string::npos) {

                //int tempPointer;
                /*
                 if(interfaces.size() != 1) {
                 // Find offset pointing to "real values" of dynamic array
                 uint64_t dataStart = str_2_Uint(dataArray[readIndex]);
                 tempPointer = dataStart / 32;
                 } else {
                 tempPointer = readIndex;
                 }
                 */
                size_t found1 = type.find('[');
                size_t found2 = type.find(']');
                string subType = type.substr(0, found1);
                if (found2 + 1 != type.size())
                    subType += type.substr(found2 + 1);

                int nBytes = stoi(type.substr(type.find('[')+1, type.find(']')));
                CParameterArray tmp;
                for (int i = 0; i < nBytes; i++)
                    loadType(tmp, subType);
                decodeTheData(tmp, dataArray, readIndex);
                pPtr->value = "[" + params_2_Str(tmp) + "]";
                //readIndex++;
            }
        }
    }
    return 1;
}

//-----------------------------------------------------------------------------------------
size_t extractParams(CParameterArray& paramArray, const string_q& paramStr) {

    string_q str = substitute(substitute(paramStr, "(", "|"), ")", "|");

    CStringArray parts;
    explode(parts, str, '|');

    CStringArray strArray;
    explode(strArray, parts[1], ',');

    for (auto p : strArray) {

        // remove extraneous spaces or tabs
        string type = trim(substitute(p, "\t", " "), ' ');
        while (contains(type, " ["))
            type = substitute(type, " [", "[");
        while (contains(type, " ]"))
            type = substitute(type, " ]", "]");

        // strip field names
        if (type.find(" ") != string::npos)
            type = type.substr(0, type.find(" "));

        // clean up syntactic sugar
        if (type == "int") type = "int256";
        if (type == "uint") type = "uint256";
        if (type == "fixed") type = "fixed128x128";
        if (type == "ufixed") type = "ufixed128x128";
        if (startsWith(type, "int[")) type = substitute(type, "int[", "int256[");
        if (startsWith(type, "uint[")) type = substitute(type, "uint[", "uint256[");
        if (startsWith(type, "fixed[")) type = substitute(type, "fixed[", "fixed128x128[");
        if (startsWith(type, "ufixed[")) type = substitute(type, "ufixed[", "ufixed128x128[");

        loadType(paramArray, type);
    }
    return paramArray.size();
}

//---------------------------------------------------------------------------
bool decodeRLP(CParameterArray& interfaces, const string_q& inputStr) {

    // Clean up the input to work with the provided interfaces - one input row per 32 bytes
    CStringArray inputs;
    if (inputStr.empty() || inputStr == "0x") {
        // In the case where the input is empty, we fill up all the fixed sized slots with zero bytes
        for (auto i : interfaces)
            inputs.push_back(string_q(64, '0'));
    } else {
        // Put each 32-byte segment into it's own string in the array
        string_q str = trim(substitute(inputStr, "0x", ""), ' ');
        ASSERT(!(str.size() % 64));
        while (!str.empty()) {
            string_q item = str.substr(0,64);
            ASSERT(items.size() == 64);
            replace(str, item, "");
            inputs.push_back(item);
        }
    }

    size_t offset = 0;
    return decodeTheData(interfaces, inputs, offset);
}

//-----------------------------------------------------------------------
bool CAbi::articulateTransaction(CTransaction *p) const {

    if (!p)
        return false;

    // articulate the events, so we can return with a fully articulated object
    for (auto& log : p->receipt.logs)
        articulateLog(&log);

    // articulate the traces, so we can return with a fully articulated object
    bool hasTraces = false;
    for (auto& trace : p->traces) {
        hasTraces = true;
        trace.articulatedTrace.m_showing = false;
        ((CAbi*)this)->loadAbiByAddress(trace.action.to);
        if (articulateTrace(&trace))
            trace.articulatedTrace.m_showing = true;
    }

    if (p->input.length() >= 10 || p->input == "0x") {
        string_q encoding = extract(p->input, 0, 10);
        string_q input    = extract(p->input, 10);
        for (auto interface : interfaces) {
            if (encoding % interface.encoding) {
                p->articulatedTx = CFunction(interface);
                p->articulatedTx.showOutput = false;
                bool ret1 = decodeRLP(p->articulatedTx.inputs, input);
                bool ret2 = (hasTraces ? decodeRLP(p->articulatedTx.outputs, p->traces[0].result.output) : false);
                return (ret1 || ret2);
            }
        }

extern bool isPrintable(const string_q& inHex);
        if (isPrintable(p->input))
            p->articulatedTx.message = hex_2_Str(p->input);
    }
    return false;
}

//-----------------------------------------------------------------------
inline bool sortByPosition(const CParameter& v1, const CParameter& v2) {
    return v1.pos < v2.pos;
}

//-----------------------------------------------------------------------
bool CAbi::articulateLog(CLogEntry *p) const {

    if (!p)
        return false;

    // From solidity documentation:
    // For all fixed-length Solidity types, the EVENT_INDEXED_ARGS array contains the 32-byte encoded value directly.
    // However, for types of dynamic length, which include string, bytes, and arrays, EVENT_INDEXED_ARGS will contain
    // the Keccak hash of the packed encoded value (see Strict Encoding Mode), rather than the encoded value directly.
    // This allows applications to efficiently query for values of dynamic-length types (by setting the hash of the
    // encoded value as the topic), but leaves applications unable to decode indexed values they have not queried for.
    // For dynamic-length types, application developers face a trade-off between fast search for predetermined values
    // (if the argument is indexed) and legibility of arbitrary values (which requires that the arguments not be
    // indexed). Developers may overcome this tradeoff and achieve both efficient search and arbitrary legibility
    // by defining events with two arguments — one indexed, one not — intended to hold the same value.
    //
    // Upshot: Topics are always fixed length. They can be parsed as fixed length. The data field needs to be parsed
    // as potentially variable length RLP encoded. Since the indexed event args and the non-indexed event args can
    // be interleaved in the function interface, we need to decode them separately
    // TODO: SEARCH FOR ISSUE #1013

    CAbi *ncABI = (CAbi*)this;
    for (size_t i = 0 ; i < ncABI->interfaces.size() ; i++) {
        CFunction *intf = &ncABI->interfaces[i];
        if (topic_2_Str(p->topics[0]) % intf->encoding) {
            // we found the event we're looking for, make a copy
            p->articulatedLog = CFunction(*intf);
            p->articulatedLog.showOutput = false;

            // separate out the topic params from the data params
            string_q topicStr; size_t which = 1, pos = 0;
            CParameterArray topics, data;
            for (auto in : p->articulatedLog.inputs) {
                if (in.indexed && which < p->topics.size()) {
                    in.pos = pos++;
                    topics.push_back(in);
                    topicStr += topic_2_Str(p->topics[which++]);
                } else {
                    in.pos = pos++;
                    data.push_back(in);
                }
            }

            // decode each separately
            bool ret1 = decodeRLP(topics, topicStr);
            bool ret2 = decodeRLP(data, p->data);

            // put them back together in the right order and we're done
            p->articulatedLog.inputs.clear();
            for (auto a : topics)
                p->articulatedLog.inputs.push_back(a);
            for (auto a : data)
                p->articulatedLog.inputs.push_back(a);
            sort(p->articulatedLog.inputs.begin(), p->articulatedLog.inputs.end(), sortByPosition);
            return (ret1 || ret2);
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
        string_q input    = extract(p->action.input, 10);
        for (auto interface : interfaces) {
            if (encoding % interface.encoding) {
                p->articulatedTrace = CFunction(interface);
                p->articulatedTrace.showOutput = false;
                bool ret1 = decodeRLP(p->articulatedTrace.inputs, input);
                bool ret2 = decodeRLP(p->articulatedTrace.outputs, p->result.output);
                return (ret1 || ret2);
            }
        }
    }
    return false;
}

//-----------------------------------------------------------------------
bool CAbi::articulateOutputs(const string_q& encoding, const string_q& output, CFunction& ret) const {
    for (auto interface : interfaces) {
        if (encoding % interface.encoding) {
            ret = CFunction(interface);
            ret.showOutput = false;
            return decodeRLP(ret.outputs, output);
        }
    }
    return true;
}
// EXISTING_CODE
}  // namespace qblocks
