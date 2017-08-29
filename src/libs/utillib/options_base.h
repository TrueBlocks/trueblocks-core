#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "version.h"

namespace qblocks {

    class COptionsBase {
    public:
        static bool useVerbose;
        static bool needsOption;
        static bool isReadme;

        SFString commandList;
        bool     fromFile;
        SFUint32 minArgs;

        COptionsBase(void) { fromFile = false; minArgs = 1; isReadme = false; needsOption = false; }
        virtual ~COptionsBase(void) { }

        bool prepareArguments(int argc, const char *argv[]);
        virtual bool parseArguments(SFString& command) = 0;
        bool builtInCmd(const SFString& arg);
        virtual SFString postProcess(const SFString& which, const SFString& str) const { return str; }

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
    extern SFString purpose(void);

    //--------------------------------------------------------------------------------
    extern int sortParams(const void *c1, const void *c2);
    extern SFString expandOption(SFString& arg);

    //--------------------------------------------------------------------------------
    extern SFUint32 verbose;

    //--------------------------------------------------------------------------------
    extern void     editFile  (const SFString& fileName);
    extern SFString configPath(const SFString& part = "");
    inline SFString cachePath (const SFString& part = "") { return configPath("slurps/") + part; }

    //--------------------------------------------------------------------------------
    extern CParams *paramsPtr;
    extern uint32_t& nParamsRef;
    extern COptionsBase *pOptions;

    #define curVersion ((uint64_t)0x00000201)
}  // namespace qblocks
