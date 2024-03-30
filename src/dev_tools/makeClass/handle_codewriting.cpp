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

extern string_q replaceCode(const string_q& orig, const string_q& which, const string_q& new_code);
extern size_t countSections(const string_q& in);
//------------------------------------------------------------------------------------------------------------
bool writeCodeIn(COptions* opts, const codewrite_t& cw) {
    if (contains(cw.fileName, "/other/data-models/")) {
        return true;
    }

    string_q codeOut = cw.codeOutIn;
    string_q orig;
    asciiFileToString(cw.fileName, orig);
    size_t nSections = countSections(orig);
    if ((nSections % 2)) {
        // ostringstream os;
        cerr << bRed << "Uneven number of EXISTING_CODE blocks in the file:" << endl;
        cerr << "file: " << cw.fileName << endl;
        exit(0);
    }

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

//--------------------------------------------------------------------------------
string_q getDocsRepoPathContent(const string_q& _part) {
#define docsRepoPath string_q("/Users/jrush/Development/trueblocks-docs/content/")
    return docsRepoPath + _part;
}

//------------------------------------------------------------------------------------------------------------
bool isDifferent(const string_q& fn, const string_q& codeIn) {
    string_q orig = substitute(codeIn, "date: $DATE\n", "");
    string_q existingIn = asciiFileToString(fn);
    CStringArray lines;
    explode(lines, existingIn, '\n', false);
    ostringstream existing;
    for (auto line : lines) {
        if (!startsWith(line, "date: ")) {
            existing << line << endl;
        }
    }
    return trim(existing.str(), '\n') != trim(orig, '\n');
}

//------------------------------------------------------------------------------------------------------------
void finalCleaning(const string_q& outFn) {
    string_q contents = asciiFileToString(outFn);
    replaceAll(contents, "\n\n\n", "\n\n");
    stringToAsciiFile(outFn, trim(contents, '\n') + "\n\n");
}

//------------------------------------------------------------------------------------------------------------
bool writeIfDifferent(const string_q& outFn, const string_q& orig) {
    if (!contains(outFn, ".yaml")) {
        finalCleaning(outFn);
    }
    if (!isDifferent(outFn, orig)) {
        return false;
    }

    time_q now = Now();
    string_q timedCode = substitute(orig, "$DATE", now.Format(FMT_EXPORT));  // May not hit which is okay

    LOG_INFO("Writing: ", cTeal, outFn, cOff);
    stringToAsciiFile(outFn, timedCode);

    if (contains(outFn, "/docs/")) {
        string_q docsFn = substitute(outFn, getDocsPathContent(""), getDocsRepoPathContent(""));
        docsFn = substitute(docsFn, "../docs/", getDocsRepoPathContent(""));
        LOG_INFO("Writing: ", cTeal, docsFn, cOff);
        stringToAsciiFile(docsFn, timedCode);
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
string_q replaceCode(const string_q& orig, const string_q& which, const string_q& new_code) {
    string_q codeOut = orig;
    codeOut = substitute(codeOut, "// BEG_" + which, "// BEG_" + which + "\n[{NEW_CODE}]\n<remove>");
    codeOut = substitute(codeOut, "\n// END_" + which, "</remove>\nX// END_" + which);
    codeOut = substitute(codeOut, "\n\t// END_" + which, "</remove>\nY// END_" + which);
    codeOut = substitute(codeOut, "\n    // END_" + which, "</remove>\n+// END_" + which);
    codeOut = substitute(codeOut, "\n        // END_" + which, "</remove>\n-// END_" + which);
    codeOut = substitute(codeOut, "\n            // END_" + which, "</remove>\n-// END_" + which);
    snagFieldClear(codeOut, "remove");
    replace(codeOut, "[{NEW_CODE}]\n\n", new_code);
    replaceAll(codeOut, "X//", "//");
    replaceAll(codeOut, "Y//", "\t//");
    replaceAll(codeOut, "+//", "    //");
    replaceAll(codeOut, "-//", "            //");
    return codeOut;
}

//------------------------------------------------------------------------------------------------------------
void expandTabbys(string_q& strOut) {
    replaceAll(strOut, "`````", string_q(5, '\t'));
    replaceAll(strOut, "````", string_q(4, '\t'));
    replaceAll(strOut, "```", string_q(3, '\t'));
    replaceAll(strOut, "``", string_q(2, '\t'));
    replaceAll(strOut, "`", string_q(1, '\t'));
}

//------------------------------------------------------------------------------------------------------------
size_t countSections(const string_q& in) {
    string_q inIn = substitute(in, "the code inside of 'EXISTING_CODE' tags", "");
    CStringArray lines;
    explode(lines, inIn, '\n');
    size_t cnt = 0;
    for (auto line : lines) {
        if (contains(line, "EXISTING_CODE")) {
            cnt++;
        }
    }
    return cnt;
}
