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
extern string_q handle_sdk_go_enum(const string_q& route, const string_q& fn, const CCommandOption& option);

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_go(void) {
    handle_sdk_go_innersdk();
    handle_sdk_go_outersdk();

    ostringstream log;
    log << cYellow << "makeClass --sdk (go)" << cOff;
    log << " processed " << counter.routeCount << "/" << counter.cmdCount;
    log << " paths (changed " << counter.nProcessed << ")." << string_q(40, ' ');
    LOG_INFO(log.str());

    return true;
}

//------------------------------------------------------------------------------------------------------------
string_q handle_sdk_go_enum(const string_q& route, const string_q& fn, const CCommandOption& option,
                            ostringstream& enumsThing) {
    string_q ret = option.data_type;
    replace(ret, "list<", "");
    replace(ret, "enum[", "");
    replace(ret, "]", "");
    replace(ret, ">", "");
    replaceAll(ret, "*", "");
    bool hasAll = contains(ret, "all");

    CStringArray parts;
    explode(parts, ret, '|');

    string_q r(1, route[0]);
    r = toProper(r) + fn[0];
    string_q a = "";
    if (r == "CM") {
        static int cnt = 1;
        if (cnt == 1) {
            a = "1";
        } else {
            a = "2";
        }
        cnt = 2;
    }
    string_q none = "No" + r + a;
    string_q some = r + "Some";
    string_q all = r + "All";
    string_q inc =
        "balance|proxy|deployed|accttype|name|symbol|decimals|totalsupply|ext|int|token|nfts|1155|index|blooms";

    ostringstream os;
    os << "type " << toProper(route) << fn << " int" << endl;
    os << endl;
    os << "const (\n";
    os << "\t" << none << " " << toProper(route) << fn << " = 0" << endl;
    for (size_t i = 0; i < parts.size() - (hasAll ? 2 : 0); i++) {
        string_q part = parts[i];
        os << "\t" << r << firstUpper(part);
        if (i == 0) {
            os << " = 1 << iota";
        }
        os << endl;
    }
    if (hasAll) {
        os << "\t" << some << " = ";
        bool first = true;
        for (auto p : parts) {
            if (containsI(inc, p)) {
                if (!first)
                    os << " | ";
                os << r << firstUpper(p);
                first = false;
            }
        }
        os << endl;
        os << "\t" << all << " = ";
        for (size_t i = 0; i < parts.size() - 2; ++i) {
            if (i > 0)
                os << " | ";
            os << r << firstUpper(parts[i]);
        }
        os << endl;
    }
    os << ")" << endl;
    os << endl;

    os << "func (v " << toProper(route) << fn << ") String() string {\n";

    os << "\tswitch v {" << endl;
    os << "\tcase " << none << ":" << endl;
    os << "\t\treturn \"none\"" << endl;
    if (hasAll) {
        os << "\tcase " << some << ":" << endl;
        os << "\t\treturn \"some\"" << endl;
        os << "\tcase " << all << ":" << endl;
        os << "\t\treturn \"all\"" << endl;
    }
    os << "\t}" << endl;
    os << endl;

    os << "\tvar m = map[" << toProper(route) << fn << "]string{" << endl;
    for (auto& part : parts) {
        if (part == "all" || part == "some") {
            continue;
        }
        os << "\t\t" << r << firstUpper(part) << ": \"" << firstLower(part) << "\"," << endl;
    }
    os << "\t}" << endl << endl;
    os << "\tvar ret []string" << endl;
    os << "\tfor _, val := range []" << toProper(route) << fn << "{";
    ostringstream cases;
    for (size_t i = 0; i < parts.size() - (hasAll ? 2 : 0); ++i) {
        if (i > 0)
            os << ", ";
        os << r << firstUpper(parts[i]);
        cases << "\t\tcase \"" << firstLower(parts[i]) << "\":" << endl;
        cases << "\t\t\tresult |= " << r << firstUpper(parts[i]) << endl;
    }
    os << "} {" << endl;
    os << "\t\tif v&val != 0 {" << endl;
    os << "\t\t\tret = append(ret, m[val])" << endl;
    os << "\t\t}" << endl;
    os << "\t}" << endl << endl;
    os << "\treturn strings.Join(ret, \",\")\n";
    os << "}" << endl << endl;

    const char* STR_FROMSTRS =
        "func enumFrom[{PROPER}][{NAME}](values []string) ([{PROPER}][{NAME}], error) {\n"
        "	if len(values) == 0 {\n"
        "		return [{none}], fmt.Errorf(\"no value provided for [{LNAME}] option\")\n"
        "	}\n"
        "\n"
        "[{ALLTHING}]	var result [{PROPER}][{NAME}]\n"
        "	for _, val := range values {\n"
        "		switch val {\n"
        "[{CASES}]		default:\n"
        "			// JIMMYJAM\n"
        "			// JIMMYJAM\n"
        "			return [{none}], fmt.Errorf(\"unknown [{LNAME}]: %s\", val)\n"
        "		}\n"
        "	}\n"
        "\n"
        "	// JIMMYJAM\n"
        "	// JIMMYJAM\n"
        "\n"
        "	return result, nil\n"
        "}\n";

    string_q aStr;
    if (hasAll) {
        const char* STR_ALL =
            "\tif len(values) == 1 && values[0] == \"all\" {\n"
            "\t\treturn [{ALL}], nil\n"
            "\t} else if len(values) == 1 && values[0] == \"some\" {\n"
            "\t\treturn [{SOME}], nil\n"
            "\t}\n\n";
        aStr = STR_ALL;
        replace(aStr, "[{ALL}]", all);
        replace(aStr, "[{SOME}]", some);
    }

    string_q str = STR_FROMSTRS;
    replaceAll(str, "[{PROPER}]", toProper(route));
    replaceAll(str, "[{CASES}]", cases.str());
    replaceAll(str, "[{LNAME}]", toLower(fn));
    replaceAll(str, "[{NAME}]", fn);
    replaceAll(str, "[{none}]", none);
    replaceAll(str, "[{ALLTHING}]", aStr);
    os << str;

    const char* STR_ENUM_THING =
        "\tcase \"[{LNAME}]\":\n"
        "\t\tvar err error\n"
        "\t\tvalues := strings.Split(value, \",\")\n"
        "\t\tif opts.[{NAME}], err = enumFrom[{PROPER}][{NAME}](values); err != nil {\n"
        "\t\t\treturn false, err\n"
        "\t\t} else {\n"
        "\t\t\tfound = true\n"
        "\t\t}";
    string_q eT = STR_ENUM_THING;
    replaceAll(eT, "[{LNAME}]", firstLower(fn));
    replaceAll(eT, "[{NAME}]", firstUpper(fn));
    replaceAll(eT, "[{PROPER}]", toProper(route));
    enumsThing << eT << endl;

    return os.str();
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_go_innersdk(void) {
    string_q path = getCWD() + "apps/chifra/sdk/";
    establishFolder(path);

    for (auto ep : endpointArray) {
        if (!isApiRoute(ep.api_route) || ep.api_route == "explore") {
            continue;
        }

        string_q contents = asciiFileToString(getPathToTemplates("blank_sdk.go.tmpl"));
        contents = substitute(contents, "[{PROPER}]", toProper(ep.api_route));
        contents = substitute(contents, "[{LOWER}]", toLower(ep.api_route));

        string_q package = toLower(ep.api_route) + (toLower(ep.api_route) == "init" ? "Pkg" : "");
        contents = substitute(contents, "[{PKG}]", package);

        codewrite_t cw(path + ep.api_route + ".go", contents);
        cw.nSpaces = 0;
        cw.stripEOFNL = false;
        counter.nProcessed += writeCodeIn(this, cw);
        counter.nVisited++;
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_go_outersdk(void) {
    string_q path = getCWD() + "../sdk/go/";
    establishFolder(path);

    for (auto ep : endpointArray) {
        if (!isApiRoute(ep.api_route) || ep.api_route == "explore") {
            continue;
        }

        size_t maxNameWid = 0;
        size_t maxTypeWid = 0;
        ostringstream fields, enums, enumsThing;
        for (auto member : routeOptionArray) {
            if (member.generate == "config") {
                continue;
            }
            bool isOne = member.api_route == ep.api_route && member.isChifraRoute(true);
            if (isOne) {
                string_q fn = substitute(toProper(member.longName), "_", "");
                if (fn == "Blocks") {
                    fn = "BlockIds";
                } else if (fn == "Transactions") {
                    fn = "TransactionIds";
                }
                maxNameWid = max(maxNameWid, (fn).size() + 1);
                string_q t = member.go_intype;
                if (member.data_type == "<blknum>" || member.data_type == "<txnum>") {
                    t = "base.Blknum";
                } else if (member.data_type == "list<addr>") {
                    t = "[]string";
                } else if (member.data_type == "list<blknum>") {
                    t = "[]string";
                } else if (member.data_type == "list<topic>") {
                    t = "[]string";
                } else if (contains(member.data_type, "enum")) {
                    t = toProper(member.api_route) + toProper(member.longName);
                    // enums << handle_sdk_go_enum(ep.api_route, fn, member) << endl;
                } else if (contains(member.data_type, "address")) {
                    t = "base.Address";
                } else if (contains(member.data_type, "topic")) {
                    t = "base.Topic";
                }
                maxTypeWid = max(maxTypeWid, t.size() + 1);
            }
        }

        for (auto member : routeOptionArray) {
            if (member.generate == "config") {
                continue;
            }
            if (!member.is_visible_docs) {  //  && !containsI(member.longName, "cache")) {
                continue;
            }

            bool isOne = member.api_route == ep.api_route && member.isChifraRoute(true);
            if (isOne) {
                string_q fn = substitute(toProper(member.longName), "_", "");
                if (fn == "Blocks") {
                    fn = "BlockIds";
                } else if (fn == "Transactions") {
                    fn = "TransactionIds";
                }
                string_q t = member.go_intype;
                if (member.data_type == "<blknum>" || member.data_type == "<txnum>") {
                    t = "base.Blknum";
                } else if (member.data_type == "list<addr>") {
                    t = "[]string";
                } else if (member.data_type == "list<blknum>") {
                    t = "[]string";
                } else if (member.data_type == "list<topic>") {
                    t = "[]string";
                } else if (contains(member.data_type, "enum")) {
                    t = toProper(member.api_route) + toProper(member.longName);
                    enums << handle_sdk_go_enum(ep.api_route, fn, member, enumsThing) << endl;
                } else if (contains(member.data_type, "address")) {
                    t = "base.Address";
                } else if (contains(member.data_type, "topic")) {
                    t = "base.Topic";
                }
                ostringstream f;
                f << padRight(fn + " ", maxNameWid) << t;
                fields << "\t" << padRight(f.str(), maxNameWid + maxTypeWid);
                fields << "`json:\"" << substitute(firstLower(fn), "Ids", "s") << ",omitempty\"`" << endl;
                if (!(member.option_type % "positional")) {
                    reportOneOption(ep.api_route, toCamelCase(member.longName), "go-sdk");
                }
            }
        }
        fields << "\t" << toProper("Globals") << endl << endl;

        string_q caps = get_corrected_caps(ep.capabilities);
        CStringArray globals;
        explode(globals, caps, '|');
        for (auto global : globals) {
            reportOneOption(ep.api_route, global, "go-sdk");
        }

        string_q package = toLower(ep.api_route) + (toLower(ep.api_route) == "init" ? "Pkg" : "");
        string_q contents = asciiFileToString(getPathToTemplates("blank_sdk2.go.tmpl"));
        contents = substitute(contents, "[{CODE}]", "");
        contents = substitute(contents, "[{FIELDS}]", fields.str());
        contents = substitute(contents, "[{ENUMS}]", enums.str() == "" ? "// No enums\n\n" : enums.str());
        contents = substitute(contents, "[{PROPER}]", toProper(ep.api_route));
        contents = substitute(contents, "[{LOWER}]", toLower(ep.api_route));
        contents = substitute(contents, "[{PKG}]", package);
        if (enumsThing.str() == "") {
            contents = substitute(substitute(contents, "[{ENUMTHING}]", "\t// No enums\n\n"), "opts, ok := target.(*",
                                  "_, ok := target.(*");
        } else {
            contents = substitute(contents, "[{ENUMTHING}]", "\tswitch key {\n" + enumsThing.str() + "\t}\n");
        }

        codewrite_t cw(path + ep.api_route + ".go", contents);
        cw.nSpaces = 0;
        cw.stripEOFNL = false;
        counter.nProcessed += writeCodeIn(this, cw);
        counter.nVisited++;
    }

    return true;
}
