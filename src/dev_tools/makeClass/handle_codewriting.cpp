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

#define forceWrite false
extern string_q replaceCode(const string_q& orig, const string_q& which, const string_q& new_code);
//------------------------------------------------------------------------------------------------------------
bool writeCodeIn(const codewrite_t& cw) {
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

    bool testing = isTestMode();
    if (codeOut != orig || forceWrite) {
        // Do the actual writing of the data only if we're not testing or the user has told us not to
        if (!testing) {
            LOG_INFO("Writing: ", cTeal, cw.fileName, cOff);
            stringToAsciiFile(cw.fileName, codeOut);
        } else {
            LOG8("Not writing: ", cw.fileName);
        }
        // We return 'true' if we WOULD HAVE have written the file (even if we didn't).
        return true;
    }

    // We return 'false' if we would NOT have written the file (not if we actually did).
    return false;
}

//---------------------------------------------------------------------------------------------------
bool writeCodeOut(COptions* opts, const string_q& fn) {
    if (contains(fn, "/stub/") || goPortNewCode(fn))
        return true;

    string_q orig = asciiFileToString(fn);
    string_q converted = orig;
    if (endsWith(fn, ".cpp")) {
        CStringArray tokens = {"_CODE_AUTO", "_CODE_OPTIONS", "_CODE_LOCAL_INIT",
                               "_CODE_INIT", "_CODE_NOTES",   "ERROR_STRINGS"};

        for (auto tok : tokens)
            if (!contains(orig, tok) && !contains(orig, "_CHIFRA") && !contains(fn, "flame") && !contains(fn, "when"))
                LOG_WARN(fn, " does not contain token ", tok);

        converted = replaceCode(converted, "CODE_AUTO", opts->autoStream.str());
        converted = replaceCode(converted, "CODE_OPTIONS", opts->optionStream.str());
        converted = replaceCode(converted, "CODE_LOCAL_INIT", opts->localStream.str());
        converted = replaceCode(converted, "CODE_INIT", opts->initStream.str());
        converted = replaceCode(converted, "CODE_NOTES", opts->notesStream.str());
        converted = replaceCode(converted, "ERROR_STRINGS", opts->errorStrStream.str());
        converted = replaceCode(converted, "CODE_CHIFRA_CMDMAP", opts->chifraCmdStream.str());
        converted = replaceCode(converted, "CODE_CHIFRA_PAIRMAP", opts->pairMapStream.str());
        replaceAll(converted, "    // clang-format on\n    // clang-format off\n", "");

    } else if (endsWith(fn, ".go.tmpl") || endsWith(fn, ".go")) {
        converted = replaceCode(converted, "ROUTE_PKGS", trim(opts->goPkgStream.str(), '\n') + "\n");
        converted = replaceCode(converted, "ROUTE_CODE", opts->goCallStream.str());
        converted = replaceCode(converted, "ROUTE_ITEMS", opts->goRouteStream.str());
        converted = replaceCode(converted, "CONVERT_CODE", opts->goConvertStream.str());

    } else if (endsWith(fn, ".yaml")) {
        string_q components = trim(asciiFileToString(getDocsPathTemplates("api/components.txt")), '\n');
        string_q descr = asciiFileToString(getDocsPathTemplates("api/description.txt"));
        replaceAll(descr, "~~~~", "    ");

        converted = asciiFileToString(getPathToTemplates("blank.yaml"));
        replace(converted, "[{TAGS}]", opts->apiTagStream.str());
        replace(converted, "[{PATHS}]", opts->apiPathStream.str());
        replace(converted, "[{DESCRIPTION}]", descr);
        replace(converted, "[{COMPONENTS}]", components);
        replace(converted, "[{VERSION}]", getVersionStr(false /* product */, false /* git_hash */));

    } else if (endsWith(fn, ".h")) {
        CStringArray tokens = {"ERROR_DEFINES", "_CODE_DECLARE"};
        for (auto tok : tokens)
            if (!contains(orig, tok) && !contains(fn, "flame") && !contains(fn, "when"))
                LOG_WARN(fn, " does not contain token ", tok);
        converted = replaceCode(converted, "CODE_DECLARE", opts->headerStream.str());
        converted = replaceCode(converted, "ERROR_DEFINES", opts->errorDefStream.str());

    } else if (endsWith(fn, "Routes.tsx")) {
        converted = replaceCode(converted, "CODE_LOCATIONS", opts->jsLocationStream.str());
        // converted = replaceCode(converted, "CODE_TEMPLATES", jsTemplateStream.str());
        converted = replaceCode(converted, "CODE_ROUTES", opts->jsRouteStream.str());
        converted = replaceCode(converted, "CODE_KEYS", opts->jsHotkeyStream.str());

    } else {
        cerr << "Unkown file type for " << fn << endl;
    }

    ostringstream out;
    out << "Writing: " << cTeal << fn << " " << cOff;
    opts->counter.nVisited++;
    if (converted != orig || forceWrite) {
        LOG_INFO(out.str(), "wrote ", cGreen, converted.size(), " bytes...", cOff);
        stringToAsciiFile(fn, converted);
        opts->counter.nProcessed++;
        return true;
    }
    cerr << cTeal << "no changes..." << cOff << "\r";
    cerr.flush();
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool writeIfDifferent(const string_q& outFn, const string_q& codeIn, const time_q& now) {
    string_q orig = substitute(codeIn, "date: $DATE\n", "");
    string_q existingIn = asciiFileToString(outFn);
    CStringArray lines;
    explode(lines, existingIn, '\n', false);
    ostringstream existing;
    for (auto line : lines) {
        if (!startsWith(line, "date: ")) {
            existing << line << endl;
        }
    }

    if (existing.str() != orig || forceWrite) {
        string_q out = substitute(codeIn, "$DATE", now.Format(FMT_EXPORT));
        stringToAsciiFile(outFn, out);
        LOG_INFO("Writing: ", cTeal, outFn, cOff);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool writeIfDifferent(const string_q& outFn, const string_q& orig) {
    string_q existing = asciiFileToString(outFn);
    if (existing != orig || forceWrite) {
        stringToAsciiFile(outFn, orig);
        LOG_INFO("Writing: ", cTeal, outFn, cOff);
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
string_q replaceCode(const string_q& orig, const string_q& which, const string_q& new_code) {
    string_q converted = orig;
    converted = substitute(converted, "// BEG_" + which, "// BEG_" + which + "\n[{NEW_CODE}]\n<remove>");
    converted = substitute(converted, "\n// END_" + which, "</remove>\nX// END_" + which);
    converted = substitute(converted, "\n\t// END_" + which, "</remove>\nY// END_" + which);
    converted = substitute(converted, "\n    // END_" + which, "</remove>\n+// END_" + which);
    converted = substitute(converted, "\n        // END_" + which, "</remove>\n-// END_" + which);
    converted = substitute(converted, "\n            // END_" + which, "</remove>\n-// END_" + which);
    snagFieldClear(converted, "remove");
    replace(converted, "[{NEW_CODE}]\n\n", new_code);
    replaceAll(converted, "X//", "//");
    replaceAll(converted, "Y//", "\t//");
    replaceAll(converted, "+//", "    //");
    replaceAll(converted, "-//", "            //");
    return converted;
}

//------------------------------------------------------------------------------------------------------------
void expandTabbys(string_q& strOut) {
    replaceAll(strOut, "`````", string_q(5, '\t'));
    replaceAll(strOut, "````", string_q(4, '\t'));
    replaceAll(strOut, "```", string_q(3, '\t'));
    replaceAll(strOut, "``", string_q(2, '\t'));
    replaceAll(strOut, "`", string_q(1, '\t'));
}
