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

// statusCmd represents the status command
var statusCmd = &cobra.Command{
	Use:   "status",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("status called")
	},
}

func init() {
	rootCmd.AddCommand(statusCmd)
	statusCmd.SetHelpTemplate(getHelpTextStatus())
}

func getHelpTextStatus() string {
	return `chifra argc: 5 [1:status] [2:--help] [3:--verbose] [4:2] 
chifra status --help --verbose 2 
chifra status argc: 4 [1:--help] [2:--verbose] [3:2] 
chifra status --help --verbose 2 
PROG_NAME = [chifra status]

  Usage:    chifra status [-d|-t|-v|-h] <mode> [mode...]  
  Purpose:  Report on the status of the TrueBlocks system.

  Where:
    modes                 the type of status info to retrieve, one or more of [index|monitors|collections|names|abis|caches|some*|all]
    -d  (--details)       include details about items found in monitors, slurps, abis, or price caches
    -t  (--types <val>)   for caches mode only, which type(s) of cache to report, one or more of [blocks|transactions|traces|slurps|prices|all*]

    #### Hidden options
    -p  (--depth <num>)   for cache mode only, number of levels deep to report
    -r  (--report)        show a summary of the current status of TrueBlocks (deprecated)
    -e  (--terse)         show a terse summary report
    -m  (--migrate <val>) either effectuate or test to see if a migration is necessary, one or more of [test|abi_cache|block_cache|tx_cache|trace_cache|recon_cache|name_cache|slurp_cache|all]
    -g  (--get_config)    returns JSON data of the editable configuration file items
    -s  (--set_config)    accepts JSON in an env variable and writes it to configuration files
    -S  (--test_start <num>)first block to process (inclusive -- testing only)
    -E  (--test_end <num>)last block to process (inclusive -- testing only)
    #### Hidden options

    -x  (--fmt <val>)     export format, one of [none|json*|txt|csv|api]
    -v  (--verbose)       set verbose level (optional level defaults to 1)
    -h  (--help)          display this help screen

  Powered by TrueBlocks
`
}
