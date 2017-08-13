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

   // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("fastest");

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool loaded = loadData();
    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        if (!loaded) {
            usage(configPath("configs/names.conf") + " is empty. Use ethName -e to add some names. Quitting...");
            exit(0);
        }

        SFString fmt = (options.addrOnly ? "[{ADDR}]" : "");
        if (options.list) {
            if (options.count)
                cout << accounts.getCount() << " items\n";
            for (uint32_t i = 0 ; i < accounts.getCount() ; i++)
                cout << accounts[i].Format(fmt).Substitute("\n", " ").Substitute("  ", " ") << "\n";
            exit(0);
        }

        SFString ret = showName(options);
        if (!ret.empty())
            cout << ret;
        else if (verbose)
            cout << "Address '" << options.addr << "' not found\n";
        cout.flush();
    }

    return 0;
}

//-----------------------------------------------------------------------
uint32_t countOf(const SFString& addr) {
    uint32_t cnt = 0;
    for (uint32_t i = 0 ; i < accounts.getCount() ; i++)
        if (accounts[i].addr % addr)
            cnt++;
    return cnt;
}

//-----------------------------------------------------------------------
SFString showName(const COptions& options) {
    SFString ret;
    uint32_t hits = 0;
    SFString fmt = (options.addrOnly ? "[{ADDR}]" : "");
    for (uint32_t i = 0 ; i < accounts.getCount() ; i++) {
        if (accounts[i].Match(options.addr, options.name, options.source, options.matchCase, options.all)) {
            ret += (accounts[i].Format(fmt).Substitute("\n", " ").Substitute("  ", " ") + "\n");
            hits++;
        }
    }

    if (options.count)
        ret = asString(hits) + " match" + (hits==1?"":"es") + "\n" + (verbose ? ret : "");
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

extern const char *STR_DEFAULT_DATA;
//-----------------------------------------------------------------------
bool loadData(void) {
    if (accounts.getCount() > 0)
        return true;

    if (!folderExists(configPath("configs/")))
        establishFolder(configPath("configs/"));

    SFString contents = asciiFileToString(configPath("configs/names.conf"));
    contents.ReplaceAll("\t\t", "\t");
    if (contents.empty()) {
        stringToAsciiFile(configPath("configs/names.conf"), STR_DEFAULT_DATA);
        return false;
    }

    while (!contents.empty()) {
        SFString line = nextTokenClear(contents, '\n');
        if (!line.startsWith("#")) {
            if (!countOf('\t', line))
                cerr << "Line " << line << " does not contain two tabs.\n";
            accounts[accounts.getCount()] = CAccountName(line);
        }
    }
    return true;
}

const char *STR_DEFAULT_DATA =
"#---------------------------------------------------------------------------------------------------\n"
"#  This is the ethName database. Format records as tab seperated lines with the following format:\n"
"#\n"
"#      ethereum_address <tab> name <tab> source <newline>\n"
"#\n"
"#  An entry for \"The DAO\" is included below.\n"
"#---------------------------------------------------------------------------------------------------\n"
"#          Ethereum Address                      Name                    Source\n"
"#---------------------------------------------------------------------------------------------------\n"
"0xbb9bc244d798123fde783fcc1c72d3bb8c189413	The DAO		The DAO website and git hub\n";
