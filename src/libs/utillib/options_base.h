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
#define OPT_CRUD (1 << 14)
#define OPT_MOCKDATA (1 << 21)
#define OPT_DENOM (OPT_DOLLARS | OPT_WEI | OPT_ETHER)
#define OPT_DEFAULT (OPT_HELP | OPT_VERBOSE | OPT_FMT | OPT_DENOM | OPT_PARITY | OPT_MOCKDATA | OPT_OUTPUT)

#define OPT_REQUIRED (1 << 14)
#define OPT_POSITIONAL (1 << 15)
#define OPT_FLAG (1 << 16)
#define OPT_SWITCH OPT_FLAG
#define OPT_TOGGLE OPT_SWITCH
#define OPT_HIDDEN (1 << 17)
#define OPT_DEPRECATED (OPT_HIDDEN | (1 << 18))
#define NOOPT ((uint32_t)-1)

#define ERR_NOERROR 0

//-----------------------------------------------------------------------------
namespace qblocks {

class COption;

//-----------------------------------------------------------------------------
typedef bool (*NAMEFUNC)(CAccountName& name, void* data);
typedef bool (*NAMEVALFUNC)(CNameValue& pair, void* data);
typedef bool (*UINT64VISITFUNC)(uint64_t num, void* data);
typedef uint64_t (*HASHFINDFUNC)(const hash_t& hash, void* data);
typedef map<address_t, CAccountName> CAddressNameMap;

//-----------------------------------------------------------------------------
class COptionsBase {
  public:
    CStringArray commandLines;
    CStringArray arguments;
    CStringArray notes;
    CStringArray configs;
    CErrorStringMap usageErrs;
    CStringArray errors;

    // TODO(tjayrush): global data
    uint64_t minArgs;
    uint32_t enableBits;
    bool isReadme;
    bool isRaw;
    bool isVeryRaw;
    bool noHeader;
    bool mocked;
    bool firstOut;
    bool freshenOnly;
    string_q overrideStr;

  public:
    COptionsBase(void);
    virtual ~COptionsBase(void);

    //--------------------------------------------------------------------------------
    static string_q g_progName;
    void setProgName(const string_q& name);
    string_q getProgName(void) const;

    virtual bool parseArguments(string_q& command) = 0;
    bool prepareArguments(int argc, const char* argv[]);
    bool prePrepareArguments(CStringArray& separatedArgs_, int argCountIn, const char* argvIn[]);
    bool builtInCmd(const string_q& arg);
    bool standardOptions(string_q& cmdLine);
    bool confirmEnum(const string_q& name, string_q& value, const string_q& arg) const;
    bool confirmBlockNum(const string_q& name, blknum_t& value, const string_q& arg, blknum_t latest) const;
    bool confirmUint(const string_q& name, uint64_t& value, const string_q& arg) const;
    bool confirmDouble(const string_q& name, double& value, const string_q& arg) const;
    bool confirmUint(const string_q& name, uint32_t& value, const string_q& arg) const;

    // supporting special block names
    static CNameValueArray specials;
    static void loadSpecials(void);
    static bool findSpecial(CNameValue& pair, const string_q& arg);
    static bool forEverySpecialBlock(NAMEVALFUNC func, void* data);

    // One of --create, --update, --delete, --undelete, --remove for use anywhere
    CStringArray crudCommands;
    bool isCrudCommand(void) const {
        return crudCommands.size() > 0;
    }

  protected:
    // supporting named accounts
    // TODO(tjayrush): All of these can (and should) be moved to expContext as it would be available to things other
    // TODO(tjayrush): than options. See fmtMap and tsMemMap for examples
    CAddressNameMap tokenMap;
    CAddressBoolMap airdropMap;
    bool buildOtherMaps(void);

  public:
    CAddressNameMap namesMap;
    bool loadNamesDatabaseFromSQL(void);
    bool loadNames(void);
    bool findName(const string_q& addr, CAccountName& acct);

    // enabling options
    bool isEnabled(uint32_t q) const;
    void optionOff(uint32_t q);
    void optionOn(uint32_t q);

    // usage related
    bool usage(const string_q& errMsg = "") const;
    bool flag_required(const string_q& command) const;
    bool invalid_option(const string_q& arg) const;
    string_q usageStr(const string_q& errMsg = "") const;
    string_q purpose(void) const;
    string_q options(void) const;
    string_q descriptions(void) const;
    string_q descriptionOverride(void) const;
    string_q get_notes(void) const;
    string_q format_notes(const CStringArray& strs) const;
    string_q get_configs(void) const;
    string_q format_configs(const CStringArray& strs) const;

    bool findParam(const string_q& name, COption& paramOut) const;
    string_q expandOption(string_q& arg);
    bool isBadSingleDash(const string_q& arg) const;

    bool isRedirected(void) const;
    string_q getOutputFn(void) const {
        return rd_outputFilename;
    }

    bool findToken(CAccountName& acct, const address_t& addr);

    void configureDisplay(const string_q& tool, const string_q& dataType, const string_q& defFormat,
                          const string_q& meta = "");

  protected:
    vector<COption> opts;
    string_q hiUp1;
    string_q hiUp2;
    string_q hiDown;

    virtual void Init(void) = 0;
    virtual bool Mocked(const string_q& which);
    void registerOptions(size_t nP, const COption* pP, uint32_t on = NOOPT, uint32_t off = NOOPT);

  private:
    streambuf* coutSaved;   // saves original cout buffer
    ofstream outputStream;  // the redirected stream (if any)
    void closeRedirect(void);
    string_q rd_outputFilename;
    bool rd_zipOnClose;
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
    string_q hotKey;
    string_q longName;
    string_q description;
    string_q permitted;
    string_q type;
    bool is_hidden;
    bool is_positional;
    bool is_optional;
    bool is_deprecated;
    COption(void) : is_hidden(false), is_positional(false), is_optional(false), is_deprecated(false) {
    }
    COption(const string_q& ln, const string_q& sn, const string_q& type, size_t opts, const string_q& d);
    bool isPublic(void) const {
        return (!is_hidden && !is_deprecated && !longName.empty());
    }
    string_q readmeDash(const string_q& str, bool isReadme) const;
    string_q getHotKey(bool isReadme) const;
    string_q getLongKey(bool isReadme) const;
    string_q getDescription(bool isReadme) const;
    string_q oneDescription(bool isReadme, size_t* widths) const;
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

inline bool listBlocks(uint64_t bn, void* data) {
    CUintArray* array = (CUintArray*)data;
    array->push_back(bn);
    return true;
}
class COptionsBlockList {
  public:
    CBlockNumArray numList;
    CStringArray hashList;
    HASHFINDFUNC hashFind;
    blknum_t start;
    blknum_t stop;
    blknum_t skip;
    blknum_t latest;
    period_t skip_type;
    bool hasZeroBlock{false};

    void Init(void);
    string_q parseBlockList_inner(const string_q& arg, blknum_t latest);
    COptionsBlockList(void);
    bool forEveryBlockNumber(UINT64VISITFUNC func, void*) const;
    bool empty(void) const {
        return !(hashList.size() || numList.size() || (start != stop));
    }
    size_t nItems(void) const {
        return hashList.size() + numList.size() + (start - stop);
    }
    size_t size(void) const {
        CUintArray nums;
        forEveryBlockNumber(listBlocks, &nums);
        return nums.size();
    }
    uint64_t operator[](size_t offset) const {
        CUintArray nums;
        forEveryBlockNumber(listBlocks, &nums);
        return nums[offset];
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
    bool empty(void) const {
        return queries.empty();
    }
    size_t size(void) const {
        return countOf(queries, '|');
    }
    string_q operator[](size_t offset) const {
        CStringArray parts;
        explode(parts, queries, '|');
        return parts[offset];
    };
};

extern bool prepareEnv(int argc, const char* argv[]);
extern string_q cleanFmt(const string_q& str);
extern void errorMessage(const string_q& msg);
inline bool isReserved(const string_q& command) {
    const char* STR_RESERVED =
        "|help|verbose|fmt|output|noop|version|nocolor|no_header|very_raw|raw|"
        "wei|ether|dollars|parity|cmd|mocked|api_mode|to_file|file|";
    return contains(STR_RESERVED, "|" + command + "|");
}
extern map<string_q, string_q> progNameMap;

}  // namespace qblocks
