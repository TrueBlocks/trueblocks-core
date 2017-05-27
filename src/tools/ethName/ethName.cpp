/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"
#include "accountname.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    loadData();
    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        SFString fmt = (options.addrOnly ? "[{ADDR}]" : "");
        if (options.list) {
            for (int i = 0 ; i < accounts.getCount() ; i++)
                out << accounts[i].Format(fmt).Substitute("\n", " ").Substitute("  ", " ") << "\n";
            exit(0);
        }

        SFString ret = showName(options);
        if (!ret.empty())
            out << ret;
        else if (verbose)
            out << "Address '" << options.addr << "' not found\n";

        out.flush();
    }

    return 0;
}

//-----------------------------------------------------------------------
void loadData(void) {
    if (accounts.getCount() > 0)
        return;

    SFString contents = asciiFileToString(configPath("configs/names.conf"));
    contents.ReplaceAll("\t\t", "\t");

    while (!contents.empty()) {
        SFString line = nextTokenClear(contents, '\n');
        if (!countOf('\t', line))
            cerr << "Line " << line << " does not contain two tabs.\n";
        accounts[accounts.getCount()] = CAccountName(line);
    }
}

//-----------------------------------------------------------------------
uint32_t countOf(const SFString& addr) {
    uint32_t cnt = 0;
    for (int i = 0 ; i < accounts.getCount() ; i++)
        if (accounts[i].addr % addr)
            cnt++;
    return cnt;
}

//-----------------------------------------------------------------------
SFString showName(const COptions& options) {
    SFString ret;
    SFString fmt = (options.addrOnly ? "[{ADDR}]" : "");
    for (int i = 0 ; i < accounts.getCount() ; i++) {
        if (accounts[i].Match(options.addr, options.name, options.source, options.matchCase, options.all))
            ret += (accounts[i].Format(fmt).Substitute("\n", " ").Substitute("  ", " ") + "\n");
    }
    return ret;
}

//-----------------------------------------------------------------------
SFString setName(const SFString& addr, const SFString& name) {
    CStringExportContext ctx;
    ctx << "setName(" << addr << "," << name << ");\n";
    return ctx.str;
}

//-----------------------------------------------------------------------
CAccountNameArray accounts;
CFileExportContext out;

//---------------------------------------------------------------------------
bool CAccountName::Match(const SFString& s1, const SFString& s2, const SFString& s3, bool matchCase, bool all) {
    bool m11 = (matchCase ? addr.Contains(s1) : addr.ContainsI(s1));
    bool m12 = (matchCase ? name.Contains(s1) : name.ContainsI(s1));
    bool m13 = (matchCase ? source.Contains(s1) : source.ContainsI(s1));
    bool m2 = (matchCase ? name.Contains(s2) : name.ContainsI(s2));
    bool m3 = (matchCase ? source.Contains(s3) : source.ContainsI(s3));

    if (!s1.empty() && !s2.empty() && !s3.empty())
        return m11 && m2 && m3;  // all three must match

    if (!s1.empty() && !s2.empty())
        return m11 && m2;  // addr and name must both match

    if (s1.empty())
        return false;  // nothing matches

    // We have only s1
    return (all ? m11 || m12 || m13 : m11 || m12);
}
