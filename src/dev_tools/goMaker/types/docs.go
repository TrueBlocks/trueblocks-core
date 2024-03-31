package types

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func (c *Command) PkgDoc() string {
	docsPath := "src/dev_tools/goMaker/templates/readme-intros/" + c.Route + ".md"
	docsPath = strings.ReplaceAll(docsPath, " ", "")
	contents := file.AsciiFileToString(docsPath)
	contents = strings.ReplaceAll(contents, "`", "")
	contents = strings.ReplaceAll(contents, "\n", " ")
	contents = strings.ReplaceAll(contents, "  ", " ")
	contents = strings.ReplaceAll(contents, "{{.Route}}", c.Route)
	sentences := strings.Split(contents, ".")
	return "// " + strings.Join(sentences, ".")
}

// handle_gocmds_docfile
// verifyGoEnumValidators
// verifyDescriptions
// replaceAll(source, "opts.LastBlock != utils.NOPOS", "opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS");
// string_q n = "readme-intros/" + substitute(toLower(cmd.group), " ", "") + "-" + cmd.api_route + ".config.md";
// string_q configDocs = getDocsPathTemplates(n);
// ::remove(configDocs.c_str());  // remove it if it exists, we will replace it
// ostringstream os;
// bool hasConfig = 0;
// for (auto member : *((CCommandOptionArray*)cmd.members)) {
//     if (member.generate == "config") {
//         ostringstream dd;
//         if (!hasConfig) {
//             dd << "| " << padRight("Item", 18) << " | " << padRight("Type", 12) << " | " << padRight("Default", 12)
//                << " | Description / Default |" << endl;
//             dd << "| " << string_q(18, '-') << " | " << string_q(12, '-') << " | " << string_q(12, '-')
//                << " | --------- |" << endl;
//         }
//         string_q x = toCamelCase(member.Format("[{LONGNAME}]"));
//         dd << "| " << padRight(x, 18) << " | " << padRight(member.Format("[{GO_INTYPE}]"), 12) << " | "
//            << padRight(member.Format("[{DEF_VAL}]"), 12) << " | " << member.Format("[{DESCRIPTION}]") << " |"
//            << endl;
//         appendToAsciiFile(configDocs, dd.str());
//         hasConfig = true;
//         continue;
//     }
//     string_q var = member.Format("[{VARIABLE}]");
//     string_q type = member.Format("[{GO_INTYPE}]");
//     ONE(os, var, varWidth, type, typeWidth, member.description);
//     if (contains(var, "Blocks") && contains(member.go_intype, "[]string")) {
//         ONE(os, "BlockIds", varWidth, "[]identifiers.Identifier", typeWidth, "block identifiers");
//     }
//     if (contains(var, "Transactions") && contains(member.go_intype, "[]string")) {
//         ONE(os, "TransactionIds", varWidth, "[]identifiers.Identifier", typeWidth, "transaction identifiers");
//     }
// }
// if (hasConfig) {
//     string type = cmd.Format("config.[{PROPER}]Settings");
//     ONE(os, "Settings", varWidth, type, typeWidth, "Configuration items for the " + cmd.api_route);
// }
// ONE(os, "Globals", varWidth, "globals.GlobalOptions", typeWidth, "the global options");
// ONE(os, "Conn", varWidth, "*rpc.Connection", typeWidth, "the connection to the RPC server");
// ONE(os, "BadFlag", varWidth, "error", typeWidth, "an error flag if needed");
// hasValidator(const string_q& route, const string_q& str) {
//     string_q contents = asciiFileToString(getPathToSource("apps/chifra/internal/" + route + "/validate.go"));
//     CStringArray parts;
//     explode(parts, str, '|');
//     for (auto part : parts) {
//         if (!contains(contents, part))
//             return false;
//     }
//     return true;
// }

// //---------------------------------------------------------------------------------------------------
// void COptions::verifyGoEnumValidators(void) {
//     for (auto p : cmdOptionArray) {
//         if (contains(p.data_type, "enum") && !p.api_route.empty()) {
//             string_q e = p.data_type;
//             replace(e, "enum", "");
//             replace(e, "list", "");
//             replaceAny(e, "<[*]>", "");
//             e = "[" + e + "]";
//             if (!hasValidator(p.api_route, e)) {
//                 LOG_WARN("\t", bRed, p.api_route, " has no enum validator for ", e, cOff);
//                 exit(0);
//             }
//         }
//         if (p.generate == "deprecated") {
//             if (!hasValidator(p.api_route, "deprecated|--" + p.longName)) {
//                 ostringstream os;
//                 os << "The '" << p.api_route << " --" << p.longName
//                    << "' option is deprecated, but no usage message in validate.go";
//                 LOG_WARN("\t", bRed, os.str(), cOff);
//             }
//         }
//     }
// }

// //---------------------------------------------------------------------------------------------------
// bool COptions::handle_gocmds_docfile(const CCommandOption& ep) {
//     string_q source = asciiFileToString(getPathToTemplates("blank_doc.go.tmpl"));
//     string_q fn = getPathToSource("apps/chifra/internal/" + ep.api_route + "/doc.go");
//     establishFolder(fn);

//     string_q docFn = substitute(toLower(ep.group), " ", "") + "-" + ep.api_route + ".md";
//     string_q docSource = getDocsPathTemplates("readme-intros/" + docFn);
//     const char* STR_WHAT = "// Package [{API_ROUTE}]Pkg handles the chifra [{API_ROUTE}] command. It ";
//     string_q contents = STR_WHAT + asciiFileToString(docSource);
//     contents = substitute(substitute(substitute(contents, "\n", " "), "\t", " "), "\r", " ");
//     replaceAll(contents, "  ", " ");
//     replaceAll(contents, "`", "");
//     replaceAll(contents, "[{NAME}]", "[chifra {ROUTE}]");
//     replaceAll(source, "TEXT_TEMPLATE", contents);
//     replaceAll(source, "[{ROUTE}]", ep.api_route);
//     replaceAll(source, "[{LOWER}]", toLower(ep.api_route));
//     source = ep.Format(source);

//     codewrite_t cw(fn, source);
//     cw.nSpaces = 0;
//     cw.stripEOFNL = false;
//     counter.nProcessed += writeCodeIn(this, cw);
//     counter.nVisited++;

//     return true;
// }
//---------------------------------------------------------------------------------------------------
// void COptions::verifyDescriptions(void) {
//     for (auto ep : endpointArray) {
//         if (!ep.is_visible)
//             continue;
//         for (auto option : cmdOptionArray) {
//             if (ep.api_route == option.api_route && option.option_type == "description") {
//                 if (ep.description != option.description) {
//                     ostringstream os;
//                     os << "Endpoint descriptions for " << ep.api_route << " do not agree:" << endl;
//                     os << "\tendpoint: " << ep.description << endl;
//                     os << "\toption:   " << option.description << endl;
//                     LOG_WARN(os.str());
//                 }
//             }
//         }
//     }
// }
