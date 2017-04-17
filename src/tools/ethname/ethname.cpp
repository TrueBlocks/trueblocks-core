/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 1999, 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "ethname.h"

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

        if (options.list) {
            for (int i = 0 ; i < nNames ; i++)
                out << accounts[i].Format12() << "\n";
            exit(0);
        }
        SFString ret = showName(options.addr, options.name, options.source, options.matchCase, options.all);
        if (!ret.empty())
            out << ret;
        else if (verbose)
            out << "Address '" << options.addr << "' not found\n";

        if (options.write)
            out << "Writing is not implemented.\n";

        out.Flush();
    }
    if (accounts)
        delete [] accounts;

    return 0;
}

//-----------------------------------------------------------------------
void loadData(void) {
    if (nNames > 0 || accounts != NULL)
        return;

    SFString contents = asciiFileToString(DATA_FILE);
    contents.ReplaceAll("\t\t", "\t");

    uint32_t nLines = countOf('\n', contents);
    accounts = new CAccountName[nLines];
    if (!accounts) {
        outErr << "Could not allocate memory for " << nLines << " account names. Quitting...\n";
        exit(0);
    }

    while (!contents.empty()) {
        SFString line = nextTokenClear(contents, '\n');
        if (!countOf('\t', line))
            outErr << "Line " << line << " does not contain two tabs.\n";
        accounts[nNames++] = CAccountName(line);
    }
}

//-----------------------------------------------------------------------
uint32_t countOf(const SFString& addr) {
    uint32_t cnt = 0;
    for (int i = 0 ; i < nNames ; i++)
        if (accounts[i].addr % addr)
            cnt++;
    return cnt;
}

//-----------------------------------------------------------------------
SFString showName(const SFString& s1, const SFString& s2, const SFString& s3, bool matchCase, bool all) {
    SFString ret;
    for (int i = 0 ; i < nNames ; i++) {
        if (accounts[i].Match(s1, s2, s3, matchCase, all))
            ret += (accounts[i].Format12() + "\n");
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
CAccountName *accounts = NULL;
uint32_t nNames = 0;

//-----------------------------------------------------------------------
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
