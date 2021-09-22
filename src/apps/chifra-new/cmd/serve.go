package cmd

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
	"fmt"

	"github.com/spf13/cobra"
)

// serveCmd represents the serve command
var serveCmd = &cobra.Command{
	Use:   "serve",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("serve called")
	},
}

func init() {
	rootCmd.AddCommand(serveCmd)
	serveCmd.SetHelpTemplate(getHelpTextServe())
}

func getHelpTextServe() string {
	return `chifra argc: 5 [1:serve] [2:--help] [3:--verbose] [4:2] 
chifra serve --help --verbose 2 
Usage of flame:
      --monitor       enable monitor scraper mode
      --pin           pins Bloom filters and chunks to pinning service (requires API key)
      --port string   specify the server's port (default ":8080")
      --scrape        enable block scraper mode
      --sleep int     specifies sleep interval between scrapes (default 14)
      --verbose int   verbose level (between 0 and 10 inclusive)
pflag: help requested
`
}
