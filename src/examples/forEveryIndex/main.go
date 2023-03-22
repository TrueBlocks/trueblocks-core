package main

import (
	"fmt"
	"io/fs"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
)

func main() {
	// Figure out where the finalized index chunks are...
	path := config.GetPathToIndex("mainnet")

	// Walk all the finalized chunks...
	filepath.Walk(path+"finalized/", func(indexPath string, info fs.FileInfo, err error) error {
		// We only want to view the index chunks...
		if strings.HasSuffix(indexPath, ".bin") {
			c, _ := index.NewChunk(indexPath)
			fmt.Println(c)
			fmt.Println("---------------------------------------------------------")
			fmt.Println()

			cd, _ := index.NewChunkData(indexPath)
			fmt.Println(cd)
			fmt.Println("---------------------------------------------------------")
			fmt.Println()

			b, _ := bloom.NewChunkBloom(cache.ToBloomPath(indexPath))
			fmt.Println(b)
			fmt.Println("---------------------------------------------------------")
			fmt.Println()
		}
		return nil
	})
}

//     size_t missed = 0, nFp = 0;  // number of false positives
//     for (size_t i = 0; i < chunk.header.nAddrs; i++) {
//         bool hit = bloomFilter.Is MemberOf(addrs[i].bytes);
//         if (!hit) {
//             LOG_WARN("Address should be in bloom: ", bytes_2_Addr(addrs[i].bytes));
//             missed++;
//         } else {
//             if (!(i % 57)) {
//                 LOG_INFO(v->range.first, ":", v->range.second, " ", i, " ", chunk.header.nAddrs, " ", missed, "\r");
//             }
//         }
//         testBloom.addToSet(bytes_2_Addr(addrs[i].bytes));
//         alterAddress(addrs[i].bytes, int(i));
//         hit = bloomFilter.Is MemberOf(addrs[i].bytes);
//         if (hit) {
//             nFp++;
//         } else {
//             if (!(i % 57)) {
//                 LOG_INFO("Pass 1 ", v->range.first, ":", v->range.second, " ", i, " ", chunk.header.nAddrs, " ", nFp,
//                          "\r");
//             }
//         }
//     }
//     if (testBloom != bloomFilter) {
//         LOG_WARN("Bloom created is different from the bloom read from disc ", bloomPath);
//     }
//     chunk.Seek(sizeof(CIndexHeader), SEEK_SET);
//     nRead = chunk.Read(addrs, sizeof(CIndexedAddress) * chunk.header.nAddrs, sizeof(char));
//     if (nRead != (sizeof(CIndexedAddress) * chunk.header.nAddrs)) {
//         LOG_WARN("Could not read addresses from ", indexPath);
//         return false;
//     }
//     size_t missed2 = 0;  // number of false positives
//     for (size_t i = 0; i < chunk.header.nAddrs; i++) {
//         bool hit = testBloom.Is MemberOf(addrs[i].bytes);
//         if (!hit) {
//             LOG_WARN("Address should be in bloom: ", bytes_2_Addr(addrs[i].bytes));
//             missed2++;
//         } else {
//             if (!(i % 57)) {
//                 LOG_INFO("Pass 2 ", v->range.first, ":", v->range.second, " ", i, " ", chunk.header.nAddrs, " ",
//                          missed2, "    \r");
//             }
//         }
//     }
//     blkrange_t* overall = (blkrange_t*)v->callData;
//     overall->first += nFp;
//     overall->second += chunk.header.nAddrs;
//     double pct = (double(nFp) / double(chunk.header.nAddrs)) * 100.;
//     double oPct = (double(overall->first) / double(overall->second)) * 100.;
//     cout << "range: " << padNum9(v->range.first) << "-" << padNum9(v->range.second);
//     cout << " tested: " << padNum7T(uint64_t(chunk.header.nAddrs));
//     cout << " missed: " << missed << " fp: " << padNum4T(uint64_t(nFp));
//     cout << " pct: " << double_2_Str(pct, 2) << "%";
//     cout << " overall: " << double_2_Str(oPct, 2) << "%" << endl;
//     delete[] addrs;
//     return true;
// }

// //----------------------------------------------------------------
// int main(int argc, const char* argv[]) {
//     loadEnvironmentPaths("mainnet", "/Users/jrush/Development/trueblocks-core/build/shit/unchained/", "");
//     etherlib_init(quickQuitHandler);
//     blkrange_t overall;
//     overall.first = 218419;
//     overall.second = 26844586;
//     forEveryIndexChunk(visitIndexChunk, &overall);
//     etherlib_cleanup();
//     return 0;
// }
// //----------------------------------------------------------------
// void alterAddress(uint8_t bytes[20], int inc) {
//     for (size_t i = 0; i < 20; i++) {
//         bytes[i] += uint8_t(i);
//     }
// }
