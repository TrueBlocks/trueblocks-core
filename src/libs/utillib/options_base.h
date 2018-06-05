#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
#include "namevalue.h"
#include "accountname.h"
#include "filenames.h"
#include "toml.h"

// Bit flags to enable / disable various options
#define OPT_VERBOSE (1<<2)
#define OPT_DOLLARS (1<<3)
#define OPT_WEI     (1<<4)
#define OPT_ETHER   (1<<5)
#define OPT_DENOM   (OPT_DOLLARS|OPT_WEI|OPT_ETHER)
#define OPT_PARITY  (1<<6)
#define OPT_BLOCKS  (1<<8)
#define OPT_TRANS   (1<<10)
#define OPT_ADDRS   (1<<12)
#define OPT_DEFAULT (OPT_VERBOSE|OPT_DENOM|OPT_BLOCKS|OPT_ADDRS|OPT_PARITY)

namespace qblocks {

    class COptionsBase {
    public:
        static uint32_t enableBits;
        static bool needsOption;
        static bool isReadme;

        SFString commandList;
        bool     fromFile;
        uint64_t minArgs;
        CRuntimeClass *sorts[5];

        COptionsBase(void);
        virtual ~COptionsBase(void) { }

        bool prepareArguments(int argc, const char *argv[]);
        virtual bool parseArguments(SFString& command) = 0;
        bool builtInCmd(const SFString& arg);
        bool standardOptions(SFString& cmdLine);
        virtual SFString postProcess(const SFString& which, const SFString& str) const { return str; }

        // supporting special block names
        CNameValueArray specials;
        void     loadSpecials(void);
        bool     findSpecial(CNameValue& pair, const SFString& arg) const;

        // supporting named accounts
        CAccountNameArray namedAccounts;
        CFilename namesFile;
        bool loadNames(void);
        bool getNamedAccount(CAccountName& acct, const SFString& addr) const;

    protected:
        virtual void Init(void) = 0;
    };

    //--------------------------------------------------------------------------------
    class CDefaultOptions : public COptionsBase {
    public:
        CDefaultOptions() {}
        bool parseArguments(SFString& command) { return true; }
        void Init(void) {}
    };

    //--------------------------------------------------------------------------------
    class CParams {
    public:
        SFString  shortName;
        SFString  longName;
        SFString  hotKey;
        SFString  description;
        SFString  permitted;
        CParams(const SFString& name, const SFString& descr);
    };

    //--------------------------------------------------------------------------------
    extern int usage(const SFString& errMsg = "");
    extern SFString usageStr(const SFString& errMsg = "");
    extern SFString options(void);
    extern SFString descriptions(void);
    extern SFString notes(void);
    extern SFString purpose(void);

    //--------------------------------------------------------------------------------
    extern int sortParams(const void *c1, const void *c2);
    extern SFString expandOption(SFString& arg);

    //--------------------------------------------------------------------------------
    extern uint64_t verbose;

    //--------------------------------------------------------------------------------
    extern void     editFile  (const SFString& fileName);
    extern SFString configPath(const SFString& part);

    //--------------------------------------------------------------------------------
    extern CParams *paramsPtr;
    extern uint32_t& nParamsRef;
    extern COptionsBase *pOptions;

    extern bool isEnabled(uint32_t q);
    extern void optionOff(uint32_t q);
    extern void optionOn (uint32_t q);

    //--------------------------------------------------------------------------------
    extern const CToml *getGlobalConfig(const SFString& name="");

    typedef bool (*UINT64VISITFUNC)(uint64_t num, void *data);
    typedef uint64_t (*HASHFINDFUNC)(const SFHash& hash, void *data);
    class COptionsBlockList {
    public:
        SFBlockArray numList;
        SFStringArray hashList;
        HASHFINDFUNC hashFind;
        blknum_t start;
        blknum_t stop;
        blknum_t latest;
        void Init(void);
        SFString parseBlockList(const SFString& arg, blknum_t latest);
        COptionsBlockList(void);
        bool forEveryBlockNumber(UINT64VISITFUNC func, void *) const;
        bool hasBlocks(void) const { return (hashList.getCount() || numList.getCount() || (start != stop)); }
        bool isInRange(blknum_t bn) const;
        blknum_t parseBlockOption(SFString& msg, blknum_t lastBlock) const;
    };

    class COptionsTransList {
    public:
        SFString queries;
        void Init(void);
        SFString parseTransList(const SFString& arg);
        COptionsTransList(void);
        SFString toString(void) const;
        bool hasTrans(void) const { return !queries.empty(); }
    };

    extern const char *STR_DEFAULT_DATA;
}  // namespace qblocks
