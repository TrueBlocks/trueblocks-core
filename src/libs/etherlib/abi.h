#pragma once
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
#include "function.h"

namespace qblocks {

// EXISTING_CODE
class CTransaction;
class CLogEntry;
class CTrace;
typedef map<string, CFunction> CStringFunctionMap;
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAbi : public CBaseNode {
  public:
    address_t address;
    CStringFunctionMap interfaceMap;

  public:
    CAbi(void);
    CAbi(const CAbi& ab);
    virtual ~CAbi(void);
    CAbi& operator=(const CAbi& ab);

    DECLARE_NODE(CAbi);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    CStringBoolMap abiSourcesMap;

    bool articulateTransaction(CTransaction* p) const;
    bool articulateLog(CLogEntry* l) const;
    bool articulateTrace(CTrace* t) const;
    bool articulateOutputs(const string_q& encoding, const string_q& value, CFunction& ret) const;

    bool loadAbiFromEtherscan(const address_t& addr);
    bool loadAbiFromSolidity(const address_t& addr);
    bool loadAbisFromKnown(bool tokensOnly = false);
    bool loadAbisOneKnown(const string_q& addr);

    bool findInterface(const string_q& enc, CFunction& func) const;
    bool hasInterface(const string_q& enc) const;
    void addInterfaceToMap(const CFunction& func);
    void clearInterfaceMap(void);

    size_t nInterfaces(void) const;
    size_t nFunctions(void) const;
    size_t nEvents(void) const;
    size_t nOther(void) const;
    // EXISTING_CODE
    bool operator==(const CAbi& it) const;
    bool operator!=(const CAbi& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CAbi& v1, const CAbi& v2);
    friend ostream& operator<<(ostream& os, const CAbi& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAbi& ab);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
  private:
    bool loadAbiFromFile(const string_q& fileName);
    bool loadAbiFromAddress(const address_t& addr, bool recurse);
    bool loadAbiFromString(const string_q& str);
    void loadAbiAddInterface(const CFunction& func);
    friend bool loadAbiFile(const string_q& path, void* data);
    friend bool loadAbiString(const string_q& path, CAbi& abi);
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAbi::CAbi(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAbi::CAbi(const CAbi& ab) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ab);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAbi::~CAbi(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::clear(void) {
    // EXISTING_CODE
    interfaceMap.clear();
    abiSourcesMap.clear();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::initialize(void) {
    CBaseNode::initialize();

    address = "";
    interfaceMap.clear();

    // EXISTING_CODE
    abiSourcesMap.clear();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::duplicate(const CAbi& ab) {
    clear();
    CBaseNode::duplicate(ab);

    address = ab.address;
    interfaceMap = ab.interfaceMap;

    // EXISTING_CODE
    abiSourcesMap = ab.abiSourcesMap;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAbi& CAbi::operator=(const CAbi& ab) {
    duplicate(ab);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAbi::operator==(const CAbi& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return (address == it.address);
}

//-------------------------------------------------------------------------
inline bool operator<(const CAbi& v1, const CAbi& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAbi> CAbiArray;
extern CArchive& operator>>(CArchive& archive, CAbiArray& array);
extern CArchive& operator<<(CArchive& archive, const CAbiArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CAbi& abi);
extern CArchive& operator>>(CArchive& archive, CAbi& abi);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ABI;

//---------------------------------------------------------------------------
// EXISTING_CODE
extern bool fromDefinition(CFunction& func, const string_q& lineIn);
extern bool fromDefinition(CParameter& parm, const string_q& input);
extern bool decodeRLP(CParameterArray& ifaces, const string_q& desc, const string_q& input);
extern bool sortByFuncName(const CFunction& f1, const CFunction& f2);
bool isKnownAbi(const string_q& addr, string_q& path);
// EXISTING_CODE
}  // namespace qblocks
