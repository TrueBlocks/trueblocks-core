// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) showIndex(path string, first bool) error {
	path = index.ToIndexPath(path)
	ff, err := os.Open(path)
	if err != nil {
		return err
	}
	defer ff.Close()
	header, err := index.ReadHeader(ff)
	var obj types.SimpleIndex
	obj.Range, _ = cache.RangeFromFilename(path)
	obj.Magic = header.Magic
	obj.Hash = header.Hash
	obj.AddressCount = header.AddressCount
	obj.AppearanceCount = header.AppearanceCount
	obj.Size = file.FileSize(path)
	if err != nil {
		return err
	}

	// TODO: Fix export without arrays
	err = opts.Globals.RenderObject(obj, first)
	if err != nil {
		return err
	}

	return nil
}

// TODO: Don't forget about this option (pushing to IPFS)
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
*/
