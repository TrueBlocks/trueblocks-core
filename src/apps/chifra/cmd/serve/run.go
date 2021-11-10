package serve

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
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/root"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/spf13/cobra"
)

func PrintServeSettings() {
	if Options.Port != ":8080" {
		log.Println(Green, "port:        ", Off, Options.Port)
	}

	meta := output.GetMeta()
	log.Print("\n")
	log.Println(Green, "Starting API server on port "+Options.Port, Off)
	log.Println(Green, "Cache Path:   ", Off, config.ReadTrueBlocks().Settings.CachePath)
	log.Println(Green, "Index Path:   ", Off, config.ReadTrueBlocks().Settings.CachePath)
	log.Println(Green, "Rpc Provider: ", Off, config.ReadTrueBlocks().Settings.RpcProvider)
	log.Println(Green, "Progress:     ", Off, meta.Client, ", ", meta.Finalized, ", ", meta.Staging, ", ", meta.Unripe)

}

// Run runs serve command and sends any errors to log.Fatal
func Run(cmd *cobra.Command, args []string) {
	Options.Port = Options.Port
	if root.Options.Verbose {
		Options.Verbose = 1
	}
	PrintServeSettings()
	log.Fatal(RunInternal(Options.Port))
}
