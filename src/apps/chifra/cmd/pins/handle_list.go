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
package pins

import (
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleList loads manifest, sorts pins and prints them out
func HandleList() {
	// Load manifest
	manifestData, err := manifest.FromLocalFile()
	if err != nil {
		logger.Fatal("Cannot open local manifest file", err)
	}

	// Sort pins
	sort.Slice(manifestData.NewPins, func(i, j int) bool {
		iPin := manifestData.NewPins[i]
		jPin := manifestData.NewPins[j]
		return iPin.FileName < jPin.FileName
	})

	// Shorten the array for testing
	if utils.IsTestMode() {
		manifestData.NewPins = manifestData.NewPins[:100]
	}

	// TODO: if Root.to_file == true, write the output to a filename
	// TODO: if Root.output == <fn>, write the output to a <fn>

	if output.Format == "" || output.Format == "none" {
		if utils.IsApiMode() {
			output.Format = "api"
		} else {
			output.Format = "txt"
		}
	}

	outFmt := "%s\t%s\t%s\n"
	switch output.Format {
	case "txt":
		// do nothing
	case "csv":
		outFmt = "\"%s\",\"%s\",\"%s\"\n"
	case "json":
		fallthrough
	case "api":
		err := output.PrintJson(manifestData.NewPins)
		if err != nil {
			logger.Fatal(err)
		}
		return
	}

	if output.Format == "txt" && utils.IsTerminal() {
		table := &output.Table{}
		table.New()
		table.Header([]string{"filename", "bloomhash", "indexhash"})
		for _, pin := range manifestData.NewPins {
			table.Row([]string{pin.FileName, pin.BloomHash, pin.IndexHash})
		}
		table.Print()
	} else {
		fmt.Printf(outFmt, "filename", "bloomhash", "indexhash")
		for _, pin := range manifestData.NewPins {
			fmt.Printf(outFmt, pin.FileName, pin.BloomHash, pin.IndexHash)
		}
	}
}
