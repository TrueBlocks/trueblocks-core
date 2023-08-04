package rpcClient

import (
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/prefunds"
)

// IsNodeArchive returns true if the node is an archive node
func (conn *Connection) IsNodeArchive() bool {
	// TODO: from C++ code
	// const CToml* config = getGlobalConfig("blockScrape");
	// if (!config->getConfigBool("requires", "archive", true))
	//     return true;

	// TODO: chain-specific code - some chains have no pre-allocation
	// An archive node better have a balance at the end of block zero the same as
	// the pre-allocation amount for that account. We use the largest allocation
	// so as to ensure we get an actual balance
	thePath := filepath.Join(config.GetPathToChainConfig(conn.Chain), "allocs.csv")
	largest, err := prefunds.GetLargestPrefund(conn.Chain, thePath)
	if err != nil {
		return false
	}

	bal, err := conn.GetBalanceAt(largest.Address, 0)
	if err != nil {
		return false
	}

	// fmt.Println("Largest:", largest.Balance, "Balance:", *bal, "Cmp:", bal.Cmp(&largest.Balance) == 0)
	return bal.Cmp(&largest.Balance) == 0
}

//-----------------------------------------------------------------------
// bool findLargest(const Allocation& prefund, void* data) {
//     Allocation* largest = (Allocation*)data;
//     if (prefund.amount > largest->amount) {
//         largest->address = prefund.address;
//         largest->amount = prefund.amount;
//     }
//     return true;
// }
//
// //-----------------------------------------------------------------------
// Allocation largestPrefund(void) {
//     if (prefundBalMap.size() == 0)
//         loadPrefundBalances();
//     Allocation largest;
//     forEveryPrefund(findLargest, &largest);
//     return largest;
// }
// extern Allocation largestPrefund(void);
// //-------------------------------------------------------------------------
// bool isArchiveNode(void) {
//     // short curcuit for some situations
//     const CToml* config = getGlobalConfig("blockScrape");
//     if (!config->getConfigBool("requires", "archive", true))
//         return true;
//
//     // An archive node better have a balance at the end of block zero the same as
//     // the allocation amount for that account. We use the largest allocation so as
//     // to ensure we get an actual balance
//     Allocation largest = largestPrefund();
//     return get BalanceAt(largest.address, 0) == largest.amount;
// }
//
// // #if 0
// // //-----------------------------------------------------------------------
// // bool readPrefundBals(void) {
// //     if (!fileExists(cacheFolderBin_allocs))
// //         return false;
//
// //     CArchive archive(READING_ARCHIVE);
// //     if (!archive.Lock(cacheFolderBin_allocs, modeReadOnly, LOCK_NOWAIT))
// //         return false;
//
// //     uint64_t count;
// //     archive >> count;
// //     for (size_t i = 0; i < count; i++) {
// //         wei_t amount;
// //         string_q address;
// //         archive >> address >> amount;
// //         prefundBalMap[address] = amount;
// //     }
// //     archive.Release();
//
// //     return true;
// // }
//
// // //-----------------------------------------------------------------------
// // bool readPrefundAscii(void) {
// //     if (!fileExists(chainConfigsTxt_allocs))
// //         return false;
//
// //     CStringArray lines;
// //     asciiFileToLines(chainConfigsTxt_allocs, lines);
// //     for (auto line : lines) {
// //         if (startsWith(line, "0x")) {
// //             CStringArray parts;
// //             explode(parts, line, '\t');
// //             string_q address = toLower(parts[0]);
// //             wei_t amount = str_2_Wei(parts[1]);
// //             prefundBalMap[address] = amount;
// //         }
// //     }
// //     return true;
// // }
//
// // //-----------------------------------------------------------------------
// // bool writePrefundBin(void) {
// //     CArchive archive(WRITING_ARCHIVE);
// //     if (!archive.Lock(cacheFolderBin_allocs, modeWriteCreate, LOCK_NOWAIT)) {
// //         LOG_WARN("Could not lock prefund cache at: ", cacheFolderBin_allocs);
// //         return false;
// //     }
//
// //     archive << uint64_t(prefundBalMap.size());
// //     for (const auto& item : prefundBalMap)
// //         archive << item.first << item.second;
// //     archive.Release();
//
// //     return true;
// // }
//
// // //-----------------------------------------------------------------------
// // bool loadPrefundBalances(void) {
// //     LOG_TEST_STR("Loading prefund balances");
// //     if (prefundBalMap.size() > 0) {
// //         LOG_TEST_STR("Already loaded");
// //         return true;
// //     }
//
// //     if (readPrefundBals())
// //         return true;
//
// //     if (!readPrefundAscii())
// //         return false;
//
// //     return writePrefundBin();
// // }
//
// // #endif
