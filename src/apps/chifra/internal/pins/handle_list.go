package pins

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

import (
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
)

func HandleList(opts *PinsOptionsType) {
	pins, err := ListInternal(opts)
	if err != nil {
		logger.Fatal("Cannot open local manifest file", err)
	}

	err = output.Output(os.Stdout, opts.Globals.Format, pins)
	if err != nil {
		logger.Log(logger.Error, err)
	}
}

func ListInternal(opts *PinsOptionsType) ([]manifest.PinDescriptor, error) {
	manifestData, err := manifest.FromLocalFile()
	if err != nil {
		return nil, err
	}

	// Sort pins
	sort.Slice(manifestData.NewPins, func(i, j int) bool {
		iPin := manifestData.NewPins[i]
		jPin := manifestData.NewPins[j]
		return iPin.FileName < jPin.FileName
	})

	// Shorten the array for testing
	if os.Getenv("TEST_MODE") == "true" {
		manifestData.NewPins = manifestData.NewPins[:100]
	}

	return manifestData.NewPins, nil
}
