/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------------
extern bool cutFiles(const string_q& path, void *data);
extern bool writeRecord(const char *line, void *data);

//-----------------------------------------------------------------------------
class CThing {
public:
    uint64_t nWritten;
    string_q outputFilename;
    string_q sourceFile;
    CArchive *output;
    CThing(void) { output = NULL; nWritten = 0; }
};

//-----------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    CThing thing;
    forEveryFileInFolder(blockCachePath("addr_index/unsorted_by_10000"), cutFiles, &thing);
    return 0;
}

//-----------------------------------------------------------------------------
bool cutFiles(const string_q& path, void *data) {

    if (endsWith(path,'/')) {
        return forEveryFileInFolder(path+"*", cutFiles, data);

    } else {

        if (endsWith(path, ".txt")) {
            blknum_t bn = bnFromPath(path);
            if (bn < 4350000)
                return true;
            CThing *thing = (CThing*)data;
            thing->sourceFile = path;
            forEveryLineInAsciiFile(path, writeRecord, thing);
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
bool writeRecord(const char *l, void *data) {
    CThing *thing = (CThing*)data;
    static bool on = false;
    const char *find = strstr(l, "4358591\t108\t\t0xea421551ac2403a98543bd8227cd57998915ad32\tinput");
    if (find) {
        on = true;
        thing->nWritten = 500000;
        return true;
    }
    if (!on)
        return true;
    blknum_t bn = str_2_Uint(l);
    if (!thing->output || thing->nWritten == 500000) {
        if (thing->output) {
            thing->output->Release();
            delete thing->output;
            thing->output = NULL;
        }
        thing->output = new CArchive(READING_ARCHIVE);
        if (!thing->output) {
            cerr << "Could allocate file pointer " << thing->outputFilename << "\n";
            return false;
        }
        thing->outputFilename = blockCachePath("addr_index/unsorted_by_n_lines/") + padLeft(uint_2_Str(bn), 9, '0') + ".txt";
        cerr << "\tOpening file " << thing->outputFilename << "\n";
        if (!thing->output->Lock(thing->outputFilename, asciiWriteCreate, LOCK_NOWAIT)) {
            cerr << "Could not open file " << thing->outputFilename << "\n";
            return false;
        }
        thing->nWritten = 0;
    }
    thing->output->WriteLine(l);
    thing->output->flush();
    thing->nWritten++;
    ((char*)l)[strlen(l)-1] = '\r';
    static uint64_t counter = 0;
    if (!(++counter%43))
        cerr << "\t" << cGreen << bn << ": " << cTeal << l << cOff;
    cerr.flush();
    return true;
}




























#ifdef OLD_CODE_THAT_MAKES_SORTED_FILES
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"
#include "reporter.h"

extern bool forEveryAccount(CTreeRoot *trie, ACCTVISITOR func, void *data);
extern bool visitFiles(const string_q& path, void *data);
extern bool cutFiles(const string_q& path, void *data);

//-----------------------------------------------------------------------------
extern string_q getAddrIndexPath(blknum_t blk);
//-----------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    forEveryFileInFolder(blockCachePath("addr_index/unsorted_by_tx"), cutFiles, NULL);
    return 0;

#if 0
    acctlib_init1();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    if (options.commandList.empty())
        options.commandList = "null";
    //    while (!options.commandList.empty())
    {
        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        CReporter reporter;
        reporter.tree = new CTreeRoot;
        bool isWrite = false;
        if (reporter.tree) {

            while (!options.mode.empty()) {
                string_q mode = nextTokenClear(options.mode,'|');
                if (mode == "list") {
                    cout << "Listing\n";
                    CArchive treeCache(READING_ARCHIVE);
                    if (treeCache.Lock(accountIndex, binaryReadOnly, LOCK_NOWAIT)) {
                        reporter.tree->Serialize(treeCache);
                        treeCache.Release();
                    }
                } else {
                    //-----------------------------------------------
                    cout << "Building\n";
                    reporter.startTimer("Accumulating accounts...");
                    forEveryFileInFolder(blockCachePath("addrs_by_block/txt"), visitFiles, &reporter);
//                    forEveryBlockOnDisc(buildTree, &reporter, start, getLatestBlockFromClient());
                    reporter.stopTimer();
                    isWrite = true;
                }

                //-----------------------------------------------
                reporter.startTimer("Displaying accounts...");
                forEveryAccount(reporter.tree, printTree, &reporter);
                reporter.stopTimer();
            }
            reporter.finalReport();
        }

        if (isWrite) {
            CArchive treeCache(WRITING_ARCHIVE);
            if (treeCache.Lock(accountIndex, binaryWriteCreate, LOCK_CREATE)) {
                reporter.tree->Serialize(treeCache);
                treeCache.Release();
            }
            delete reporter.tree;
            reporter.tree = NULL;
        }
    }

    return 0;
#endif
}

#if 0
//-----------------------------------------------------------------------
bool buildTree(CBlock& block, void *data) {

    CReporter *r = reinterpret_cast<CReporter*>(data);

    r->nBlocksVisited++;
    for (uint32_t i = 0 ; i < block.transactions.size() ; i++) {
        CTransaction *tr = (CTransaction*)&block.transactions[i];  // NOLINT
        if (string_q(tr->to).empty())
            tr->to = "0x0";
        r->nTransVisited++;
        r->tree->insert(tr->from, uint_2_Str(block.blockNumber));
        r->tree->insert(tr->to,   uint_2_Str(block.blockNumber));
        cerr << r->nBlocksVisited << ":" << block.blockNumber << ":" << tr->transactionIndex << "      \n";
        cerr.flush();
    }
    cerr << r->nBlocksVisited << ":" << block.blockNumber << "\t\r";
    cerr.flush();
    return (r->nBlocksVisited < maxBlocks);
}
#endif

//------------------------------------------------------------------
bool CLeaf::visitItems(ACCTVISITOR func, void *data) const {
    ASSERT(func);
    CVisitData *vd = reinterpret_cast<CVisitData*>(data);
    uint64_t save = vd->type;
    vd->counter = counter2;
    vd->type = T_LEAF;
    vd->strs0 = vd->strs0 + "+" + (cMagenta+prefixS + cOff + "|" + cBlue + at(prefixS) + cOff);
    vd->blocks = blocks1;
    (*func)(this, data);
    vd->blocks.clear();
    nextTokenClearReverse(vd->strs0, '+');
    vd->type = save;
    return true;
}

//------------------------------------------------------------------
bool CInfix::visitItems(ACCTVISITOR func, void *data) const {
    ASSERT(func);
    CVisitData *vd = reinterpret_cast<CVisitData*>(data);
    uint64_t save = vd->type;
    vd->counter = 0;
    vd->type = T_INFIX;
    vd->strs0 = vd->strs0 + "+" + prefixS;
    (*func)(this, data);
    if (next) {
        vd->level++;
        next->visitItems(func, data);
        vd->level--;
    }
    nextTokenClearReverse(vd->strs0, '+');
    vd->type = save;
    return true;
}

//------------------------------------------------------------------
bool CBranch::visitItems(ACCTVISITOR func, void *data) const {
    ASSERT(func);
    CVisitData *vd = reinterpret_cast<CVisitData*>(data);
    uint64_t save = vd->type;
    vd->type = T_BRANCH;
    vd->counter = 0;
    vd->strs0 = vd->strs0 + branchValue + "+";
    // vd->strs = vd->strs + "+";
    (*func)(this, data);
    for (size_t i = 0; i < 16; ++i) {
        if (nodes[i]) {
            vd->level++;
            vd->strs0 = vd->strs0 + "-" + idex((char)i);  // NOLINT
            nodes[i]->visitItems(func, data);
            nextTokenClearReverse(vd->strs0, '-');
            vd->level--;
        }
    }
    nextTokenClearReverse(vd->strs0, '+');
    vd->type = save;
    return true;
}

//-----------------------------------------------------------------------------
bool CTreeRoot::visitItems(ACCTVISITOR func, void *data) const {
    ASSERT(func);
    CVisitData *vd = reinterpret_cast<CVisitData*>(data);
    vd->level = 0;
    if (root) {
        bool ret = root->visitItems(func, data);
        return ret;
    }
    return true;
}

//------------------------------------------------------------------
bool forEveryAccount(CTreeRoot *trie, ACCTVISITOR func, void *data) {
    ASSERT(trie);
    return trie->visitItems(func, data);
}

//-----------------------------------------------------------------------------
bool printTree(const CTreeNode *node, void *data) {
    CReporter *r = reinterpret_cast<CReporter*>(data);
    r->nAccts++;

    if (r->getNext) {
        r->maxMatch2 = r->strs0;
        r->getNext = false;
    }

    if (r->isMax()) {
        r->maxDepth  = countOf(r->strs0, '-');
        r->getNext = true;
        r->maxMatch1 = r->strs0;
    }

    // print the report
    r->interumReport();

    return true;
}

//-----------------------------------------------------------------------------
void CReporter::interumReport(void) {
    string_q types[] = {
        cWhite   +     "T_TRTOP " + cOff,
        cRed     +     "T_TRLEAF" + cOff,
        bYellow  + "\n""T_BRANCH" + cOff,
        cGreen   + "\n""T_INFIX " + cOff,
    };
    cout << types[type] << " " << substitute(substitute(substitute(strs0, "+", ""), "-", ""), "|0", "") << "\n";
    for (auto bl : blocks)
        cout << "\t" << bl << "\n";
}

//-----------------------------------------------------------------------------
unsigned char getArchiveIndex(const address_t& addr) {
    return hex2Ascii(addr.substr(2,2).c_str());
}

//-----------------------------------------------------------------------------
bool visitFiles(const string_q& path, void *data) {

    if (endsWith(path,'/')) {
        return forEveryFileInFolder(path+"*",visitFiles,data);
    } else {
        CReporter *rep = (CReporter*)data;
        blknum_t bn = str_2_Uint(substitute(path,"/Volumes/Samsung_T7/scraper/addrs_by_block/txt/",""));
        if (bn >= 2286910 && bn <= 2717576)
            return true;
        cout << "Reading " << path << "\n";
        string_q contents;
        asciiFileToString(path, contents);
        CStringArray lines;
        explode(lines, contents, '\n');
//        cout << path << "\n";
        for (auto line : lines) {
            CStringArray dat;
            explode(dat, line, '\t');
            cout << "adding to archive ";
            if ((int)getArchiveIndex(dat[3]) < 17)
                cout << "0";
            cout << hex << (int)getArchiveIndex(dat[3]) << dec;
            cout << ": " << dat[3] << "\t" << dat[0] << "\n";
            rep->tree->insert(dat[3], dat[0]);
        }
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool cutFiles(const string_q& path, void *data) {

    if (endsWith(path,'/')) {
        return forEveryFileInFolder(path+"*", cutFiles, data);

    } else {
        cout << "Reading " << path << "\n";
        string_q contents;
        asciiFileToString(path, contents);
        CStringArray lines;
        lines.reserve(2500000);
        explode(lines, contents, '\n');
        blknum_t prev = INT_MAX;
        string_q filename;
        CStringArray output;
        output.reserve(100000);
        for (auto line : lines) {
            CStringArray dat;
            explode(dat, line, '\t');
            blknum_t bn = str_2_Uint(dat[0]);
            blknum_t cur = (bn / 10000) * 10000;
            if (prev != cur) {
                if (output.size() > 0) {
                    sort(output.begin(), output.end());
                    cout << "    Appending to file " << filename << "\n";
                    string_q out;
                    for (auto str : output)
                        out += str;
                    appendToAsciiFile(filename, out);
                    output.clear();
                    output.reserve(100000);
                }
                filename = getAddrIndexPath(bn);
                establishFolder(filename);
                prev = cur;
            }
            string_q out = dat[3] + "\t" + padLeft(dat[0], 8, '0') + "\t" + padLeft(dat[1], 5, '0');
            output.push_back(out + "\n");
            cerr << out << "\r";
            cerr.flush();
        }
        if (output.size() > 0) {
            sort(output.begin(), output.end());
            cout << "    Appending to file " << filename << "\n";
            string_q out;
            for (auto str : output)
                out += str;
            appendToAsciiFile(filename, out);
        }
    }
    return true;
}

string_q getAddrIndexPath(blknum_t blk) {
    string_q path = getBinaryFilename((blk / 10000) * 10000);
    CStringArray parts;
    explode(parts, path, '/');
    ostringstream os;
    for (size_t p = 0 ; p < parts.size() ; p++) {
        if (p < parts.size()-3 || p == parts.size()-1)
            os << "/" << parts[p];
    }
    string_q ret = os.str();
    ret = substitute(ret, "/blocks/", "/addr_index/sorted_by_10000/");
    ret = substitute(ret, ".bin", ".txt");
    ret = substitute(ret, "/""/", "/");
    return ret;
}
#endif
