#pragma once
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
#include "version.h"
#include "accountname.h"
#include "filenames.h"
#include "toml.h"
#include "exportcontext.h"

// Bit flags to enable / disable various options
#define OPT_DESCRIPTION (0)
#define OPT_HELP (1 << 1)
#define OPT_VERBOSE (1 << 2)
#define OPT_FMT (1 << 3)
#define OPT_DOLLARS (1 << 4)
#define OPT_WEI (1 << 5)
#define OPT_ETHER (1 << 6)
#define OPT_PARITY (1 << 7)
#define OPT_RAW (1 << 11)
#define OPT_PREFUND (1 << 12)
#define OPT_OUTPUT (1 << 13)
#define OPT_EDITCMD (1 << 20)
#define OPT_MOCKDATA (1 << 21)
#define OPT_DENOM (OPT_DOLLARS | OPT_WEI | OPT_ETHER)
#define OPT_DEFAULT (OPT_HELP | OPT_VERBOSE | OPT_FMT | OPT_DENOM | OPT_PARITY | OPT_MOCKDATA)

#define OPT_REQUIRED (1 << 14)
#define OPT_POSITIONAL (1 << 15)
#define OPT_FLAG (1 << 16)
#define OPT_SWITCH OPT_FLAG
#define OPT_TOGGLE OPT_SWITCH
#define OPT_HIDDEN (1 << 17)
#define OPT_SKIP (OPT_HIDDEN | (1 << 18))

typedef map<size_t, qblocks::string_q> CErrorStringMap;
#define ERR_NOERROR 0

//-----------------------------------------------------------------------------
namespace qblocks {

typedef bool (*NAMEFUNC)(CAccountName& name, void* data);
typedef bool (*NAMEVALFUNC)(CNameValue& pair, void* data);
typedef bool (*UINT64VISITFUNC)(uint64_t num, void* data);
typedef uint64_t (*HASHFINDFUNC)(const hash_t& hash, void* data);

class COption;
class COptionsBase {
  public:
    CErrorStringMap errStrs;
    addr_wei_mp prefundWeiMap;
    CStringArray arguments;
    CStringArray errors;
    // TODO(tjayrush): global data
    uint32_t enableBits;
    bool isReadme;
    bool isRaw;
    bool isVeryRaw;
    bool mockData;
    string_q editCmd;
    bool isNoHeader;
    blkrange_t scanRange;
    CStringArray notes;

    bool isRedirected(void) const;

  private:
    streambuf* coutSaved;   // saves original cout buffer
    ofstream outputStream;  // the redirected stream (if any)
    string_q outputFilename;
    void closeRedirect(void);
    bool zipOnClose;

  public:
    CStringArray commandLines;
    uint64_t minArgs;
    CRuntimeClass* sorts[5];

    COptionsBase(void);
    virtual ~COptionsBase(void);

    //--------------------------------------------------------------------------------
    static string_q g_progName;
    void setProgName(const string_q& name);
    string_q getProgName(void) const;
    bool prepareArguments(int argc, const char* argv[]);
    virtual bool parseArguments(string_q& command) = 0;
    bool builtInCmd(const string_q& arg);
    bool standardOptions(string_q& cmdLine);

    // supporting special block names
    static CNameValueArray specials;
    static void loadSpecials(void);
    static bool findSpecial(CNameValue& pair, const string_q& arg);
    static bool forEverySpecialBlock(NAMEVALFUNC func, void* data);

    // supporting named accounts
    CAccountNameArray namedAccounts;
    bool getNamedAccount(CAccountName& acct, const string_q& addr);
    bool loadNames(void);
    string_q findNameByAddress(const string_q& addr);
    bool forEveryNamedAccount(NAMEFUNC func, void* data);

    // enabling options
    bool isEnabled(uint32_t q) const;
    void optionOff(uint32_t q);
    void optionOn(uint32_t q);

    string_q expandOption(string_q& arg);
    bool usage(const string_q& errMsg = "") const;
    string_q usageStr(const string_q& errMsg = "") const;
    string_q purpose(void) const;
    string_q options(void) const;
    string_q descriptions(void) const;
    string_q oneDescription(const string_q& sN, const string_q& lN, const string_q& d, bool isMode = false,
                            bool required = false) const;
    string_q get_notes(void) const;

    bool confirmEnum(const string_q& name, string_q& value, const string_q& arg) const;
    bool confirmBlockNum(const string_q& name, blknum_t& value, const string_q& arg, blknum_t latest) const;
    bool confirmUint(const string_q& name, uint64_t& value, const string_q& arg) const;
    inline bool confirmUint(const string_q& name, uint32_t& value, const string_q& arg) const {
        value = (uint32_t)NOPOS;
        uint64_t temp;
        if (!confirmUint(name, temp, arg))
            return false;
        value = (uint32_t)temp;
        return true;
    }
    const COption* findParam(const string_q& name) const;
    void setSorts(CRuntimeClass* c1, CRuntimeClass* c2, CRuntimeClass* c3);

  protected:
    void configureDisplay(const string_q& tool, const string_q& dataType, const string_q& defFormat,
                          const string_q& meta = "");
    void registerOptions(size_t nP, COption const* pP);
    virtual void Init(void) = 0;
    const COption* pParams;
    size_t cntParams;
    string_q hiUp1;
    string_q hiUp2;
    string_q hiDown;
};

//--------------------------------------------------------------------------------
class CDefaultOptions : public COptionsBase {
  public:
    CDefaultOptions() {
    }
    bool parseArguments(string_q& command) {
        return true;
    }
    void Init(void) {
    }
};

//--------------------------------------------------------------------------------
class COption {
  public:
    string_q shortName;
    string_q longName;
    string_q description;
    string_q permitted;
    string_q type;
    bool is_hidden;
    bool is_positional;
    bool is_optional;
    bool is_skip;
    COption(const string_q& ln, const string_q& sn, const string_q& type, size_t opts, const string_q& d);
};

//--------------------------------------------------------------------------------
extern int sortParams(const void* c1, const void* c2);

//--------------------------------------------------------------------------------
extern uint64_t verbose;

//--------------------------------------------------------------------------------
extern void editFile(const string_q& fileName);
extern string_q configPath(const string_q& part);
extern string_q configPathRelative(const string_q& part);

//--------------------------------------------------------------------------------
class CToml;
extern const CToml* getGlobalConfig(const string_q& name = "");

class COptionsBlockList {
  public:
    CBlockNumArray numList;
    CStringArray hashList;
    HASHFINDFUNC hashFind;
    blknum_t start;
    blknum_t stop;
    blknum_t skip;
    blknum_t latest;
    bool hasZeroBlock{false};

    void Init(void);
    string_q parseBlockList(const string_q& arg, blknum_t latest);
    COptionsBlockList(void);
    bool forEveryBlockNumber(UINT64VISITFUNC func, void*) const;
    bool hasBlocks(void) const {
        return (hashList.size() || numList.size() || (start != stop));
    }
    bool isInRange(blknum_t bn) const;
    blknum_t parseBlockOption(string_q& msg, blknum_t lastBlock, direction_t offset, bool& hasZero) const;
};

class COptionsTransList {
  public:
    string_q queries;
    void Init(void);
    string_q parseTransList(const string_q& arg);
    COptionsTransList(void);
    string_q int_2_Str(void) const;
    bool hasTrans(void) const {
        return !queries.empty();
    }
};

extern bool prepareEnv(int argc, const char* argv[]);
extern string_q cleanFmt(const string_q& str);
extern void errorMessage(const string_q& msg);

}  // namespace qblocks
