#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

namespace qblocks {

    class COptionsBase {
    public:
        static SFString header;
        static SFString footer;
        static SFString seeAlso;
        static bool useVerbose;
        static bool useTesting;

        SFString commandList;
        bool     fromFile;
        SFUint32 minArgs;

        COptionsBase(void) { fromFile = false; minArgs = 1; }
        virtual ~COptionsBase(void) { }

        bool prepareArguments(int argc, const char *argv[]);
        virtual bool parseArguments(SFString& command) = 0;

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
        SFString  description;
        CParams(const SFString& name, const SFString& descr);
    };

    //--------------------------------------------------------------------------------
    extern int usage(const SFString& errMsg = "");
    extern SFString options(void);
    extern SFString descriptions(void);
    extern SFString purpose(void);

    //--------------------------------------------------------------------------------
    extern int sortParams(const void *c1, const void *c2);
    extern SFString expandOption(SFString& arg);

    //--------------------------------------------------------------------------------
    extern SFUint32 verbose;
    extern bool isTesting;

    //--------------------------------------------------------------------------------
    extern CFileExportContext outScreen;
    extern CFileExportContext& outErr;

    //--------------------------------------------------------------------------------
    extern SFString configPath(const SFString& part = "");
    inline SFString cachePath(const SFString& part = "") { return configPath("slurps/") + part; }
    
    //--------------------------------------------------------------------------------
    extern CParams *paramsPtr;
    extern uint32_t& nParamsRef;
}
