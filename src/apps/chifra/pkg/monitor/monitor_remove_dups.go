// TODO: BOGUS -- Check out develop -- run chifra list <addr> -- check out update -- re-run -- should remove the old monitor files
package monitor

import (
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

func (mon *Monitor) RemoveDups() (cntBefore uint32, cntAfter uint32, err error) {
	if mon.GetAddrStr() != "0xf503017d7baf7fbc0fff7492b751025c6a78179b" {
		return mon.Count(), mon.Count(), nil
	}
	defer mon.Close()

	cntBefore = mon.Count()
	cntAfter = cntBefore
	if cntBefore > 0 {
		apps := make([]index.AppearanceRecord, mon.Count())
		deDupped := make([]index.AppearanceRecord, 0, mon.Count())
		err = mon.ReadAppearances(&apps)
		if err != nil {
			return
		}

		sort.Slice(apps, func(i, j int) bool {
			si := uint64(apps[i].BlockNumber)
			si = (si << 32) + uint64(apps[i].TransactionId)
			sj := uint64(apps[j].BlockNumber)
			sj = (sj << 32) + uint64(apps[j].TransactionId)
			return si < sj
		})

		deDupped = append(deDupped, apps[0])
		prev := apps[0]
		for i, app := range apps {
			if i > 0 {
				fmt.Println(app, prev)
				if prev.BlockNumber != app.BlockNumber || prev.TransactionId != app.TransactionId {
					deDupped = append(deDupped, app)
					prev = app
				}
			}
		}
		cntAfter = uint32(len(deDupped))
	}

	return
}

/*
			report.SizeNow, report.SizeThen = mon.RemoveDups()
            static bool first = true;
            if (verbose || sizeThen != sizeNow) {
                if (!first)
                    cout << ",";
                first = false;
                cout << "{ ";
                cout << "\"path\": \"" << substitute(path, m.getPathToMonitor("", false), "$CACHE/") << "\", ";
                cout << "\"sizeThen\": " << sizeThen << ", ";
                cout << "\"sizeNow\": " << sizeNow;
                if (sizeThen > sizeNow)
                    cout << ", \"dupsRemoved\": " << (sizeThen - sizeNow);
                cout << " }" << endl;
            }
        }
    }

    return !shouldQuit();
}

//----------------------------------------------------------------
bool CMonitor::removeDuplicates(const string_q& path) {
    if (!isMonitorFilePath(path))
        return false;
    CStringArray parts;
    explode(parts, path, '/');
    address = substitute(parts[parts.size() - 1], "mon.bin", "");

    if (!readAppearances(nullptr, nullptr)) {
        LOG_WARN("Could load monitor for address ", address);
        return false;
    }
    sort(apps.begin(), apps.end());

    CAppearance_mon prev;
    bool hasDups = false;
    for (auto a : apps) {
        if (a.blk == prev.blk && a.txid == prev.txid) {
            hasDups = true;
            break;
        }
        prev = a;
    }
    if (!hasDups)
        return true;

    CAppearanceArray_mon deduped;
    for (auto a : apps) {
        if (a.blk != prev.blk || a.txid != prev.txid) {
            deduped.push_back(a);
        }
        prev = a;
    }

    CArchive archiveOut(WRITING_ARCHIVE);
    archiveOut.Lock(path, modeWriteCreate, LOCK_WAIT);
    for (auto item : deduped)
        archiveOut << item.blk << item.txid;
    archiveOut.Release();

    return true;
}
*/
