//IT SHOULD NOT START IF IT CANNOT FIND THE FILE.
//I REMOVED AN ADDRESS ENTRY, THE FIRST, WHICH CHANGED THE NAME OF THE FILE
//SHOULD DISALLOW WEIRD COMMAND LINE options
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("-fmt:<fmt>",       "export format (one of [json|txt|csv]"),
    COption("-fi(l)ter:<addr>", "show results for this address (you may specify more than one filter)"),
    COption("-useBlooms",       "use bloom filters to decide whether or not to re-check the cache"),
    COption("-ignoreDdos",      "ignore apparent dDos transactions."),
    COption("",                 "Export transactions for one or more Ethereum addresses.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    CAccountWatch::registerClass();

    export_t fmt = JSON;
    CStringArray filters;
    if (!standardOptions(command))
        return false;

    Init();
#ifdef OUTPUT_REDIR
    outFile = "file.txt";
#endif
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (startsWith(arg, "-f:") || startsWith(arg, "--fmt:")) {

            arg = substitute(substitute(arg, "-f:", ""), "--fmt:", "");
                 if ( arg == "txt" ) fmt = TXT;
            else if ( arg == "csv" ) fmt = CSV;
            else if ( arg == "json") fmt = JSON;
            else return usage("Export format must be one of [ json | txt | csv ]. Quitting...");

        } else if (startsWith(arg, "-l:") || startsWith(arg, "--filter:")) {
            arg = substitute(substitute(arg, "-l:", ""), "--filter:", "");
            filters.push_back(arg);

        } else if (arg == "-i" || arg == "--ignoreDdos") {
            ignoreDdos = true;

        } else if ((arg == "-u") || (arg == "--useBlooms")) {
            useBloom = true;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    if (!fileExists("./config.toml"))
        return usage("The config.toml file was not found. Are you in the right folder? Quitting...\n");

    // show certain fields and hide others
    manageFields(defHide, false);
    manageFields(defShow, true);

    CToml toml("./config.toml");
    manageFields(toml.getConfigStr("fields", "hide", ""), false);
    manageFields(toml.getConfigStr("fields", "show", ""), true );

    if (!loadWatches(toml))
        return false;

    if (filters.size() > 0) {
        for (CAccountWatch& watch : watches) {
            watch.enabled = false;
            for (auto addr : filters)
                if (addr % watch.address)
                    watch.enabled = true;
        }
    }

#ifdef OUTPUT_REDIR
    if (!outFile.empty()) {
        outStream.open(outFile);
        out = new COutputPipe(outStream.rdbuf(), cout);
    }
#endif

    transFmt = "";  // empty string gets us JSON output
    if (fmt != JSON) {
        string_q format = toml.getConfigStr("formats", "trans_fmt", "");
        if (format.empty())
            return usage("Non-json export requires 'trans_fmt' string in config.toml. Quitting...");
        transFmt = cleanFmt(format);
        if (fmt == CSV)
            transFmt = "\"" + substitute(transFmt, "\t", "\",\"") + "\"";
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    arguments.clear();
    paramsPtr = params;
    nParamsRef = nParams;

    transFmt = "";
    blk_minWatchBlock = 0;
    blk_maxWatchBlock = UINT32_MAX;
    showProgress = getGlobalConfig()->getConfigBool("debug", "showProgress", false);
    useBloom = false;
    ignoreDdos = true;
    needsArt = false;
    needsTrace = false;

    minArgs = 0;
#ifdef OUTPUT_REDIR
    if (out)
        delete out;
    out = NULL;
    outFile = "";
    outStream.close();
#endif
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
#ifdef OUTPUT_REDIR
    out = NULL;
#endif
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
#ifdef OUTPUT_REDIR
    if (out)
        delete out;
    outStream.close();
#endif
}

//-----------------------------------------------------------------------
string_q cleanFmt(const string_q& str) {
    return (substitute(substitute(substitute(str, "\n", ""), "\\n", "\n"), "\\t", "\t"));
}
