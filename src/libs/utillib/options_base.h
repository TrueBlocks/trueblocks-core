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
#include "option.h"

#define ERR_NOERROR 0

//-----------------------------------------------------------------------------
namespace qblocks {

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
    CStringArray overrides;
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
    string_q get_header(void) const;
    string_q get_purpose(void) const;
    string_q get_description(void) const;
    string_q get_notes(void) const;
    string_q get_configs(void) const;
    string_q get_version(void) const;
    string_q get_options(void) const;
    string_q get_errmsg(const string_q& errMsg) const;
    string_q get_override(void) const;
    string_q get_positionals(COptionArray& pos) const;

    string_q format_notes(const CStringArray& strs) const;
    bool flag_required(const string_q& command) const;
    bool invalid_option(const string_q& arg) const;

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
    vector<COption> parameters;

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
extern int sortParams(const void* c1, const void* c2);

//--------------------------------------------------------------------------------
extern uint64_t verbose;

//--------------------------------------------------------------------------------
extern string_q getConfigPath(const string_q& part);
extern string_q getConfigPathRel(const string_q& part);

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

extern string_q colorize(const string_q& strIn);
extern bool prepareEnv(int argc, const char* argv[]);
extern string_q cleanFmt(const string_q& str);
extern void errorMessage(const string_q& msg);
inline bool isReserved(const string_q& command) {
    const char* STR_RESERVED =
        "|help|verbose|fmt|output|noop|version|nocolor|no_header|raw|"
        "wei|ether|dollars|cmd|mocked|api_mode|to_file|file|";
    return contains(STR_RESERVED, "|" + command + "|");
}

extern string_q path_2_Cmd(const string_q& path);
extern string_q cmd_2_Path(const string_q& cmd);

}  // namespace qblocks
