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
package cmd

import (
	"log"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/serve"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

// TODO: this should be soon part of pkg/utils
var Off string = "\033[0m"
var Green string = "\033[32m"

func validateServeArgs(cmd *cobra.Command, args []string) error {
	if len(ServeOpts.port) > 0 && !strings.Contains(ServeOpts.port, ":") {
		return validate.Usage("The --port option must start with a ':'")
	}

	return nil
}

func PrintServeSettings() {
	if ServeOpts.port != ":8080" {
		log.Println(Green, "port:        ", Off, ServeOpts.port)
	}

	meta := output.GetMeta()
	log.Print("\n")
	log.Println(Green, "Starting API server on port "+ServeOpts.port, Off)
	log.Println(Green, "Cache Path:   ", Off, config.ReadTrueBlocks().Settings.CachePath)
	log.Println(Green, "Index Path:   ", Off, config.ReadTrueBlocks().Settings.CachePath)
	log.Println(Green, "Rpc Provider: ", Off, config.ReadTrueBlocks().Settings.RpcProvider)
	log.Println(Green, "Progress:     ", Off, meta.Client, ", ", meta.Finalized, ", ", meta.Staging, ", ", meta.Unripe)

}

// runServe runs serve command and sends any errors to log.Fatal
func runServe(cmd *cobra.Command, args []string) {
	serve.Options.Port = ServeOpts.port
	// serve.Options.Verbose = RootOpts.verbose
	PrintServeSettings()
	log.Fatal(serve.Run(ServeOpts.port))
}
