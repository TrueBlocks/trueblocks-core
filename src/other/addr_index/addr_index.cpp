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
string_q getAddrIndexPath(blknum_t blk) {
    string_q path = getBinaryFilename((blk / 10000) * 10000);
    CStringArray parts;
    explode(parts, path, '/');
    ostringstream os;
    for (size_t p = 0 ; p < parts.size() ; p++) {
        if (p < parts.size()-3 || p == parts.size()-1)
            os << "/" << parts[p];
    }
    return substitute(substitute(substitute(os.str(), "/blocks/", "/addr_index/"), ".bin", ".txt"),"//","/");
}

//-----------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

//    blknum_t n = getLatestBlockFromClient();
//    for (uint64_t blk = 10000 ; blk < n ; blk = blk + 10000) {
//        cout << getAddrIndexPath(blk) << "\n";
//    }
    forEveryFileInFolder(blockCachePath("addr_index/txt"), cutFiles, NULL);
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
                    forEveryFileInFolder(blockCachePath("addr_index/txt"), visitFiles, &reporter);
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
        blknum_t bn = str_2_Uint(substitute(path,"/Volumes/Samsung_T7/scraper/addr_index/txt/",""));
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

    //cout << path << "\n";
    if (endsWith(path,'/')) {
        return forEveryFileInFolder(path+"*",cutFiles,data);
    } else {
        cout << "Reading " << path << "\n";
        string_q contents;
        asciiFileToString(path, contents);
        CStringArray lines;
        explode(lines, contents, '\n');
        //cout << path << "\n";
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
            cerr << dat[3] << "\t" << dat[0] << "\r";
            cerr.flush();
            output.push_back(dat[3] + "\t" + dat[0] + "\n");
        }
        if (output.size() > 0) {
            cout << "    Appending to file " << filename << "\n";
            string_q out;
            for (auto str : output)
                out += str;
            appendToAsciiFile(filename, out);
            output.clear();
            output.reserve(100000);
        }
    }
    return true;
}

