// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) HandleNeighbors(rCtx *output.RenderCtx, monitorArray []monitor.Monitor) error {
	testMode := opts.Globals.TestMode
	nErrors := 0
	filter := filter.NewFilter(
		opts.Reversed,
		opts.Reverted,
		opts.Fourbytes,
		base.BlockRange{First: opts.FirstBlock, Last: opts.LastBlock},
		base.RecordRange{First: opts.FirstRecord, Last: opts.GetMax()},
	)

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		for _, mon := range monitorArray {
			if apps, cnt, err := mon.ReadAndFilterAppearances(filter, false /* withCount */); err != nil {
				errorChan <- err
				rCtx.Cancel()

			} else if cnt == 0 {
				errorChan <- fmt.Errorf("no blocks found for the query")
				continue

			} else {
				if sliceOfMaps, _, err := types.AsSliceOfMaps[bool](apps, filter.Reversed); err != nil {
					errorChan <- err
					rCtx.Cancel()

				} else {
					showProgress := opts.Globals.ShowProgress()
					bar := logger.NewBar(logger.BarOptions{
						Prefix:  mon.Address.Hex(),
						Enabled: showProgress,
						Total:   int64(cnt),
					})

					// TODO: BOGUS - THIS IS NOT CONCURRENCY SAFE
					finished := false
					for _, thisMap := range sliceOfMaps {
						if finished {
							continue
						}

						for app := range thisMap {
							thisMap[app] = new(bool)
						}

						neighbors := make([]Reason, 0)
						iterFunc := func(app types.Appearance, unused *bool) error {
							if theseNeighbors, err := GetNeighbors(&app); err != nil {
								return err
							} else {
								neighbors = append(neighbors, theseNeighbors...)
								return nil
							}
						}

						iterErrorChan := make(chan error)
						iterCtx, iterCancel := context.WithCancel(context.Background())
						defer iterCancel()
						go utils.IterateOverMap(iterCtx, iterErrorChan, thisMap, iterFunc)
						for err := range iterErrorChan {
							if !testMode || nErrors == 0 {
								errorChan <- err
								nErrors++
							}
						}

						items := make([]types.Appearance, 0, len(thisMap))
						for _, neighbor := range neighbors {
							app := types.Appearance{
								Address:          *neighbor.Address,
								BlockNumber:      neighbor.App.BlockNumber,
								TransactionIndex: neighbor.App.TransactionIndex,
								Reason:           neighbor.Reason,
							}
							items = append(items, app)
						}

						sort.Slice(items, func(i, j int) bool {
							if opts.Reversed {
								i, j = j, i
							}
							if items[i].BlockNumber == items[j].BlockNumber {
								if items[i].TransactionIndex == items[j].TransactionIndex {
									return items[i].Address.Hex() < items[j].Address.Hex()
								}
								return items[i].TransactionIndex < items[j].TransactionIndex
							}
							return items[i].BlockNumber < items[j].BlockNumber
						})

						for _, item := range items {
							var passes bool
							passes, finished = filter.ApplyCountFilter()
							if passes {
								modelChan <- &item
							}
							if finished {
								break
							}
						}
					}
					bar.Finish(true /* newLine */)
				}
			}
		}
	}

	extraOpts := map[string]any{
		"uniq": true,
	}

	return output.StreamMany(rCtx.Ctx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}

/*
#include "options.h"

//----------------------------------------------------------------
bool showApp(const CAppearance& item, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    cout << ((isJson() && !opt->firstOut) ? ", " : "");
    cout << item;
    opt->firstOut = false;
    return !shouldQuit();
}

extern bool getChunkRanges(CBlockRangeArray& ranges);

//-----------------------------------------------------------------------
int compareEntry(const CReverseAppMapEntry* a, const CReverseAppMapEntry* b) {
    int ret = int(a->blk) - int(b->blk);
    if (ret)
        return ret;
    ret = int(a->tx) - int(b->tx);
    if (ret)
        return ret;
    return 0;
}

//----------------------------------------------------------------
int findRevMapEntry(const void* v1, const void* v2) {
    const CReverseAppMapEntry* at1 = (CReverseAppMapEntry*)v1;  // NOLINT
    const CReverseAppMapEntry* at2 = (CReverseAppMapEntry*)v2;  // NOLINT
    return compareEntry(at1, at2);
}

//----------------------------------------------------------------
bool isSame(const CReverseAppMapEntry* a, const CReverseAppMapEntry* b) {
    return !compareEntry(a, b);
}

//-----------------------------------------------------------------------
bool assignReason(const CName& accountedFor, CAppearance& app, const CTransaction& trans) {
    if (app.transactionIndex > types.ExternalReward) {  // leave this here for searching: types.BlockReward
        app.reason = "miner";
        return true;
    }
    if (app.address == trans.from) {
        app.reason = "from";
        return true;
    }
    if (app.address == trans.to) {
        app.reason = "to";
        return true;
    }
    if (app.address == trans.receipt.contractAddress) {
        app.reason = "creation";
        return true;
    }
    bool junk = false;
    string_q ss = substitute(app.address, "0x", "");
    if (contains(trans.input, ss)) {
        app.reason = "input";
        if (accountedFor.address == app.address) {
            return true;
        }
        junk = true;
    }

    for (size_t i = 0; i < trans.receipt.logs.size(); i++) {
        const CLog* l = &trans.receipt.logs[i];
        if (l->address == app.address) {
            app.reason = "log_" + uint_2_Str(i) + "_generator";
            return true;
        }
        if (contains(l->topics[0], ss)) {
            app.reason = "log_" + uint_2_Str(i) + "_topic_" + uint_2_Str(0);
            return true;
        }
        for (size_t j = 1; j < l->topics.size(); j++) {
            if (contains(l->topics[j], ss)) {
                app.reason = "log_" + uint_2_Str(i) + "_topic_" + uint_2_Str(j);
                if (accountedFor.address == app.address) {
                    return true;
                }
                junk = true;
            }
        }
        if (contains(l->data, ss)) {
            app.reason = "log_" + uint_2_Str(i) + "_data";
            if (accountedFor.address == app.address) {
                return true;
            }
            junk = true;
        }
    }

    if (!junk) {
        app.reason = "trace";
    }
    return true;  // !junk;
}

//-----------------------------------------------------------------------
CIndexArchiveWithNeighborMaps::~CIndexArchiveWithNeighborMaps() {
    clean();
}

//-----------------------------------------------------------------------
int sortRecords(const void* i1, const void* i2) {
    int32_t* p1 = (int32_t*)i1;
    int32_t* p2 = (int32_t*)i2;
    if (p1[1] == p2[1]) {
        if (p1[2] == p2[2]) {
            return (p1[0] - p2[0]);
        }
        return p1[2] - p2[2];
    }
    return p1[1] - p2[1];
}

//-----------------------------------------------------------------------
bool CIndexArchiveWithNeighborMaps::LoadReverseMaps(const blkrange_t& range) {
    if (reverseAppMap) {
        delete[] reverseAppMap;
        reverseAddrRanges.clear();
        reverseAppMap = nullptr;
    }

    uint32_t nAppsHere = header.nApps;

    string_q mapFile = substitute(getFilename(), indexFolder_finalized, indexFolder_map);
    if (fileExists(mapFile)) {
        CArchive archive(READING_ARCHIVE);
        if (!archive.Lock(mapFile, modeReadOnly, LOCK_NOWAIT)) {
            LOG_ERR("Could not open file ", mapFile);
            return false;
        }
        size_t nRecords = fileSize(mapFile) / sizeof(CReverseAppMapEntry);
        ASSERT(nRecords == nAppsHere);
        // Cleaned up on destruction of the chunk
        reverseAppMap = new CReverseAppMapEntry[nRecords];
        if (!reverseAppMap) {
            LOG_ERR("Could not allocate memory for CReverseAppMapEntry");
            return false;
        }
        archive.Read((char*)reverseAppMap, sizeof(char), nRecords * sizeof(CReverseAppMapEntry));
        archive.Release();
        for (uint32_t i = 0; i < header.nAddrs; i++) {
            reverseAddrRanges.push_back(getAppRangeForAddrAt(i));
        }
        return true;
    }

    // Cleaned up on destruction of the chunk
    reverseAppMap = new CReverseAppMapEntry[nAppsHere];
    if (!reverseAppMap) {
        LOG_ERR("Could not allocate memory for CReverseAppMapEntry");
        return false;
    }
    for (uint32_t i = 0; i < nAppsHere; i++) {
        reverseAppMap[i].n = i;
        CIndexedAppearance* app = getAppearanceAt(i);
        reverseAppMap[i].blk = app->blk;
        reverseAppMap[i].tx = app->txid;
    }

    for (uint32_t i = 0; i < header.nAddrs; i++) {
        reverseAddrRanges.push_back(getAppRangeForAddrAt(i));
    }

    qsort(reverseAppMap, nAppsHere, sizeof(CReverseAppMapEntry), sortRecords);

    CArchive archive(WRITING_ARCHIVE);
    if (!archive.Lock(mapFile, modeWriteCreate, LOCK_WAIT)) {
        LOG_ERR("Could not open file ", mapFile);
        return false;
    }
    archive.Write(reverseAppMap, sizeof(char), nAppsHere * sizeof(CReverseAppMapEntry));
    archive.Release();

    return true;
}

//-----------------------------------------------------------------------
bool COptions::showAddrsInTx(CTraverser* trav, const blkrange_t& range, const CAppearance_mon& app) {
    string_q fn = range_2_Str(range);
    string_q chunkPath = indexFolder_finalized + fn + ".bin";

    if (!theIndex || theIndex->getFilename() != chunkPath) {
        if (theIndex) {
            delete theIndex;
            theIndex = nullptr;
        }
        theIndex = new CIndexArchiveWithNeighborMaps(READING_ARCHIVE);
        if (theIndex->ReadIndexFromBinary(chunkPath, IP_ALL)) {
            theIndex->LoadReverseMaps(range);
            if (!theIndex->reverseAppMap) {
                LOG_ERR("Could not allocate reverseAppMap");
                return false;
            }
        } else {
            LOG_WARN("Cannot open index file ", chunkPath);
        }
    }

    CReverseAppMapEntry search;
    search.blk = app.blk;
    search.tx = app.txid;
    CReverseAppMapEntry* found = (CReverseAppMapEntry*)bsearch(&search, theIndex->reverseAppMap, theIndex->header.nApps,
                                                               sizeof(CReverseAppMapEntry), findRevMapEntry);

    if (found) {
        trav->app = &app;
        loadTx_Func(trav, this);

        while (found > theIndex->reverseAppMap) {
            found--;
            if (!isSame(found, &search)) {
                found++;
                break;
            }
        }

        size_t start = 0;
        CReverseAppMapEntry* endOfRevMap =
            theIndex->reverseAppMap + (theIndex->header.nApps * sizeof(CReverseAppMapEntry));
        while (found < endOfRevMap && isSame(found, &search)) {
            for (size_t i = start; i < theIndex->reverseAddrRanges.size(); i++) {
                blkrange_t* r = &theIndex->reverseAddrRanges[i];
                if (inRange(found->n, *r)) {
                    CAppearance appHere;
                    appHere.blockNumber = found->blk;
                    appHere.transactionIndex = found->tx;
                    appHere.address = bytes_2_Addr(theIndex->getAddressAt(i)->bytes);
                    if (assignReason(ledgerManager.name, appHere, trav->trans)) {
                        trav->n Processed++;
                        if (!prog_Log(trav, this))
                            return false;
                        showApp(appHere, this);
                    }
                    start = i;
                    break;
                }
            }
            found++;
        }
    } else {
        LOG_ERR("Appearance (", app.blk, ".", app.txid, ") for address \"", ledgerManager.accountedFor,
                "\" not found in ", chunkPath);
    }

    return prog_Log(trav, this);
}

//-----------------------------------------------------------------------
// For neighbors we handle the entire production of data in the _Pre
// because what we actually want to do is scan across the index chunks
bool neighbors_Pre(CTraverser* trav, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);

    CBlockRangeArray ranges;
    getChunkRanges(ranges);

    uint64_t curRange = 0;
    for (size_t i = 0; i < opt->ledgerManager.appArray.size() && !shouldQuit(); i++) {
        CAppearance_mon app = opt->ledgerManager.appArray[i];
        while (curRange < ranges.size() && !inRange(blknum_t(app.blk), ranges[curRange])) {
            curRange++;
        }
        if (curRange < ranges.size()) {
            trav->index = i;
            if (!opt->showAddrsInTx(trav, ranges[curRange], app))
                return false;
            if (curRange > 0) {
                curRange--;  // back up one in case the next appearances is in the same range
            }
        }
    }

    if (opt->theIndex) {
        // LOG_INFO(bYellow, "Clearing the reverse map for ", opt->theIndex->getFilename(), cOff);
        delete opt->theIndex;
        opt->theIndex = nullptr;
    } else {
        LOG_INFO(bYellow, "Clearing the reverse map", cOff);
    }

    post_Func(trav, data);
    // LOG_INFO("   Done...");
    return false;  // !shouldQuit();
}

// //-----------------------------------------------------------------------
// bool neighbors_Display(CTraverser* trav, void* data) {
//     Note that we do all the processing in the 'neighbors_Pre' function
// }

//-----------------------------------------------------------------------
size_t neighbors_Count(CTraverser* trav, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    return opt->neighborCount;
}

extern bool visitBloom(const string_q& path, void* data);
//-----------------------------------------------------------------------
bool getChunkRanges(CBlockRangeArray& ranges) {
    forEveryFileInFolder(indexFolder_blooms + "*", visitBloom, &ranges);
    // LOG_INFO("Found ", ranges.size(), " chunks");
    return true;
}

//-----------------------------------------------------------------------
bool visitBloom(const string_q& path, void* data) {
    if (endsWith(path, ".bloom")) {
        CBlockRangeArray* ranges = (CBlockRangeArray*)data;
        blkrange_t range = str_2_Range(substitute(path, indexFolder_blooms, ""));
        ranges->push_back(range);
    }
    return true;
}

//-----------------------------------------------------------------------
void COptions::addNeighbor(CAddressUintMap& map, const address_t& addr) {
    if ((addr == ledgerManager.accountedFor || isZeroAddr(addr)))
        return;
    map[addr]++;
}

//-----------------------------------------------------------------------
void COptions::markNeighbors(const CTransaction& trans) {
    addNeighbor(fromAddrMap, trans.from);
    addNeighbor(toAddrMap, trans.to);
    for (auto log : trans.receipt.logs) {
        if (logFilter.emitters.size())
            emitterAddrMap[log.address]++;
    }
    for (auto trace : trans.traces) {
        fromTraceAddrMap[trace.action.from]++;
        toTraceAddrMap[trace.action.to]++;
        if (factory)
            creationMap[trace.result.address]++;
    }
}

//-----------------------------------------------------------------------
class CNameStats {
  public:
    address_t address;
    string_q tags;
    string_q name;
    uint64_t count;
    bool isCustom;
    bool isContract;
    bool isErc20;
    bool isErc721;
    explicit CNameStats(const CName& acct, uint64_t c = 0) {
        address = acct.address;
        tags = acct.tags;
        name = acct.name;
        isCustom = acct.isCustom;
        isContract = acct.isContract;
        isErc20 = acct.isErc20;
        isErc721 = acct.isErc721;
        count = c;
    }

  private:
    CNameStats() {
    }
};
typedef vector<CNameStats> CNameStatsArray;

//-------------------------------------------------------------------------
inline bool operator<(const CNameStats& v1, const CNameStats& v2) {
    if (v1.count != v2.count)
        return v1.count > v2.count;  // We want to sort reverse by count
    return v1.address < v2.address;
}

//-------------------------------------------------------------------------
bool doOne(COptions* options, const CAddressUintMap& theMap, const string_q& type) {
    if (theMap.size() == 0)
        return false;

    if (get EnvStr("HIDE_NAMES") == "true") {
        return false;
    }

    CNameStatsArray unnamed;
    CNameStatsArray named;
    for (auto addr : theMap) {
        CName acct;
        acct.address = addr.first;
        findName(addr.first, acct);
        if (acct.name.empty()) {
            CNameStats stats(acct, addr.second);
            unnamed.push_back(stats);
        } else {
            CNameStats stats(acct, addr.second);
            named.push_back(stats);
        }
    }

    {
        sort(named.begin(), named.end());
        ostringstream os;
        bool frst = true;
        os << ", \"named" << type << "\": {";
        for (auto stats : named) {
            if (!frst)
                os << ",";
            os << "\"" << stats.address << "\": { ";
            os << "\"tags\": \"" << stats.tags << "\", ";
            os << "\"name\": \"" << stats.name << "\", ";
            if (stats.isContract)
                os << "\"isContract\": true, ";
            if (stats.isErc20)
                os << "\"isErc20\": true, ";
            if (stats.isErc721)
                os << "\"isErc721\": true, ";
            os << "\"count\": " << stats.count << " }";
            frst = false;
        }
        os << "}\n";
        expContext().fmtMap["meta"] += os.str();
    }

    {
        sort(unnamed.begin(), unnamed.end());
        ostringstream os;
        os << ", \"unNamed" << type << "\": {";
        bool frst = true;
        for (auto stats : unnamed) {
            if (!frst)
                os << ",";
            os << "\"" << stats.address << "\": " << stats.count;
            frst = false;
        }
        os << "}";
        expContext().fmtMap["meta"] += os.str();
    }
    return true;
}

//-----------------------------------------------------------------------
bool COptions::reportNeighbors(void) {
    // BOGUS: NEW_CO DE
    if (isTestMode()) {
        return true;
    }

    doOne(this, fromAddrMap, "From");
    doOne(this, toAddrMap, "To");
    if (logFilter.emitters.size() > 0)
        doOne(this, emitterAddrMap, "Emitters");
    if (factory)
        doOne(this, creationMap, "Creations");
    if (traces) {
        doOne(this, fromTraceAddrMap, "TraceFrom");
        doOne(this, toTraceAddrMap, "TraceTo");
    }
    return true;
}

typedef struct CReverseAppMapEntry {
  public:
    uint32_t n;
    uint32_t blk;
    uint32_t tx;
} CReverseAppMapEntry;

//---------------------------------------------------------------------------
class CIndexArchiveWithNeighborMaps : public CIndexArchive {
  public:
    CBlockRangeArray reverseAddrRanges;
    CReverseAppMapEntry* reverseAppMap{nullptr};
    explicit CIndexArchiveWithNeighborMaps(bool mode) : CIndexArchive(mode) {
        reverseAppMap = nullptr;
    }
    ~CIndexArchiveWithNeighborMaps();
    bool LoadReverseMaps(const blkrange_t& range);

  private:
    void clean(void) {
        if (reverseAppMap) {
            delete[] reverseAppMap;
            reverseAppMap = nullptr;
        }
        reverseAddrRanges.clear();
    }
};

*/

func GetNeighbors(app *types.Appearance) ([]Reason, error) {
	reasons := make([]Reason, 0)
	reasons = append(reasons, Reason{App: app, Address: &app.Address, Reason: "self"})
	return reasons, nil
}

type Reason struct {
	App     *types.Appearance
	Address *base.Address
	Reason  string
}
