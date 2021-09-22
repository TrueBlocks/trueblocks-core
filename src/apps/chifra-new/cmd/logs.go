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

// logsCmd represents the logs command
var logsCmd = &cobra.Command{
	Use:   "logs",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("logs called")
	},
}

func init() {
	rootCmd.AddCommand(logsCmd)
	logsCmd.SetHelpTemplate(getHelpTextLogs())
}

func getHelpTextLogs() string {
	return `chifra argc: 5 [1:logs] [2:--help] [3:--verbose] [4:2] 
chifra logs --help --verbose 2 
chifra logs argc: 4 [1:--help] [2:--verbose] [3:2] 
chifra logs --help --verbose 2 
PROG_NAME = [chifra logs]

  Usage:    chifra logs [-t|-s|-a|-v|-h] <tx_id> [tx_id...]  
  Purpose:  Retrieve logs for the given transaction(s).

  Where:
    transactions          a space-separated list of one or more transaction identifiers (required)
    -t  (--topic <hash>)  filter by one or more log topics (not implemented)
    -s  (--source <addr>) export only if the given address emitted the event (not implemented)
    -a  (--articulate)    articulate the retrieved data if ABIs can be found
    -x  (--fmt <val>)     export format, one of [none|json*|txt|csv|api]
    -v  (--verbose)       set verbose level (optional level defaults to 1)
    -h  (--help)          display this help screen

  Notes:
    - The transactions list may be one or more space-separated identifiers which are either a transaction hash,
      a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
    - This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
    - If the queried node does not store historical state, the results for most older transactions are undefined.
    - If you specify a 32-byte hash, it will be assumed to be a transaction hash, if the transaction is
      not found, it will be used as a topic.

  Powered by TrueBlocks
`
}
