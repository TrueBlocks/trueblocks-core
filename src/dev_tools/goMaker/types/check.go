package types

// handle_gocmds_docfile
// verifyGoEnumValidators
// verifyDescriptions
// replaceAll(source, "opts.LastBlock != utils.NOPOS", "opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS");
// string_q n = "readme-intros/" + substitute(toLower(cmd.group), " ", "") + "-" + cmd.a pi_route + ".config.md";
// string_q configDocs = getDocsPathTemplates(n);
// ::remove(configDocs.c_str());  // remove it if it exists, we will replace it
// ostringstream os;
// bool hasConfig = 0;
// for (auto member : *((C CommandOptionArray*)cmd.members)) {
//     if (member.generate == "config") {
//         ostringstream dd;
//         if (!hasConfig) {
//             dd << "| " << p adRight("Item", 18) << " | " << p adRight("Type", 12) << " | " << p adRight("Default", 12)
//                << " | Description / Default |" << endl;
//             dd << "| " << string_q(18, '-') << " | " << string_q(12, '-') << " | " << string_q(12, '-')
//                << " | --------- |" << endl;
//         }
//         string_q x = toCamelCase(member.Format("[{LONGNAME}]"));
//         dd << "| " << p adRight(x, 18) << " | " << p adRight(member.Format("[{GO_INTYPE}]"), 12) << " | "
//            << p adRight(member.Format("[{DEF_VAL}]"), 12) << " | " << member.Format("[{DESCRIPTION}]") << " |"
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
//     ONE(os, "Settings", varWidth, type, typeWidth, "Configuration items for the " + cmd.a pi_route);
// }
// ONE(os, "Globals", varWidth, "globals.GlobalOptions", typeWidth, "the global options");
// ONE(os, "Conn", varWidth, "*rpc.Connection", typeWidth, "the connection to the RPC server");
// ONE(os, "BadFlag", varWidth, "error", typeWidth, "an error flag if needed");
// hasValidator(const string_q& route, const string_q& str) {
//     string_q contents = asciiFileToString(getPathToSourc e("apps/chifra/internal/" + route + "/validate.go"));
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
//             string_q e = p.data_type;
//             replace(e, "enum", "");
//             replace(e, "list", "");
//             replaceAny(e, "<[*]>", "");
//             e = "[" + e + "]";
//             if (!hasValidator(p.a pi_route, e)) {
//                 LOG_WARN("\t", bRed, p.a pi_route, " has no enum validator for ", e, cOff);
//                 exit(0);
//             }
//         }
//         if (p.generate == "deprecated") {
//             if (!hasValidator(p.a pi_route, "deprecated|--" + p.longName)) {
//                 ostringstream os;
//                 os << "The '" << p.a pi_route << " --" << p.longName
//                    << "' option is deprecated, but no usage message in validate.go";
//                 LOG_WARN("\t", bRed, os.str(), cOff);
//             }
//         }
//     }
// }

// //---------------------------------------------------------------------------------------------------
// bool COptions::handle_gocmds_docfile(const CCommandOption& ep) {
//     string_q source = asciiFileToString(getPathToTemplates("blank_doc.go.tmpl"));
//     string_q fn = getPathToSourc e("apps/chifra/internal/" + ep.a pi_route + "/doc.go");
//     establishFolder(fn);

//     string_q docFn = substitute(toLower(ep.group), " ", "") + "-" + ep.a pi_route + ".md";
//     string_q docSource = getDocsPathTemplates("readme-intros/" + docFn);
//     const char* STR_WHAT = "// Package [{a pi_route}]Pkg handles the chifra [{a pi_route}] command. It ";
//     string_q contents = STR_WHAT + asciiFileToString(docSource);
//     contents = substitute(substitute(substitute(contents, "\n", " "), "\t", " "), "\r", " ");
//     replaceAll(contents, "  ", " ");
//     replaceAll(contents, "`", "");
//     replaceAll(contents, "[{NAME}]", "[chifra {ROUTE}]");
//     replaceAll(source, "TEXT_TEMPLATE", contents);
//     replaceAll(source, "[{ROUTE}]", ep.a pi_route);
//     replaceAll(source, "[{LOWER}]", toLower(ep.a pi_route));
//     source = ep.Format(source);

//     codewrite_t cw(fn, source);
//     cw.nSpaces = 0;
//     cw.stripEOFNL = false;
//     counter.nProcessed += writeCod eIn(this, cw);
//     counter.nVisited++;

//     return true;
// }
//---------------------------------------------------------------------------------------------------
// void COptions::verifyDescriptions(void) {
//     for (auto ep : endpointAr ray) {
//         if (!ep.is_visible)
//             continue;
//         for (auto option : cmdOptionArra y) {
//             if (ep.a pi_route == option.a pi_route && option.option_type == "comm and") {
//                 if (ep.description != option.description) {
//                     ostringstream os;
//                     os << "Endpoint descriptions for " << ep.a pi_route << " do not agree:" << endl;
//                     os << "\tendpoint: " << ep.description << endl;
//                     os << "\toption:   " << option.description << endl;
//                     LOG_WARN(os.str());
//                 }
//             }
//         }
//     }
// }
