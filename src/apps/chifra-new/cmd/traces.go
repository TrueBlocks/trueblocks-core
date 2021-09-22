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

// tracesCmd represents the traces command
var tracesCmd = &cobra.Command{
	Use:   "traces",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("traces called")
	},
}

func init() {
	rootCmd.AddCommand(tracesCmd)
	tracesCmd.SetHelpTemplate(getHelpTextTraces())
}

func getHelpTextTraces() string {
	return `chifra argc: 5 [1:traces] [2:--help] [3:--verbose] [4:2] 
chifra traces --help --verbose 2 
chifra traces argc: 4 [1:--help] [2:--verbose] [3:2] 
chifra traces --help --verbose 2 
PROG_NAME = [chifra traces]

  Usage:    chifra traces [-a|-f|-d|-c|-v|-h] <tx_id> [tx_id...]  
  Purpose:  Retrieve traces for the given transaction(s).

  Where:
    transactions          a space-separated list of one or more transaction identifiers (required)
    -a  (--articulate)    articulate the retrieved data if ABIs can be found
    -f  (--filter <str>)  call the node's trace_filter routine with bang-seperated filter
    -d  (--statediff)     export state diff traces (not implemented)
    -c  (--count)         show the number of traces for the transaction only (fast)

    #### Hidden options
    -s  (--skip_ddos)     skip over the 2016 ddos during export ('on' by default)
    -m  (--max <num>)     if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250)
    #### Hidden options

    -x  (--fmt <val>)     export format, one of [none|json*|txt|csv|api]
    -v  (--verbose)       set verbose level (optional level defaults to 1)
    -h  (--help)          display this help screen

  Notes:
    - The transactions list may be one or more space-separated identifiers which are either a transaction hash,
      a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
    - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
    - If the queried node does not store historical state, the results for most older transactions are undefined.
    - A bang seperated filter has the following fields (at least one of which is required) and is separated
      with a bang (!): fromBlk, toBlk, fromAddr, toAddr, after, count.
    - A state diff trace describes, for each modified address, what changed during that trace.

  Configurable Items:
    - skip_ddos: skip over the 2016 ddos during export ('on' by default).
    - max: if --skip_ddos is on, this many traces defines what a ddos transaction is (default = 250).

  Powered by TrueBlocks
`
}
