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
#include "acctlib.h"
#include "options.h"

extern string_q replaceCode(const string_q& orig, const string_q& which, const string_q& new_code);
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
        ostringstream os;
        os << "Uneven number of EXISTING_CODE blocks in the file." << endl;
        os << "file: " << cw.fileName << endl;
        os << "replaced: " << cnt << " instances of // EXISTING_CODE" << endl;
        os << "Original: " << orig << endl;
        codeOut = os.str() + codeOut;
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

    opts->counter.nVisited++;
    if (writeIfDifferent(cw.fileName, codeOut)) {
        opts->counter.nProcessed++;
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
bool writeCodeOut(COptions* opts, const string_q& fn) {
    if (contains(fn, "/other/data-models/")) {
        return true;
    }
    if (contains(fn, "/stub/") || goPortNewCode(fn))
        return true;

    string_q orig = asciiFileToString(fn);
    string_q codeOut = orig;
    if (endsWith(fn, ".cpp")) {
        CStringArray tokens = {"_CODE_AUTO", "_CODE_OPTIONS", "_CODE_LOCAL_INIT",
                               "_CODE_INIT", "_CODE_NOTES",   "ERROR_STRINGS"};

        for (auto tok : tokens) {
            bool missing = !contains(orig, tok);
            bool ported =
                contains(orig, "_CHIFRA") || contains(fn, "flame") || contains(fn, "when") || contains(fn, "daemon");
            if (missing && !ported) {
                LOG_WARN(fn, " does not contain token ", tok);
            }
        }

        codeOut = replaceCode(codeOut, "CODE_AUTO", opts->autoStream.str());
        codeOut = replaceCode(codeOut, "CODE_OPTIONS", opts->optionStream.str());
        codeOut = replaceCode(codeOut, "CODE_LOCAL_INIT", opts->localStream.str());
        codeOut = replaceCode(codeOut, "CODE_INIT", opts->initStream.str());
        codeOut = replaceCode(codeOut, "CODE_NOTES", opts->notesStream.str());
        codeOut = replaceCode(codeOut, "ERROR_STRINGS", opts->errorStrStream.str());
        replaceAll(codeOut, "    // clang-format on\n    // clang-format off\n", "");

    } else if (endsWith(fn, ".go.tmpl") || endsWith(fn, ".go")) {
        codeOut = replaceCode(codeOut, "ROUTE_PKGS", trim(opts->goPkgStream.str(), '\n') + "\n");
        codeOut = replaceCode(codeOut, "ROUTE_CODE", opts->goCallStream.str());
        codeOut = replaceCode(codeOut, "ROUTE_ITEMS", opts->goRouteStream.str());
        codeOut = replaceCode(codeOut, "CONVERT_CODE", opts->goConvertStream.str());

    } else if (endsWith(fn, ".yaml")) {
        string_q components = trim(asciiFileToString(getDocsPathTemplates("api/components.txt")), '\n');
        string_q descr = asciiFileToString(getDocsPathTemplates("api/description.txt"));
        replaceAll(descr, "~~~~", "    ");

        codeOut = asciiFileToString(getPathToTemplates("blank.yaml"));
        replace(codeOut, "[{TAGS}]", opts->apiTagStream.str());
        replace(codeOut, "[{PATHS}]", opts->apiPathStream.str());
        replace(codeOut, "[{DESCRIPTION}]", descr);
        replace(codeOut, "[{COMPONENTS}]", components);
        replace(codeOut, "[{VERSION}]", getVersionStr(false /* product */, false /* git_hash */));

    } else if (endsWith(fn, ".h")) {
        CStringArray tokens = {"ERROR_DEFINES", "_CODE_DECLARE"};
        for (auto tok : tokens) {
            bool missing = !contains(orig, tok);
            bool ported =
                contains(orig, "_CHIFRA") || contains(fn, "flame") || contains(fn, "when") || contains(fn, "daemon");
            if (missing && !ported) {
                LOG_WARN(fn, " does not contain token ", tok);
            }
        }
        codeOut = replaceCode(codeOut, "CODE_DECLARE", opts->headerStream.str());
        codeOut = replaceCode(codeOut, "ERROR_DEFINES", opts->errorDefStream.str());

    } else if (endsWith(fn, "Routes.tsx")) {
        codeOut = replaceCode(codeOut, "CODE_LOCATIONS", opts->jsLocationStream.str());
        // codeOut = replaceCode(codeOut, "CODE_TEMPLATES", jsTemplateStream.str());
        codeOut = replaceCode(codeOut, "CODE_ROUTES", opts->jsRouteStream.str());
        codeOut = replaceCode(codeOut, "CODE_KEYS", opts->jsHotkeyStream.str());

    } else {
        cerr << "Unkown file type for " << fn << endl;
    }

    opts->counter.nVisited++;
    if (writeIfDifferent(fn, codeOut)) {
        opts->counter.nProcessed++;
        return true;
    }
    return false;
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
bool writeIfDifferent(const string_q& outFn, const string_q& orig) {
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
