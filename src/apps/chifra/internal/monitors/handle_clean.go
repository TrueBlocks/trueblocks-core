// TODO: BOGUS - Do we need to use monitors/staging or can we build it in memory?
// TODO: BOGUS - Does chifra clean work?

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
)

type CleanReport struct {
	Path     string `json:"path"`
	SizeThen int    `json:"sizeThen"`
	SizeNow  int    `json:"sizeNow"`
	Dups     int    `json:"dupsRemoved"`
}

func (opts *MonitorsOptions) HandleClean() error {
	monitorChan := make(chan monitor.Monitor)

	var monitors []monitor.Monitor
	go monitor.ListMonitors(opts.Globals.Chain, "monitors", monitorChan)

	for result := range monitorChan {
		switch result.Address {
		case monitor.SentinalAddr:
			close(monitorChan)
		default:
			monitors = append(monitors, result)
		}
	}

	for _, mon := range monitors {
		if opts.Globals.TestMode {
			if strings.ToLower(mon.Address.Hex()) == "0x001d14804b399c6ef80e64576f657660804fec0b" ||
				strings.ToLower(mon.Address.Hex()) == "0x0029218e1dab069656bfb8a75947825e7989b987" {
				fmt.Println(mon)
			}
		} else {
			// sizeThen := mon.Count()
			// if sizeThen > 0 {
			// 	// mon.RemoveDuplicates()
			// }
			// sizeNow := mon.Count()
			mon.ReadMonHeader()
			//apps := make([]index.AppearanceRecord, mon.Count, mon.Count)
			//err := mon.ReadApps(&apps)
			//if err != nil {
			//	return err
			//}
			fmt.Println(mon)
		}
	}
	return nil
}

/*
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
