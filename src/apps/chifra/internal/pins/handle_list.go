// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package pinsPkg

import (
	"net/http"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
)

func (opts *PinsOptions) ListInternal() error {
	manifestData, err := manifest.FromLocalFile()
	if err != nil {
		return err
	}

	sort.Slice(manifestData.NewPins, func(i, j int) bool {
		iPin := manifestData.NewPins[i]
		jPin := manifestData.NewPins[j]
		return iPin.FileName < jPin.FileName
	})

	if opts.Globals.TestMode {
		// Shorten the array for testing
		manifestData.NewPins = manifestData.NewPins[:100]
	}

	opts.PrintManifestHeader()
	if opts.Globals.ApiMode {
		opts.Globals.Respond(opts.Globals.Writer, http.StatusOK, manifestData.NewPins)

	} else {
		err = opts.Globals.Output(os.Stdout, opts.Globals.Format, manifestData.NewPins)
		if err != nil {
			logger.Log(logger.Error, err)
		}
	}

	return nil
}
