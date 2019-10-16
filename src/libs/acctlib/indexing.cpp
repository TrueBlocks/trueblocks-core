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
#include "indexing.h"

namespace qblocks {

    hash_t versionHash = "0x0000000000000000000000000000000000000000000000000000000000000000";

    //----------------------------------------------------------------
    void writeIndexAsAscii(const string_q& outFn, const CStringArray& lines) {

        ASSERT(!fileExists(outFn));

        address_t prev;
        uint32_t offset = 0, nAddrs = 0, cnt = 0;
        CAppearanceArray_base blockTable;

        cerr << "\tExtracting addresses...";
        ostringstream addrStream;
        for (auto line : lines) {
            CStringArray parts;
            explode(parts, line, '\t');
            CAppearance_base rec(parts[1], parts[2]);
            blockTable.push_back(rec);
            if (!prev.empty() && parts[0] != prev) {
                addrStream << prev << "\t";
                addrStream << padNum6(offset) << "\t";
                addrStream << padNum6(cnt) << endl;
                offset += cnt;
                cnt = 0;
                nAddrs++;
            }
            cnt++;
            prev = parts[0];
        }
        // The above algo always misses the last address, so we add it here
        addrStream << prev << "\t";
        addrStream << padNum6(offset) << "\t";
        addrStream << padNum6(cnt) << endl;
        nAddrs++;

        cerr << endl << "\tExtracting appearances..." << endl;
        ostringstream blockStream;
        for (auto record : blockTable) {
            blockStream << padNum9(record.blk) << "\t";
            blockStream << padNum5(record.txid) << endl;
        }

        cerr << "\tExporting data..." << endl;
        ostringstream headerStream;
        headerStream << padNum7(MAGIC_NUMBER) << "\t";
        headerStream << versionHash << "\t";
        headerStream << padNum7(nAddrs) << "\t";
        headerStream << padNum7((uint32_t)blockTable.size()) << endl;

        cerr << "\tFinalizing data..." << endl;
        lockSection(true);
        stringToAsciiFile(outFn, headerStream.str() + addrStream.str() + blockStream.str());
        lockSection(false);

        cerr << "\t" << greenCheck << " Ascii file created..." << endl;
    }

    //----------------------------------------------------------------
    void writeIndexAsBinary(const string_q& outFn, const CStringArray& lines) {

        // ASSUMES THE ARRAY IS SORTED!

        ASSERT(!fileExists(outFn));
        string_q tmpFile = substitute(outFn, ".bin", ".tmp");

        address_t prev;
        uint32_t offset = 0, nAddrs = 0, cnt = 0;
        CAppearanceArray_base blockTable;

        hashbytes_t hash = hash_2_Bytes(versionHash);

        CNewBloomArray blooms;

        cerr << "\tExtracting addresses...";
        CArchive archive(WRITING_ARCHIVE);
        archive.Lock(tmpFile, modeWriteCreate, LOCK_NOWAIT);
        archive.Seek(0, SEEK_SET);  // write the header even though it's not fully detailed to preserve the space
        archive.Write(MAGIC_NUMBER);
        archive.Write(hash.data(), hash.size(), sizeof(uint8_t));
        archive.Write(nAddrs);
        archive.Write((uint32_t)blockTable.size());  // not accurate yet
        for (size_t l = 0 ; l < lines.size() ; l++) { //auto line : lines) {
            string_q line = lines[l];
            ASSERT(countOf(line, '\t') == 2);
            CStringArray parts;
            explode(parts, line, '\t');
            CAppearance_base rec(parts[1], parts[2]);
            blockTable.push_back(rec);
            if (!prev.empty() && parts[0] != prev) {
                addToSet(blooms, prev);
                addrbytes_t bytes = addr_2_Bytes(prev);
                archive.Write(bytes.data(), bytes.size(), sizeof(uint8_t));
                archive.Write(offset);
                archive.Write(cnt);
                offset += cnt;
                cnt = 0;
                nAddrs++;
            }
            cnt++;
            prev = parts[0];
        }

        // The above algo always misses the last address, so we add it here
        addToSet(blooms, prev);
        addrbytes_t bytes = addr_2_Bytes(prev);
        archive.Write(bytes.data(), bytes.size(), sizeof(uint8_t));
        archive.Write(offset);
        archive.Write(cnt);
        nAddrs++;

        cerr << endl << "\tExtracting appearances..." << endl;
        for (auto record : blockTable) {
            archive.Write(record.blk);
            archive.Write(record.txid);
        }

        cerr << "\tExporting data..." << endl;
        archive.Seek(0, SEEK_SET);  // re-write the header now that we have full data
        archive.Write(MAGIC_NUMBER);
        archive.Write(hash.data(), hash.size(), sizeof(uint8_t));
        archive.Write(nAddrs);
        archive.Write((uint32_t)blockTable.size());
        archive.Release();

        // We've built the data in a temporary file. We do this in case we're interrupted during the building of the
        // data so it's not corrupted. In this way, we only move the data to its final resting place once. It's safer.
        cerr << "\tFinalizing data..." << endl;
        lockSection(true);  // disallow control+c
        string_q bloomFile = substitute(substitute(outFn, "/finalized/", "/blooms/"), ".bin", ".bloom");
        writeNewBloom(bloomFile, blooms);  // write the bloom file
        copyFile(tmpFile, outFn);  // move the index file
        ::remove(tmpFile.c_str());  // remove the tmp file
        lockSection(false);

        cerr << "\t" << greenCheck << " Binary file created..." << endl;
    }

}  // namespace qblocks

