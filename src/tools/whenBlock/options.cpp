/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] = {
    CParams("~date / block", "one of the special values listed below or YYYY-MM-DD-[HH[:MM[:SS]]] or a blockNumber"),
    CParams("-alone",        "show the found block or found date unadorned (useful for scripting)"),
    CParams("-list",         "list the names and block numbers of special blocks"),
    CParams("", "Finds the nearest block prior to a JSON-formatted date, or the nearest date prior to\n"
                " the given block. Alternatively, search for one of the special blocks listed below.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

extern int sortByBlockNum(const void *v1, const void *v2);
extern SFTime parseDate(const SFString& strIn);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command) {

    bool isList = false;
    bool foundOne = false;
    Init();
    while (!command.empty()) {
        SFString arg = nextTokenClear(command, ' ');
        SFString orig = arg;

        if (arg == "UTC") {
            // do nothing

        } else if (arg == "-l" || arg == "--list") {
            isList = true;

        } else if (arg == "-a" || arg == "--alone") {

            alone = true;

        } else if (arg.ContainsAny(":- ") && !arg.startsWith("-")) {

            if (isList)
                return usage("The --list option must appear alone on the line. Quitting...");

            // If we're here, we better have a good date, assume we don't
            foundOne = false;
            SFString str = arg.Substitute(" ", ";").Substitute("-", ";").Substitute("_", ";")
                                .Substitute(":", ";").Substitute(";UTC", "");
            SFTime date = parseDate(str);
            if (date == earliestDate)
                return usage("Invalid date: '" + orig + "'. Quitting...");
            if (date < SFTime(2015,07,30,15,25,00)) { // first block was at 15:26:00
                cout << "The date you specified (";
                cout << cTeal << orig << cOff;
                cout << ") is before the first block. Quitting...\n";
                return false;
            }
            foundOne = true;
            requests[requests.getCount()] = "date:" + asString(toTimestamp(date));

        } else if (arg.startsWith('-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: '" + orig + "'. Quitting...");
            }

        } else {

            if (isList)
                return usage("The --list option must appear alone on the line. Quitting...");

            // if we're here, we better have a good block, assume we don't
            foundOne = false;
            for (uint32_t i = 0 ; i < specials.getCount() ; i++) {
                SFString special = specials[i].getName();
                SFString num     = specials[i].getValue();
                if (special == arg) {
                    requests[requests.getCount()] = "special:" + special + "|" + num;
                    foundOne = true;
                }
            }

            if (!foundOne) {
                if (isUnsigned(arg)) {
                    if (toUnsigned(arg) > getLatestBlockFromClient()) {
                        cout << "The block number you requested (";
                        cout << cTeal << orig << cOff;
                        cout << ") is after the latest block (";
                        cout << cTeal << (isTestMode() ? "TESTING" : asStringU(getLatestBlockFromClient())) << cOff;
                        cout << "). Quitting...\n";
                        return false;
                    }
                    requests[requests.getCount()] = "block:" + asStringU(toUnsigned(arg));
                    foundOne = true;
                } else {
                    return usage("Invalid argument: '" + orig + "'. Please supply either a JSON formatted date or a blockNumber. Quitting...");
                }
            }
        }
    }

    if (isList) {
        if (alone || requests.getCount())
            return usage("The --list option must appear alone on the line. Quitting...");
        cout << listSpecials(false);
        return false;
    }

    if (!foundOne)
        return usage("Please supply either a JSON formatted date or a blockNumber. Quitting...");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr  = params;
    nParamsRef = nParams;
    pOptions = this;

    requests.Clear();
    alone = false;
    loadSpecials();
}

//--------------------------------------------------------------------------------
int sortByBlockNum(const void *v1, const void *v2) {
    CNameValue *b1 = (CNameValue *)v1;  // NOLINT
    CNameValue *b2 = (CNameValue *)v2;  // NOLINT
    if (b1->getName() == "latest")
        return 1;
    if (b2->getName() == "latest")
        return -1;
    return (int)(b1->getValueU() - b2->getValueU());
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
SFString COptions::listSpecials(bool terse) const {
    ostringstream os;
    if (terse) {
        os << bYellow << "\n  Notes:\n\t" << cOff;
        os << "You may specify any of the following strings to represent 'special' blocks:\n\n\t    ";
    } else {
        os << bYellow << "\n\tSpecial Blocks:" << cOff;
    }

    SFString extra;
    for (uint32_t i = 0 ; i < specials.getCount(); i++) {

        SFString name  = specials[i].getName();
        SFString block = specials[i].getValue();
        if (name == "latest") {
            if (isTestMode()) {
                block = "";
            } else if (i > 0 && specials[i-1].getValueU() >= specials[i].getValueU()) {
                extra = iWhite + " (syncing)" + cOff;
            }
        }

        if (terse) {
            os << name;
            os << " (" << cTeal << block << extra << cOff << ")";
            if (i < specials.getCount()-1)
                os << ", ";
            if (!((i+1)%4))
                os << "\n\t    ";
        } else {
            os << "\n\t  " << padRight(name, 12) << cTeal << padLeft(block, 10) << cOff << extra ;
        }
    }
    if (terse) {
        if (specials.getCount() % 4)
            os << "\n";
    } else {
        os << "\n";
    }
    return os.str().c_str();
}

//--------------------------------------------------------------------------------
SFString COptions::postProcess(const SFString& which, const SFString& str) const {
    if (which == "description")
        return str + listSpecials(true);
    return str;
}

//--------------------------------------------------------------------------------
SFTime parseDate(const SFString& strIn) {

    if (strIn.empty()) {
        return earliestDate;
    }

    SFString str = strIn.Substitute(";", "");
    if (str.length() != 14 && str.length() != 8)
        return earliestDate;

    if (str.length() == 8)
        str += "120000";

#define NP ((uint32_t)-1)
    uint32_t y, m, d, h, mn, s;
    y = m = d = h = mn = s = NP;
    if (isUnsigned(str.substr( 0,4))) { y  = toLong32u(str.substr( 0, 4)); }
    if (isUnsigned(str.substr( 4,2))) { m  = toLong32u(str.substr( 4, 2)); }
    if (isUnsigned(str.substr( 6,2))) { d  = toLong32u(str.substr( 6, 2)); }
    if (isUnsigned(str.substr( 8,2))) { h  = toLong32u(str.substr( 8, 2)); }
    if (isUnsigned(str.substr(10,2))) { mn = toLong32u(str.substr(10, 2)); }
    if (isUnsigned(str.substr(12,2))) { s  = toLong32u(str.substr(12, 2)); }

    // If any of them was not an unsigned int, it's a fail
    if (y == NP || m == NP || d == NP || h == NP || mn == NP || s == NP)
        return earliestDate;

    return SFTime(y, m, d, h, mn, s);
}

extern const char *STR_DEFAULT_SPECIALS;
//-----------------------------------------------------------------------
void COptions::loadSpecials(void) {

    static CToml *toml = NULL;
    if (!toml) {
        static CToml theToml(configPath("quickBlocks.toml"));
        toml = &theToml;
    }
    specials.Clear();

    SFString specialsStr = toml->getConfigArray("specials", "list", "");
    if (specialsStr.empty()) {
        SFString in = asciiFileToString(configPath("quickBlocks.toml"));
        stringToAsciiFile(configPath("quickBlocks.toml"), in + "\n" + STR_DEFAULT_SPECIALS);
        specialsStr = toml->getConfigArray("specials", "list", "");
    }
    char *p = cleanUpJson((char *)specialsStr.c_str());
    while (p && *p) {
        CNameValue pair;
        uint32_t nFields = 0;
        p = pair.parseJson(p, nFields);
        if (nFields) {
            //cout << pair.Format() << "\n";
            if (pair.name == "latest") {
                pair.value = asStringU(getLatestBlockFromClient());
            }
            specials[specials.getCount()] = pair;
        }
    }

    specials.Sort(sortByBlockNum);
    return;
}

const char *STR_DEFAULT_SPECIALS =
"[[specials]]\n"
"list = [\n"
"\t{ name = \"first\",      value = \"0\"          },\n"
"\t{ name = \"iceage\",     value = \"200000\"     },\n"
"\t{ name = \"homestead\",  value = \"1150000\"    },\n"
"\t{ name = \"daofund\",    value = \"1428756\"    },\n"
"\t{ name = \"daohack\",    value = \"1718497\"    },\n"
"\t{ name = \"daofork\",    value = \"1920000\"    },\n"
"\t{ name = \"tangerine\",  value = \"2463000\"    },\n"
"\t{ name = \"spurious\",   value = \"2675000\"    },\n"
"\t{ name = \"stateclear\", value = \"2718436\"    },\n"
"\t{ name = \"latest\",     value = \"\"           }\n"
"]\n";
