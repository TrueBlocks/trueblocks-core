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
#include "filenames.h"
#include "toml.h"
#include "option.h"
#include "configenv.h"

namespace qblocks {

class COptionsBase {
  public:
    CStringArray commandLines;
    CStringArray arguments;
    CStringArray configs;
    CStringArray errors;

  public:
    COptionsBase(void);
    virtual ~COptionsBase(void);

    //--------------------------------------------------------------------------------
    static string_q g_progName;
    void setProgName(const string_q& name);
    string_q getProgName(void) const;
    virtual bool parseArguments(const string_q& command) = 0;
    inline bool usage(const string_q& errMsg = "") const {
        return false;
    }
    bool isRedirected(void) const;
    string_q getOutputFn(void) const {
        return rd_outputFilename;
    }

    vector<COption> parameters;
    virtual void Init(void) = 0;

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
    bool parseArguments(const string_q& command) {
        return true;
    }
    void Init(void) {
    }
};

extern uint64_t verbose;

//--------------------------------------------------------------------------------
class CToml;
extern const CToml* getGlobalConfig(const string_q& name);

inline bool listBlocks(uint64_t bn, void* data) {
    CUintArray* array = (CUintArray*)data;
    array->push_back(bn);
    return true;
}
extern bool prepareEnv(int argc, const char* argv[]);
inline bool isReserved(const string_q& command) {
    const char* STR_RESERVED =
        "|help|verbose|fmt|output|append|noop|version|nocolor|no_header|raw|"
        "wei|ether|cmd|api_mode|file|";
    return contains(STR_RESERVED, "|" + command + "|");
}

}  // namespace qblocks
