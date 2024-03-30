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
#include "utillib.h"
#include "options.h"

//------------------------------------------------------------------------------------------------------------
bool writeCodeIn(COptions* opts, const codewrite_t& cw) {
    if (contains(cw.fileName, "/other/data-models/")) {
        return true;
    }

    string_q codeOut = cw.codeOutIn;
    string_q orig;
    asciiFileToString(cw.fileName, orig);
    string_q existingCode = substitute(orig, "//EXISTING_CODE", "// EXISTING_CODE");

    string_q checkCode = existingCode;
    uint64_t cnt = 0;
    while (contains(checkCode, "// EXISTING_CODE")) {
        replace(checkCode, "// EXISTING_CODE", "");
        cnt++;
    }
    if ((cnt % 2)) {
        // ostringstream os;
        cerr << bRed << "Uneven number of EXISTING_CODE blocks in the file:" << endl;
        cerr << "file: " << cw.fileName << endl;
        exit(0);
    }
    if (cw.nSpaces) {
        replaceAll(existingCode, string_q(cw.nSpaces, ' '), "\t");
        replaceAll(codeOut, string_q(cw.nSpaces, ' '), "\t");
    }

    string_q tabs;
    int nTabs = 6;
    while (nTabs >= 0) {
        tabs = string_q((size_t)nTabs, '\t');
        nTabs--;
        //--------------------------------------------------------------------------------------
        while (contains(existingCode, tabs + "// EXISTING_CODE")) {
            replace(existingCode, tabs + "// EXISTING_CODE", "<code>");
            replace(existingCode, tabs + "// EXISTING_CODE", "</code>");
        }
        while (contains(existingCode, "</code>")) {
            string_q snip = snagFieldClear(existingCode, "code");
            bool dub = endsWith(snip, "\n\n");
            string_q snipit = trim(snip, '\n');
            string_q r1 = tabs + "// EXISTING_CODE\n" + tabs + "// EXISTING_CODE";
            string_q r2 = tabs + "// EXISTING_CODE\n" + snipit + "\n" + (dub ? "\n" : "") + tabs + "// EXISTING_CODE";
            replace(codeOut, r1, r2);
        }

        replaceAll(codeOut, "// EXISTING_CODE\n\n" + tabs + "// EXISTING_CODE",
                   "// EXISTING_CODE\n" + tabs + "// EXISTING_CODE");
        //--------------------------------------------------------------------------------------
    }

    // One final cleanup
    replaceAll(codeOut, "\n\n}", "\n}");
    replaceAll(codeOut, "\n\n\n", "\n\n");
    if (cw.stripEOFNL) {
        if (endsWith(codeOut, "\n"))
            replaceReverse(codeOut, "\n", "");
    }

    if (cw.nSpaces)
        replaceAll(codeOut, "\t", string_q(cw.nSpaces, ' '));
    replaceAll(codeOut, "[PTAB]", "\\t");

    if (contains(codeOut, "virtual") || contains(codeOut, "override")) {
        replace(codeOut, "~C", "virtual ~C");
        replace(codeOut, "~Q", "virtual ~Q");
        replace(codeOut, "::virtual ~C", "::~C");
        replace(codeOut, "::virtual ~Q", "::~Q");
    }

    return writeIfDifferent(cw.fileName, codeOut);
}

//---------------------------------------------------------------------------------------------------
bool writeCodeOut(COptions* opts, const string_q& fn) {
    if (contains(fn, "/other/data-models/")) {
        return true;
    }
    if (contains(fn, "/stub/"))
        return true;

    string_q orig = asciiFileToString(fn);
    string_q codeOut = orig;
    cerr << "Unknown file type for " << fn << endl;

    return writeIfDifferent(fn, codeOut);
}

#define docsRepoPath string_q("/Users/jrush/Development/trueblocks-docs/content/")
//------------------------------------------------------------------------------------------------------------
bool writeIfDifferent(const string_q& outFn, const string_q& orig) {
    stringToAsciiFile(outFn, orig);
    if (contains(outFn, "/docs/")) {
        string_q docsFn = substitute(outFn, "../content/", "/Users/jrush/Development/trueblocks-docs/content/");
        docsFn = substitute(docsFn, "../docs/", "/Users/jrush/Development/trueblocks-docs/content/");
        LOG_INFO("Writing: ", cTeal, docsFn, cOff);
        stringToAsciiFile(docsFn, orig);
    }
    return true;
}
