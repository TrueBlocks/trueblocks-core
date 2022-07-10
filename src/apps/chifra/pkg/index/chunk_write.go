package index

import (
	"fmt"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/ethereum/go-ethereum/common"
)

type AddressAppearanceMap map[string][]AppearanceRecord

func WriteChunk(chain, path string, addAppMap AddressAppearanceMap, nApps int) (uint64, error) {
	addressTable := make([]AddressRecord, 0, len(addAppMap))
	appearanceTable := make([]AppearanceRecord, 0, nApps)

	sorted := []string{}
	for address := range addAppMap {
		sorted = append(sorted, address)
	}
	sort.Slice(sorted, func(i, j int) bool {
		return sorted[i] < sorted[j]
	})

	fmt.Println()
	fmt.Println("In the map")
	offset := uint32(0)
	for i, addrStr := range sorted {
		apps := addAppMap[addrStr]
		for _, app := range apps {
			fmt.Println(i, addrStr, app.BlockNumber, app.TransactionId)
			appearanceTable = append(appearanceTable, app)
		}
		addressTable = append(addressTable, AddressRecord{
			Address: common.HexToAddress(addrStr),
			Offset:  offset,
			Count:   uint32(len(apps)),
		})
		offset += uint32(len(apps))
	}

	fmt.Println()
	fmt.Println("addressTable")
	for i, addrRec := range addressTable {
		fmt.Println(i, addrRec)
	}

	fmt.Println()
	fmt.Println("appearanceTable")
	for i, appRec := range appearanceTable {
		fmt.Println(i, appRec)
	}

	r := strings.Replace(path, config.GetPathToIndex(chain), "$INDEX/", -1)
	fmt.Println("Wrote", len(addAppMap), "records to", r)

	return 0, nil
}

/*
	//----------------------------------------------------------------
	bool writeIndexAsBinary(const string_q& outFn, const CStringArray& lines) {
	    ASSERT(!fileExists(outFn));
	    string_q tmpFile2 = substitute(outFn, ".bin", ".tmp");

	    address_t prev;
	    uint32_t offset = 0, nAddrs = 0, cnt = 0;
	    CIndexedAppearanceArray appTable;

	    hashbytes_t hash = hash_2_Bytes(padLeft(keccak256(manifestVersion), 64, '0'));
	    CArchive archive(WRITING_ARCHIVE);
	    if (!archive.Lock(tmpFile2, modeWriteCreate, LOCK_NOWAIT)) {
	        LOG_ERR("Could not lock index file: ", tmpFile2);
	        return false;
	    }

	    archive.Seek(0, SEEK_SET);  // write the header even though it's not fully detailed to preserve the space
	    archive.Write(MAGIC_NUMBER);
	    archive.Write(hash.data(), hash.size(), sizeof(uint8_t));
	    archive.Write(nAddrs);
	    archive.Write((uint32_t)appTable.size());  // not accurate yet

	    CBloomFilterWrite bloomFilter;
	    for (size_t l = 0; l < lines.size(); l++) {
	        string_q line = lines[l];
	        ASSERT(countOf(line, '\t') == 2);
	        CStringArray parts;
	        explode(parts, line, '\t');
	        CIndexedAppearance rec(parts[1], parts[2]);
	        appTable.push_back(rec);
	        if (!prev.empty() && parts[0] != prev) {
	            bloomFilter.addToSet(prev);
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

	    bloomFilter.addToSet(prev);
	    addrbytes_t bytes = addr_2_Bytes(prev);
	    archive.Write(bytes.data(), bytes.size(), sizeof(uint8_t));
	    archive.Write(offset);
	    archive.Write(cnt);
	    nAddrs++;

	    for (auto record : appTable) {
	        archive.Write(record.blk);
	        archive.Write(record.txid);
	    }

	    archive.Seek(0, SEEK_SET);  // re-write the header now that we have full data
	    archive.Write(MAGIC_NUMBER);
	    archive.Write(hash.data(), hash.size(), sizeof(uint8_t));
	    archive.Write(nAddrs);
	    archive.Write((uint32_t)appTable.size());
	    archive.Release();

	    string_q bloomFile = substitute(substitute(outFn, "/finalized/", "/blooms/"), ".bin", ".bloom");
	    lockSection();                            // disallow control+c
	    bloomFilter.writeBloomFilter(bloomFile);  // write the bloom file
	    copyFile(tmpFile2, outFn);                // move the index file
	    ::remove(tmpFile2.c_str());               // remove the tmp file
	    unlockSection();

	    string_q range = substitute(substitute(outFn, indexFolder_finalized, ""), indexFolder_blooms, "");
	    range = substitute(substitute(range, ".bin", ""), ".bloom", "");
	    appendToAsciiFile(cacheFolder_tmp + "chunks_created.txt", range + "\n");

	    return !shouldQuit();
	}
	bool CBloomFilterRead::writeBloomFilter(const string_q& fileName) {
	    lockSection();
	    CArchive output(WRITING_ARCHIVE);
	    if (!output.Lock(fileName, modeWriteCreate, LOCK_NOWAIT)) {
	        unlockSection();
	        return false;
	    }
	    output.Write((uint32_t)array.size());
	    for (auto bloom : array) {
	        output.Write(bloom.nInserted);
	        output.Write(bloom.bits, sizeof(uint8_t), BLOOM_WIDTH_IN_BYTES);
	    }
	    output.Release();
	    unlockSection();
	    return true;
	}
*/
