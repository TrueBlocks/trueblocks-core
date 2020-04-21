/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <thread>
#include "etherlib.h"

using namespace std;

//------------------------------------------------------------------------------------------------
class CScannerBucket {
public:
    bool active;
    blknum_t bucket_id;
    thread* thread_ptr;
    CStringArray hits;
    const CStringArray& files;
    CScannerBucket(const CStringArray& f, blknum_t bucket, size_t expectedSize) : active(false), bucket_id(bucket), files(f) {
        hits.reserve(expectedSize);
        thread_ptr = NULL;
    }
};
typedef vector<CScannerBucket> CScannerBucketArray;

//------------------------------------------------------------------------------------------------
string_q bloomPath = configPath("") + "cache/addr_index.save/finalized/";

//------------------------------------------------------------------------------------------------
size_t thread_count(const CScannerBucketArray& buckets) {
    size_t count = 0;
    for (auto bucket : buckets)
        count += (bucket.thread_ptr != NULL);
    return count;
}

#define N_THREADS 4
class COptions {
public:
    size_t nThreads;
    blknum_t lastBlockInFile;
    blkrange_t scanRange;
    COptions(void) : nThreads(N_THREADS), lastBlockInFile(0), scanRange(0,NOPOS) {}
};
COptions opts;

//---------------------------------------------------------------
bool visitFinalIndexFiles(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitFinalIndexFiles, data);

    } else {
        CScannerBucket *bucket = (CScannerBucket*)data;

        // Pick up some useful data for either method...
        COptions* options = reinterpret_cast<COptions*>(&opts);

        // Filenames take the form 'start-end.[txt|bin]' where both 'start' and 'end'
        // are inclusive. Silently skips unknown files in the folder (such as shell scripts).
        if (!contains(path, "-") || !endsWith(path, ".bin"))
            return !shouldQuit();

        timestamp_t ts;
        blknum_t firstBlock = bnFromPath(path, options->lastBlockInFile, ts);
        ASSERT(unused != NOPOS);
        ASSERT(options->lastBlockInFile != NOPOS);

        // If the user told us to start late, start late
        if (options->lastBlockInFile != 0 && options->lastBlockInFile < options->scanRange.first)
            return !shouldQuit();

        // If the user told us to end early, end early
        if (options->scanRange.second != NOPOS && firstBlock > options->scanRange.second)
            return !shouldQuit();

        if (isTestMode() && options->lastBlockInFile > 5000000)
            return !shouldQuit();

        if ((firstBlock % options->nThreads) == bucket->bucket_id) {
            bucket->hits.push_back(path);
        }
        return true;
        //return options->visitBinaryFile(path, data) && !shouldQuit();
    }

    ASSERT(0);  // should not happen
    return !shouldQuit();
}

bool newWay = false;
//------------------------------------------------------------------------------------------------
int processOneBucket(const string_q& path, CScannerBucket *bucket) {
    bucket->active = true;
    if (newWay) {
        for (size_t x = 0 ; x < bucket->files.size() ; x++)
            visitFinalIndexFiles(bucket->files[x], bucket);
    } else {
        forEveryFileInFolder(path, visitFinalIndexFiles, bucket);
    }
    bucket->active = false;
    return true;
}

//------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {

//    for (int i=0; i< argc; i++)
//        cerr << i << ": " << argv[i] << endl;

    if (argc > 1)
        opts.nThreads = str_2_Uint(argv[1]);
    if (argc > 2)
        newWay = true;

//    if (newWay)
//        cerr << argc << " nThreads: " << opts.nThreads << " newWay: " << newWay << endl;

    size_t count = nFilesInFolder(bloomPath);
    blknum_t expected_count = (count / opts.nThreads) * 2;  // double just to be conservative

    CStringArray files;
    listFilesInFolder(files, bloomPath, false);

    for (size_t x = 0 ; x < 40 ; x++) {
        // Create the buckets...
        CScannerBucketArray buckets;
        for (size_t th = 0 ; th < opts.nThreads ; th++)
            buckets.push_back(CScannerBucket(files, th, expected_count));

        // Kick them all off...
        for (size_t b = 0 ; b < buckets.size() ; b++) {
            CScannerBucket *bucket = &buckets[b];
            bucket->thread_ptr = new thread(processOneBucket, bloomPath, bucket);
            printf("");
        }

        // Wait until they finish...
        size_t item = 0;
        for (size_t b = 0 ; b < buckets.size() ; b++) {
            CScannerBucket *bucket = &buckets[b];
            bucket->thread_ptr->join();
            cout << "\tactive: " << bucket->active << endl;
            cout << "\tbucket_id: " << bucket->bucket_id << endl;
            cout << "\thits: " << bucket->hits.size() << endl;
            for (auto hit : bucket->hits)
                cout << "\t\t" << hit << " : " << item++ << endl;
            delete bucket->thread_ptr;
        }
    }

    // Summarize and report
    return 0;
}

/*
 int processBucket_inner(wchar_t *file_name, int file_id, int bucket) {
 #if 0
 char ReadBuffer[BUFFERSIZE + 1];
 char ReadBuffer2[BUFFERSIZE2 + 1];
 long long int len = (long long int )search_len;
 bool partial = false;
 long long int tmplen;
 FILE* fp = NULL;

 errno_t retval = _wfopen_s(&fp, file_name, L"rb");
 if (retval == -1)
 {
 // DisplayError("CreateFile");
 _tprintf("Terminal failure: unable to open file \"%s\" for read.\n", file_name);
 }
 else {
 if (1) { _tprintf("open file \"%s\" for read in thrd %d.\n", file_name, bucket); }
 }
 if (fp) {
 while (1)
 {
 partial = false;
 int read = fread_s(ReadBuffer, BUFFERSIZE, sizeof(char), BUFFERSIZE, fp);
 if (read == -1)
 {
 // DisplayError("CreateFile");
 _tprintf("Terminal failure: unable to read file \"%s\" for read.\n", file_name);
 break;
 }
 else if (read == 0) {
 if (DEBUG) { _tprintf("reached end of file %s\n", file_name); }
 break;
 } else {
 if (DEBUG) { _tprintf("file \"%s\" read %d bytes\n", file_name, read); }
 ReadBuffer[read] = 0;
 if (DEBUG) { cout << "ReadBuffer " << ReadBuffer << endl; }
 char* c = ReadBuffer;
 *(c + read) = *search;
 restart_label:
 while (*c++ != *search);
 //Sleep(1);
 if (c == ReadBuffer + read + 1) continue;
 int offset = c - ReadBuffer - 1;
 if (DEBUG) { cout << "found first char at offset " << offset << endl; }
 tmplen = len;
 if (offset + len >= BUFFERSIZE) {
 tmplen = BUFFERSIZE - offset;
 partial = true;
 }
 if (!strncmp(c - 1, search, tmplen)) {
 if (partial == false) {
 all_list[file_id]->has_string = true;
 if (1) {cout << "#######String found in this file " << file_name << endl; }
 break;
 }
 else {
 size_t diff = len - tmplen;
 if (DEBUG) {
 cout << " len is " << len << " tmplen " << tmplen << endl;
 }
 int read2 = fread_s(ReadBuffer2, BUFFERSIZE2, sizeof(char), diff, fp);
 ReadBuffer2[diff] = 0;
 if (DEBUG) { cout << "ReadBuffer2 " << ReadBuffer2 << endl; }
 if (DEBUG) { cout << "diff " << diff << "retval " << read2 << " search + tmplen " << search + tmplen << endl; }

 if (read2 != diff) continue;
 if (!strncmp(ReadBuffer2, search + tmplen, diff)) {
 all_list[file_id]->has_string = true;
 if (1) { cout << "#######String found2 in this file " << file_name << endl; }
 break;
 }
 else {
 fseek(fp, tmplen - len, SEEK_CUR);
 }
 }
 }
 else {
 partial = false;
 goto restart_label;
 }
 }
 }
 retval = fclose(fp);
 }
 #endif
 return 0;
 }

 */
