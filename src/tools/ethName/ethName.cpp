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
    etherlib_init("fastest");

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool loaded = options.loadData();
    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        if (options.isEdit) {
            editFile(options.namesFile.getFullPath());
            exit(0);
        }

        if (!loaded) {
            usage(options.namesFile.getFullPath() + " is empty. Use ethName -e to add some names. Quitting...");
            exit(0);
        }

        SFString fmt = (options.alone ? "[{ADDR}]" : "");
        if (options.list) {
            if (options.count)
                cout << options.accounts.getCount() << " items\n";
            for (uint32_t i = 0 ; i < options.accounts.getCount() ; i++)
                cout << options.accounts[i].Format(fmt).Substitute("\n", " ").Substitute("  ", " ") << "\n";
            exit(0);
        }

        SFString ret = options.showName();
        if (!ret.empty())
            cout << ret;
        else if (verbose)
            cout << "Address '" << options.addr << "' not found\n";
        cout.flush();
    }

    return 0;
}

//-----------------------------------------------------------------------
SFString COptions::showName(void) {
    SFString ret;
    uint32_t hits = 0;
    SFString fmt = (alone ? "[{ADDR}]" : "");
    for (uint32_t i = 0 ; i < accounts.getCount() ; i++) {
        if (accounts[i].Match(addr, name, source, matchCase, all)) {
            ret += (accounts[i].Format(fmt).Substitute("\n", " ").Substitute("  ", " ") + "\n");
            hits++;
        }
    }

    if (count)
        ret = asString(hits) + " match" + (hits==1?"":"es") + "\n" + (verbose ? ret : "");

    return ret;
}

extern const char *STR_DEFAULT_DATA;
//-----------------------------------------------------------------------
bool COptions::loadData(void) {

    // If we're already loaded or editing, return
    if (accounts.getCount() > 0 || isEdit)
        return true;

    SFString textFile = namesFile.getFullPath();
    SFString binFile  = textFile.Substitute(".txt",".bin");

    SFTime txtDate = fileLastModifyDate(textFile);
    SFTime binDate = fileLastModifyDate(binFile);

    if (verbose && !isTestMode())
        cout << "txtDate: " << txtDate << " binDate: " << binDate << "\n";

    if (binDate > txtDate) {
        SFArchive archive(READING_ARCHIVE);
        if (archive.Lock(binFile, binaryReadOnly, LOCK_NOWAIT)) {
            if (verbose && !isTestMode())
                cout << "Reading from binrary cache\n";
            archive >> accounts;
            archive.Release();
            return true;
        }
    }

    if (verbose && !isTestMode())
        cout << "Reading from text database\n";

    // Read the data from the names database and clean it up if needed
    SFString contents = StripAny(asciiFileToString(textFile), "\t\n ");
    contents.ReplaceAll("\t\t", "\t");
    if (!contents.endsWith("\n"))
        contents += "\n";

    // Parse out the data....
    while (!contents.empty()) {
        SFString line = nextTokenClear(contents, '\n');
        if (!line.startsWith("#")) {
            if (countOf('\t', line) < 2) {
                cerr << "Line " << line << " does not contain two tabs.\n";

            } else {
                CAccountName account(line);
                accounts[accounts.getCount()] = account;
            }
        }
    }

    SFArchive archive(WRITING_ARCHIVE);
    if (archive.Lock(binFile, binaryWriteCreate, LOCK_CREATE)) {
        if (verbose && !isTestMode())
            cout << "Writing binary cache\n";
        archive << accounts;
        archive.Release();
    }

    return true;
}
