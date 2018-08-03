/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("-firstBlock:<num>", "the first block to visit (block zero by default)"),
    COption("-lastBlock:<num>",  "the last block to visit (to end of chain by default)"),
    COption("-report",           "report on the statistics without updating"),
    COption("",                  "Scan the miniBlock database for trace counts.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    blknum_t lastBlock = NOPOS;
    while (!command.empty()) {

        string_q arg = nextTokenClear(command, ' ');

        if (startsWith(arg, "-f") || startsWith(arg, "--firstBlock")) {

            arg = substitute(substitute(arg, "-f:", ""), "--firstBlock:", "");
            firstBlock = str_2_Uint(arg);

        } else if (startsWith(arg, "-l") || startsWith(arg, "--lastBlock")) {

            arg = substitute(substitute(arg, "-l:", ""), "--lastBlock:", "");
            lastBlock = str_2_Uint(arg);

        } else if (startsWith(arg, "-r") || startsWith(arg, "--report")) {

            report = true;

        } else {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    if (firstBlock == NOPOS) {
        firstBlock = 0;
    }

    if (lastBlock == NOPOS) {
        lastBlock  = getLatestBlockFromCache();
    }
    nBlocks = (lastBlock - firstBlock + 1);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr  = params;
    nParamsRef = nParams;

    report     = false;
    firstBlock = NOPOS;
    nBlocks    = 0;

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//-------------------------------------------------------------------------
void COptions::readData(void) {

#if 0
    string_q str = asciiFileToString("./data/transRes.txt");
    uint32_t lineNo = 0;
    while (!str.empty()) {
        string_q line = substitute(nextTokenClear(str, '\n'), "|", "\t");
        uint32_t colNo = 0;
        while (!line.empty()) {
            string_q val = nextTokenClear(line, '\t');
            uint32_t v = (uint32_t)str_2_Uint(val);
            transCount[lineNo][colNo++] = v;
        }
        cerr << ".";
        lineNo++;
    }
    cerr << "\n";

    firstBlock = min((uint64_t)NBLOCKS, (uint64_t)(lineNo * PERBUCKET));
    lastBlock  = NBLOCKS;

    str = asciiFileToString("./data/traceRes.txt");
    lineNo = 0;
    while (!str.empty()) {
        string_q line = substitute(nextTokenClear(str, '\n'), "|", "\t");
        uint32_t colNo = 0;
        while (!line.empty()) {
            string_q val = nextTokenClear(line, '\t');
            uint32_t v = (uint32_t)str_2_Uint(val);
            traceCount[lineNo][colNo++] = v;
        }
        cerr << ".";
        lineNo++;
    }
    cerr << "\n";
#endif
}

//-------------------------------------------------------------------------
void COptions::writeData(void) const {

#if 0
    string_q transRes, traceRes;
    for (uint32_t row = 0 ; row < ROWS ; row++) {
        for (uint32_t col = 0 ; col < COLS ; col++) {
            transRes += toStringU(transCount[row][col]);
            traceRes += toStringU(traceCount[row][col]);
            if (col < (COLS-1)) {
                transRes += "\t";
                traceRes += "\t";
            }
        }
        transRes +=  "\n";
        traceRes +=  "\n";
        cerr << ".";
        cerr.flush();
    }
    cerr << "\n";

    stringToAsciiFile("./data/transRes.txt", transRes);
    cout << transRes << "\n";

    stringToAsciiFile("./data/traceRes.txt", traceRes);
    cout << traceRes << "\n";
#endif
}
