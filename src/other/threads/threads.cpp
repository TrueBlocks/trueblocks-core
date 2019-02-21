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
#include <iostream>
#include "etherlib.h"

class COptions : public CDefaultOptions {
public:
    int64_t maxBlock = 50000000;
    int64_t startBlock = 5500000;
};
COptions options;

#define REP_FREQ  1
#define REP_FREQ2 (REP_FREQ*20)
#define STEP      1

//--------------------------------------------------------------------------
class CProgress {
public:
    int64_t bProg;
    int64_t fProg;
    bool    headDone;
    CProgress(void) { headDone = false; bProg = fProg = 5000000; }
};
//--------------------------------------------------------------------------
class CBlockScraper : public CArchive {
public:
    CProgress& prog;
    int64_t start, stop;
    thread *thread;
    CBlockScraper(CProgress& pr) : CArchive(WRITING_ARCHIVE), prog(pr), start(0), stop(0) {
    }
private:
    CBlockScraper(void) = delete;
};

//--------------------------------------------------------------------------
void scanForward(CBlockScraper *scraper) {
    cout << "front - pid: " << getpid() << " tpid: " << this_thread::get_id() << endl;
    for (int64_t i = scraper->start ; i < scraper->stop && !shouldQuit() ; i = i + STEP) {
//        CBlock block;
//        getBlock(block, (uint64_t)i);
//        if (block.transactions.size()) {
        if (true) {
            *scraper << i;
            scraper->flush();
        }
        scraper->prog.fProg = i;
        static int cnt=0;
        if (!(++cnt%REP_FREQ)) { cerr << "."; cerr.flush(); }
        if (!(cnt%REP_FREQ2)) { cerr << "\n" << scraper->prog.bProg << "-" << scraper->prog.fProg; cerr.flush(); }
    }
    scraper->prog.headDone = true;
}

//--------------------------------------------------------------------------
void scanBackward(CBlockScraper *scraper) {
    sleep(1.0);
    cout << "back - pid: " << getpid() << " tpid: " << this_thread::get_id() << endl;
    for (int64_t i = scraper->start ; i > scraper->stop && !shouldQuit() ; i = i - STEP) {
//        CBlock block;
//        getBlock(block, (uint64_t)i);
//        if (block.transactions.size()) {
        if (true) {
            *scraper << i;
            scraper->flush();
        }
        scraper->prog.bProg = i;
        if (scraper->prog.headDone) {
            static int cnt=0;
            if (!(++cnt%REP_FREQ)) { cerr << "."; cerr.flush(); }
            if (!(cnt%REP_FREQ2)) { cerr << "\n" << scraper->prog.bProg << "-" << scraper->prog.fProg; cerr.flush(); }
        }
    }
}

//--------------------------------------------------------------------------
bool appendFileToArray1(CUintArray& array, const string_q& fn) {
    CArchive file(READING_ARCHIVE);
    if (fileExists(fn) && fileSize(fn) > 0) {
        if (!file.Lock(fn, binaryReadOnly, LOCK_NOWAIT))
            return options.usage("Cannot open " + fn + ". Quitting...");
        file >> array;
        file.Release();
    }
    return true;
}

//--------------------------------------------------------------------------
bool appendFileToArray2(CUintArray& array, const string_q& fn) {
    CArchive file(READING_ARCHIVE);
    if (fileExists(fn) && fileSize(fn) > 0) {
        if (!file.Lock(fn, binaryReadOnly, LOCK_NOWAIT))
            return options.usage("Cannot open " + fn + ". Quitting...");
        uint64_t val;
        size_t s = file.Read(val);
        while (s) {
//            cout << "val: " << val << endl;
            array.push_back(val);
            s = file.Read(val);
        }
        file.Release();
    } else {
        cout << "Nothing to append" << endl;
    }
    return true;
}

//--------------------------------------------------------------------------
bool postProcess(void) {
    ASSERT(fileExists("backward.bin"));
    ASSERT(fileExists("forward.bin"));

    CUintArray array;
    if (!appendFileToArray1(array, "output.bin"))
        return false;
    if (!appendFileToArray2(array, "backward.bin"))
        return false;
    if (!appendFileToArray2(array, "forward.bin"))
        return false;

    sort(array.begin(), array.end());

    CArchive outputCache(WRITING_ARCHIVE);
    if (!outputCache.Lock("output.bin", binaryWriteCreate, LOCK_NOWAIT))
        return options.usage("Could not open output.bin. Quitting...");
    outputCache << array;
    outputCache.Release();
    ::remove("backward.bin");
    ::remove("forward.bin");

    return true;
}

int main(int argc, const char *argv[]) {
    etherlib_init(defaultQuitHandler);

    CProgress progress;
    CBlockScraper backward(progress);
    CBlockScraper forward(progress);

    CArchive outputCache(READING_ARCHIVE);
    if (!fileExists("output.bin")) {
        forward.start = (argc == 2 ? str_2_Int(argv[1]) : options.startBlock);
        backward.start = forward.start - 1;
    } else {
        if (!outputCache.Lock("output.bin", binaryReadOnly, LOCK_NOWAIT))
            return options.usage("Could not open output.bin. Quitting...");
        CUintArray array;
        outputCache >> array;
        backward.start = max(0LL, (int64_t)array[0] - 1);
        forward.start = (int64_t)array[array.size()-1] + 1;
        outputCache.Release(); // we don't need it until the processing is done, so close it.
    }
    backward.stop = backward.start - options.maxBlock;
    if (backward.stop < 0)
        backward.stop = 0;
    forward.stop  = forward.start + options.maxBlock;
    if (forward.stop > (int64_t)getLatestBlockFromClient())
        forward.stop = (int64_t)getLatestBlockFromClient();

    progress.fProg = forward.start;
    progress.bProg = backward.start;

    if (fileExists("backward.bin") || fileExists("forward.bin"))
        postProcess();

    if (!backward.Lock("backward.bin", binaryWriteCreate, LOCK_NOWAIT))
        return options.usage("Could not created backward.bin. Quitting...");
    if (!forward.Lock("forward.bin", binaryWriteCreate, LOCK_NOWAIT))
        return options.usage("Could not created forward.bin. Quitting...");

    thread first(scanForward, &forward); forward.thread = &first;
    thread second(scanBackward, &backward); backward.thread = &second;
    first.join();
    second.join();

    postProcess();

    outputCache.Lock("output.bin", binaryReadOnly, LOCK_NOWAIT);
    CUintArray array;
    outputCache >> array;
    cout << endl;
    cout << "backward: " << backward.start << "-" << backward.stop << endl;
    cout << "forward: " << forward.start << "-" << forward.stop << endl;
    cout << "count: " << array.size() << endl;
    for (size_t i = 0 ; i < 5; i++)
        cout << array[i] << endl;
    cout << "..." << endl;
    for (size_t i = array.size()-5 ; i < array.size() ; i++)
        cout << array[i] << endl;

    etherlib_cleanup();

    return 0;
}
