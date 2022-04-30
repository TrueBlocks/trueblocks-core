// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

/*
if (share) {
	string_q res := doCommand("which ipfs");
	if (res.empty()) {
		return usa ge("Could not find ipfs in your $PATH. You must install ipfs for the --share command to work.");
	}
}

if (share) {
	ostringstream os;
	os << "ipfs add -Q --pin \"" << bloomFn + "\"";
	string_q newHash = doCommand(os.str());
	LOG_INFO(cGreen, "Re-pinning ", pin.fileName, cOff, " ==> ", newHash, " ",
	(pin.bloomHash == newHash ? greenCheck : redX));
}

static bool chunkV isitFunc(const string_q& path, void* data) {
    // LOG_WARN(path);
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", chunkV isitFunc, data);

    } else {
        if (!endsWith(path, ".bin"))
            return true;

        blknum_t endBlock = NOPOS;
        blknum_t startBlock = path_2_Bn(path, endBlock);

        COptions* opts = (COptions*)data;
        blknum_t startTest = opts->blocks.start == NOPOS ? 0 : opts->blocks.start;
        blknum_t endTest = opts->blocks.stop == 0 ? NOPOS : opts->blocks.stop;
        if (!inRange(startBlock, startTest, endTest)) {
            LOG_PROG("Skipped: " + path + "\r");
            return true;
        }
        if (!inRange(endBlock, startTest, endTest)) {
            LOG_PROG("Skipped: " + path + "\r");
            return true;
        }

        CIndexArchive index(READING_ARCHIVE);
        if (index.ReadIndexFromBinary(path, IP_ALL)) {
            string_q msg = "start: {0} end: {1} fileSize: {2} bloomSize: {3} nAddrs: {4} nRows: {5}";
            replace(msg, "{0}", "{" + padNum9T(startBlock) + "}");
            replace(msg, "{1}", "{" + padNum9T(endBlock) + "}");
            replace(msg, "{2}", "{" + padNum9T(fileSize(path)) + "}");
            replace(
                msg, "{3}",
                "{" + padNum9T(fileSize(substitute(substitute(path, "finalized", "blooms"), ".bin", ".bloom"))) + "}");
            replace(msg, "{4}", "{" + padNum9T(uint64_t(index.header->nAddrs)) + "}");
            replace(msg, "{5}", "{" + padNum9T(uint64_t(index.header->nRows)) + "}");
            if (verbose) {
                string_q m = msg;
                replaceAny(m, "{}", "");
                replaceAll(m, "  ", " ");
                cout << "# " << m << endl;
                cout << "address\tstart\tcount" << endl;
            }
            replaceAll(msg, "{", cGreen);
            replaceAll(msg, "}", cOff);
            cout << msg << endl;

            if (verbose > 0) {
                for (uint32_t a = 0; a < index.nAddrs; a++) {
                    CIndexedAddress* aRec = &index.addresses[a];
                    cout << bytes_2_Addr(aRec->bytes) << "\t" << aRec->offset << "\t" << aRec->cnt << endl;
                    if (verbose > 4) {
                        for (uint32_t b = aRec->offset; b < (aRec->offset + aRec->cnt); b++) {
                            CIndexedAppearance* bRec = &index.appearances[b];
                            cout << "\t" << bRec->blk << "\t" << bRec->txid << endl;
                        }
                    }
                }
            }
        }
    }

    return true;
}
*/
