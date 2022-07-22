package index

import (
	"encoding/binary"
	"fmt"
	"io"
	"log"
	"os"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/ethereum/go-ethereum/crypto"
)

type AddressAppearanceMap map[string][]AppearanceRecord

func WriteChunk(chain, indexPath string, addAppMap AddressAppearanceMap, nApps int, pin bool) (uint64, error) {
	addressTable := make([]AddressRecord, 0, len(addAppMap))
	appearanceTable := make([]AppearanceRecord, 0, nApps)

	sorted := []string{}
	for address := range addAppMap {
		sorted = append(sorted, address)
	}
	sort.Slice(sorted, func(i, j int) bool {
		return sorted[i] < sorted[j]
	})

	bl := bloom.ChunkBloom{}

	offset := uint32(0)
	for _, addrStr := range sorted {
		apps := addAppMap[addrStr]
		appearanceTable = append(appearanceTable, apps...)
		address := common.HexToAddress(addrStr)
		bl.AddToSet(address)
		addressTable = append(addressTable, AddressRecord{
			Address: address,
			Offset:  offset,
			Count:   uint32(len(apps)),
		})
		offset += uint32(len(apps))
	}

	rel := strings.Replace(indexPath, config.GetPathToIndex(chain), "$INDEX/", -1)
	// TODO: BOGUS - TESTING SCRAPING
	if utils.OnOff {
		// We have everything we need here, properly sorted with all fields completed
		fmt.Println("Writing", rel)
		fmt.Printf("%x,%s,%d,%d\n", file.MagicNumber, hexutil.Encode(crypto.Keccak256([]byte(version.ManifestVersion))), len(addressTable), len(appearanceTable))
		for _, addrRec := range addressTable {
			fmt.Println(addrRec.Address, addrRec.Offset, addrRec.Count)
		}
		for _, appRec := range appearanceTable {
			fmt.Println(appRec.BlockNumber, appRec.TransactionId)
		}
		fmt.Println("In WriteIndex", indexPath)
	}

	// TODO: BOGUS - YIKES!
	tempPath := strings.Replace(indexPath, "unchained/sepolia/finalized/", "cache/sepolia/tmp/", -1)
	tempPath = strings.Replace(tempPath, "unchained/gnosis/finalized/", "cache/gnosis/tmp/", -1)
	tempPath = strings.Replace(tempPath, "unchained/polygon/finalized/", "cache/polygon/tmp/", -1)
	if indexPath == tempPath {
		log.Fatal("Paths should differ:", tempPath, indexPath)
	}
	fp, err := os.OpenFile(tempPath, os.O_WRONLY|os.O_CREATE, 0644)
	defer func() {
		os.Remove(tempPath)
		// sigintTrap.Disable(trapCh)
		// writeMutex.Lock()
	}()
	if err != nil {
		return 0, err
	}

	fp.Seek(0, io.SeekStart)
	header := HeaderRecord{
		Magic:           file.MagicNumber,
		Hash:            common.BytesToHash(crypto.Keccak256([]byte(version.ManifestVersion))),
		AddressCount:    uint32(len(addressTable)),
		AppearanceCount: uint32(len(appearanceTable)),
	}
	err = binary.Write(fp, binary.LittleEndian, header)
	if err != nil {
		fp.Close()
		return 0, err
	}
	err = binary.Write(fp, binary.LittleEndian, addressTable)
	if err != nil {
		fp.Close()
		return 0, err
	}
	err = binary.Write(fp, binary.LittleEndian, appearanceTable)
	if err != nil {
		fp.Close()
		return 0, err
	}
	// Don't defer this because we want it to be closed before we copy it
	fp.Close()

	fmt.Println("Wrote", len(addAppMap), "records to", rel)
	// TODO: BOGUS - TESTING SCRAPING
	if utils.OnOff {
		nBlooms, nInserted, nBitsLit, nBitsNotLit, sz, bitsLit := bl.GetStats()
		fmt.Println("nBlooms:    ", nBlooms)
		fmt.Println("nInserted:  ", nInserted)
		fmt.Println("nBitsLit:   ", nBitsLit)
		fmt.Println("nBitsNotLit:", nBitsNotLit)
		fmt.Println("sz:         ", sz)
		fmt.Println("bitsLit:    ", bitsLit)
	}

	os.Remove(indexPath)
	_, err = file.Copy(tempPath, indexPath)
	if err != nil {
		return 0, err
	}

	err = bl.WriteBloom(chain, bloom.ToBloomPath(indexPath))
	if err != nil {
		return 0, err
	}

	if pin {
		// TODO: BOGUS - PINNING WHEN WRITING IN GOLANG
		rng := "000000000-000000000"
		newPinsFn := config.GetPathToCache(chain) + "tmp/chunks_created.txt"
		file.AppendToAsciiFile(newPinsFn, rng+"\n")
	}

	return 0, nil
}

/*
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

	    CBloomFilterWrit e bloomFilter;
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
	    bloomFilter.write BloomFilter(bloomFile);  // write the bloom file
	    copyFile(tmpFile2, outFn);                // move the index file
	    ::remove(tmpFile2.c_str());               // remove the tmp file
	    unlockSection();

	    string_q range = substitute(substitute(outFn, indexFolder_finalized, ""), indexFolder_blooms, "");
	    range = substitute(substitute(range, ".bin", ""), ".bloom", "");
	    appendToAsciiFile(cacheFolder_tmp + "chunks_created.txt", range + "\n");

	    return !shouldQuit();
	bool CBloomFi lterRead::write BloomFilter(const string_q& fileName) {
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

type Renderer interface {
	RenderObject(data interface{}, first bool) error
}

func TestWrite(chain, path string, rend Renderer) {
	path = ToIndexPath(path)

	indexChunk, err := NewChunkData(path)
	if err != nil {
		fmt.Println(err.Error())
		return
	}
	defer indexChunk.Close()

	_, err = indexChunk.File.Seek(int64(HeaderWidth), io.SeekStart)
	if err != nil {
		fmt.Println(err.Error())
		return
	}

	addrAppMap := make(AddressAppearanceMap, indexChunk.Header.AddressCount)
	for i := 0; i < int(indexChunk.Header.AddressCount); i++ {
		obj := AddressRecord{}
		err := obj.ReadAddress(indexChunk.File)
		if err != nil {
			fmt.Println(err.Error())
			return
		}
		apps, err := indexChunk.ReadAppearanceRecordsAndResetOffset(&obj)
		if err != nil {
			fmt.Println(err.Error())
			return
		}
		addr := hexutil.Encode(obj.Address.Bytes()) // a lowercase string
		addrAppMap[addr] = append(addrAppMap[addr], apps...)
	}
	WriteChunk(chain, path, addrAppMap, len(addrAppMap), false)
}
