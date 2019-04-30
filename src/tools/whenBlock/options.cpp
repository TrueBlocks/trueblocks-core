/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <string>
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("~!block",    "one or more block numbers (or a 'special' block), or..."),
    COption("~!date",     "one or more dates formatted as YYYY-MM-DD[THH[:MM[:SS]]]"),
    COption("-data",      "display the result as data (tab delimited; useful for scripting)"),
    COption("-list",      "list names and block numbers for special blocks"),
    COption("-fmt:<fmt>", "export format (one of [json|txt|csv])"),
    COption("",           "Finds the nearest block prior to a date, or the nearest date prior to a block.\n"
                          " Alternatively, search for one of special 'named' blocks.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_DISPLAY;
extern const char* STR_DATA;
extern bool containsAny(const string_q& haystack, const string_q& needle);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    ENTER("parseArguments");
    if (!standardOptions(command))
        return false;

    export_t fmt = NONE;
    string_q format = STR_DISPLAY;
    bool isList = false;
    bool foundOne = false;
    Init();
    blknum_t latestBlock = getLastBlock_client();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        if (arg == "UTC") {
            // do nothing

        } else if (arg == "-l" || arg == "--list") {
            isList = true;

        } else if (arg == "-d" || arg == "--data") {
            asData = true;
            colorsOff();

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--fmt:")) {
            arg = substitute(substitute(arg, "-f:", ""), "--fmt:", "");
            if ( arg == "txt" ) fmt = TXT;
            else if ( arg == "csv" ) fmt = CSV;
            else if ( arg == "json") fmt = JSON;
            else EXIT_USAGE("Export format must be one of [ json | txt | csv ].");

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                EXIT_USAGE("Invalid option: '" + orig + "'.");
            }

        } else if (containsAny(arg, ":- ") && countOf(arg, '-') > 1) {

            ASSERT(!startsWith(arg, "-"));
            if (isList)
                EXIT_USAGE("The --list option must appear alone on the line.");

            time_q date = str_2_Date(arg);
            if (date == earliestDate) {
                EXIT_USAGE("Invalid date: '" + orig + "'.");

            } else if (date > Now()) {
                ostringstream os;
                os << "The date you specified (" << cTeal << orig << cOff << ")";
                os << "is in the future. No such block.";
                EXIT_FAIL(os.str());

            } else if (date < time_q(2015, 7, 30, 15, 25, 00)) {
                ostringstream os;
                os << "The date you specified (" << cTeal << orig << cOff << ")";
                os << "is before the first block.";
                EXIT_FAIL(os.str());
            }

            foundOne = true;
            requests.push_back("date:" + int_2_Str(date_2_Ts(date)));

        } else {

            if (isList)
                EXIT_USAGE("The --list option must appear alone on the line.");

            // if we're here, we better have a good block, assume we don't
            CNameValue spec;
            if (findSpecial(spec, arg)) {
                string_q val = spec.second;
                if (spec.first == "latest")
                    val = uint_2_Str(getLastBlock_client());
                requests.push_back("special:" + spec.first + "|" + val);
                foundOne = true;

            } else  {

                string_q ret = blocks.parseBlockList(arg, latestBlock);
                if (endsWith(ret, "\n")) {
                    EXIT_FAIL(substitute(ret,"\n",""));

                } else if (!ret.empty()) {
                    EXIT_USAGE(ret);
                }

                // Now we transfer the list of blocks to the requests array
                string_q blockList = getBlockNumList();  // get the list from blocks
                blocks.Init();  // clear out blocks
                CStringArray blks;
                explode(blks, blockList, '|');
                for (auto blk : blks) {
                    requests.push_back("block:" + blk);
                    foundOne = true;
                }
            }
        }
    }

    switch (fmt) {
        case NONE: break;
        case JSON: format = ""; break;
        case TXT:
        case CSV:  format = getGlobalConfig()->getConfigStr("display", "format", STR_DATA); break;
    }
    expContext().fmtMap["nick"] = cleanFmt(format, fmt);

    if (isList) {
        if (requests.size())
            EXIT_USAGE("The --list option must appear alone on the line.");
        cout << listSpecials(false, fmt);
        EXIT_NOMSG(false);
    }

    if (!foundOne)
        EXIT_USAGE("Please supply either a JSON formatted date or a blockNumber.");

    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    requests.clear();
    asData = false;
    optionOff(OPT_DENOM);
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);

    // Upgrade the configuration file by opening it, fixing the data, and then re-writing it
    CToml toml(configPath("whenBlock.toml"));

    // versions prior to 0.6.0
    if (toml.isBackLevel()) {
        string_q ss = toml.getConfigStr("specials", "list", "");
        if (!contains(ss, "kitties")) {
            toml.setConfigArray("specials", "list", STR_DEFAULT_WHENBLOCKS);
            toml.writeFile();
            getGlobalConfig("whenBlock");
        }
    }

    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {

    if (which == "options") {
        return substitute(str, "block date", "< block | date > [ block... | date... ]");

    } else if (which == "notes") {
        string_q ret = str;
        if (verbose || COptions::isReadme) {
            ret += "Add custom special blocks by editing ~/.quickBlocks/whenBlock.toml.\n";
        }
        ret += listSpecials(true, NONE);
        return ret;
    }
    return str;
}

//--------------------------------------------------------------------------------
bool getBlock_light(CBlock& block, blknum_t num) {
//    LOG2(getBinaryCacheFilename(CT_BLOCKS, num));
    if (fileSize(getBinaryCacheFilename(CT_BLOCKS, num)) > 0) {
//        LOG2("Reading");
        return readBlockFromBinary(block, getBinaryCacheFilename(CT_BLOCKS, num));
    }
//    LOG2("Querying");
    getObjectViaRPC(block, "eth_getBlockByNumber", "["+quote(uint_2_Hex(num))+",false]");
    return true;
}

//--------------------------------------------------------------------------------
string_q COptions::listSpecials(bool terse, export_t fmt) const {
    if (specials.size() == 0)
        ((COptionsBase *)this)->loadSpecials();  // NOLINT

    if (fmt == JSON) {
        extern const char* STR_KNOWN_BLOCKS;
        return substitute(STR_KNOWN_BLOCKS, "+", "\"");
/*
        HIDE_ALL_FIELDS(CBlock);
        SHOW_FIELD(CBlock, "hash");
        SHOW_FIELD(CBlock, "parentHash");
        SHOW_FIELD(CBlock, "blockNumber");
        SHOW_FIELD(CBlock, "price");
        SHOW_FIELD(CBlock, "timestamp");
        SHOW_FIELD(CBlock, "date");
        SHOW_FIELD(CBlock, "finalized");
        SHOW_FIELD(CBlock, "transactionsCnt");
        CBlock latest;
        getBlock(latest, "latest");
        map<blknum_t, string_q> theMap;
        CStringArray lines;
        for (auto special : specials) {
            string_q t1 = TIC();
            string_q name = special.first;
            blknum_t bn = str_2_Uint(special.second);
            CBlock block;
            getBlock(block, bn);
            if (!fileExists(getBinaryCacheFilename(CT_BLOCKS, bn))) {
                block.finalized = isBlockFinal(block.timestamp, latest.timestamp);
                writeBlockToBinary(block, getBinaryCacheFilename(CT_BLOCKS, bn));
            }
            block.parentHash = name;
            ostringstream os;
            os << block << endl;
            string_q s = substitute(os.str(), "parentHash", "name");
            //LOG1("Fethcing block #: " + uint_2_Str(bn));
            theMap.insert(pair<blknum_t, string_q>(bn, string_q(s)));
//            cerr << bn << ":" << t1 << ":" << TIC() << endl;
        }
        ostringstream os;
        for (auto item : theMap)
            os << item.second << ",";
//        map<blknum_t, string_q>::iterator it = theMap.begin();
//        while (it != theMap.end()) {
//            os << it->second;
//            it++;
//        }
        return "[" + trim(os.str(),',') + "]";
*/
    }

    ostringstream os;
    if (!asData) {
        if (terse) {
            os << "Use the following names to represent `special` blocks:\n  ";
        } else {
            os << bYellow << "\n  Blocks:" << cOff;
        }
    }

    string_q extra;
    for (size_t i = 0 ; i < specials.size(); i++) {

        string_q name = specials[i].first;
        string_q bn = specials[i].second;
        if (name == "latest") {
            bn = uint_2_Str(getLastBlock_client());
            if (isTestMode()) {
                bn = "";
            } else if (COptionsBase::isReadme) {
                bn = "--";
            } else if (i > 0 && str_2_Uint(specials[i-1].second) >= getLastBlock_client()) {
                extra = iWhite + " (syncing)" + cOff;
            }
        }

#define N_PER_LINE 4
        if (asData && !terse) {
            if (!contains(bn, "tbd"))
                os << bn << " ";
        } else {
            if (terse) {
                os << name;
                os << " (" << cTeal << bn << extra << cOff << ")";
                if (!((i+1) % N_PER_LINE)) {
                    if (i < specials.size()-1)
                        os << "\n  ";
                } else if (i < specials.size()-1) {
                    os << ", ";
                }
            } else {
                os << "\n      - " << padRight(name, 15);
                os << cTeal << padLeft(bn, 9) << cOff;
                if (verbose) {
                    CBlock block;
                    getBlock(block, bn);
                    block.timestamp = (block.blockNumber == 0 ? 1438269973 : block.timestamp);
                    if (block.timestamp != 0) {
                        UNHIDE_FIELD(CBlock, "date");
                        os << block.Format(" [{DATE}] ([{TIMESTAMP}])");
                    }
                }
                os << extra;
            }
        }
    }
    if (terse) {
        if (specials.size() % N_PER_LINE)
            os << "\n";
    } else {
        os << "\n";
    }
    return os.str().c_str();
}

//---------------------------------------------------------------------------------------
bool containsAny(const string_q& haystack, const string_q& needle) {
    string need = needle.c_str();
    for (const auto elem : need)
        if (contains(haystack, elem))
            return true;
    return false;
}

//-----------------------------------------------------------------------
const char* STR_DATA =
"";
const char* STR_DISPLAY =
"block #[{BLOCKNUMBER}][ : {TIMESTAMP}][ : {DATE}]";
const char* STR_KNOWN_BLOCKS =
"[{"\
" +hash+: +0xd4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8fa3+,"\
" +blockNumber+: 0,"\
" +name+: +first+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 0,"\
" +date+: +1970-01-01 00:00:00 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x4e3a3754410177e6937ef1f84bba68ea139e8d1a2258c5f85db9f1cd715a1bdd+,"\
" +blockNumber+: 46147,"\
" +name+: +firstTrans+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1438918233,"\
" +date+: +2015-08-07 03:30:33 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x13ced9eaa49a522d4e7dcf80a739a57dbf08f4ce5efc4edbac86a66d8010f693+,"\
" +blockNumber+: 200000,"\
" +name+: +iceage+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1441661589,"\
" +date+: +2015-09-07 21:33:09 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x827af61b9748436aed52828003f6a226a75be9e29676b4734ed011e94ff3b38d+,"\
" +blockNumber+: 543626,"\
" +name+: +devcon1+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1447577992,"\
" +date+: +2015-11-15 08:59:52 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x584bdb5d4e74fe97f5a5222b533fe1322fd0b6ad3eb03f02c3221984e2c0b430+,"\
" +blockNumber+: 1150000,"\
" +name+: +homestead+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1457981393,"\
" +date+: +2016-03-14 18:49:53 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x24caf7385e9bc711deaae286f8f2d7f79058be48b1ad76540974cf61a3fddeb7+,"\
" +blockNumber+: 1428756,"\
" +name+: +daofund+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1461980573,"\
" +date+: +2016-04-30 01:42:53 UTC+"\
"}"\
""\
",{"\
" +hash+: +0xcaaa13ce099342d5e1342b04d588d7733093591666af8ef756ce20cf13d16475+,"\
" +blockNumber+: 1718497,"\
" +name+: +daohack+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1466134488,"\
" +date+: +2016-06-17 03:34:48 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x4985f5ca3d2afbec36529aa96f74de3cc10a2a4a6c44f2157a57d2c6059a11bb+,"\
" +blockNumber+: 1920000,"\
" +name+: +daofork+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1469020840,"\
" +date+: +2016-07-20 13:20:40 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x2cb2611f99c5a16f20a7670322dca2df783e6b53bf03ab85bc436407b0142ab6+,"\
" +blockNumber+: 2286910,"\
" +name+: +devcon2+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1474275599,"\
" +date+: +2016-09-19 08:59:59 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x2086799aeebeae135c246c65021c82b4e15a2c451340993aacfd2751886514f0+,"\
" +blockNumber+: 2463000,"\
" +name+: +tangerine+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1476796771,"\
" +date+: +2016-10-18 13:19:31 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x58eff9265aedf8a54da8121de1324e1e0d9aac99f694d16c6a41afffe3817d73+,"\
" +blockNumber+: 2675000,"\
" +name+: +spurious+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1479831344,"\
" +date+: +2016-11-22 16:15:44 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x4a8dde305769cf93d16ac1986d624a8c1a2a530405c21d248ed5448f9549f972+,"\
" +blockNumber+: 2717576,"\
" +name+: +stateclear+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1480448309,"\
" +date+: +2016-11-29 19:38:29 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x0f2b57fe7a50a21591ba541d510187e6cd645087d2fa609f965e351563fd2408+,"\
" +blockNumber+: 3265360,"\
" +name+: +eea+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1488272400,"\
" +date+: +2017-02-28 09:00:00 UTC+"\
"}"\
""\
",{"\
" +hash+: +0xf925f632b57333c41961cc38f96600d71c7d10370e13779d00cd05e3d99eb1cd+,"\
" +blockNumber+: 3327417,"\
" +name+: +ens2+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1489165544,"\
" +date+: +2017-03-10 17:05:44 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x40dd9a773b81b00aacdc81598d19a5beef3ab66d391cc1cea6fb083294e7a184+,"\
" +blockNumber+: 4041179,"\
" +name+: +parityhack1+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1500417203,"\
" +date+: +2017-07-18 22:33:23 UTC+"\
"}"\
""\
",{"\
" +hash+: +0xb1fcff633029ee18ab6482b58ff8b6e95dd7c82a954c852157152a7a6d32785e+,"\
" +blockNumber+: 4370000,"\
" +name+: +byzantium+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1508131331,"\
" +date+: +2017-10-16 05:22:11 UTC+"\
"}"\
""\
",{"\
" +hash+: +0xa74908153dd5ad12233f8588156e5aac2dfc2762c0ec4a921aa6cdfe06caaf1a+,"\
" +blockNumber+: 4469339,"\
" +name+: +devcon3+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1509526798,"\
" +date+: +2017-11-01 08:59:58 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x894f3aac1c8a0c9b05d2cbe6c0c9af907ca44a1c96aeda69a0ec064b9a74b790+,"\
" +blockNumber+: 4501969,"\
" +name+: +parityhack2+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1509981921,"\
" +date+: +2017-11-06 15:25:21 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x79c943cb77f647e0553a101d0c1df2d05645782b3a1ac8d3cabc593eb4fc3fa3+,"\
" +blockNumber+: 4605167,"\
" +name+: +kitties+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1511415679,"\
" +date+: +2017-11-23 05:41:19 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x69ab90f74c7aca421c4f9bdf70e22534e5a653f995288d5b90c3783fa75722db+,"\
" +blockNumber+: 6610279,"\
" +name+: +devcon4+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 1540886395,"\
" +date+: +2018-10-30 07:59:55 UTC+"\
"}"\
""\
",{"\
" +hash+: +0x0+,"\
" +blockNumber+: 0,"\
" +name+: +constantinople+,"\
" +price+: 0,"\
" +finalized+: 1,"\
" +timestamp+: 0,"\
" +date+: +1970-01-01 00:00:00 UTC+"\
"}"\
""\
"]";
