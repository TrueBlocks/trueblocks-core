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

// exportCmd represents the export command
var exportCmd = &cobra.Command{
	Use:   "export",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("export called")
	},
}

func init() {
	rootCmd.AddCommand(exportCmd)
	exportCmd.SetHelpTemplate(getHelpTextExport())
}

func getHelpTextExport() string {
	return `chifra argc: 5 [1:export] [2:--help] [3:--verbose] [4:2] 
chifra export --help --verbose 2 
chifra export argc: 4 [1:--help] [2:--verbose] [3:2] 
chifra export --help --verbose 2 
PROG_NAME = [chifra export]

  Usage:    chifra export [-p|-r|-A|-l|-t|-C|-a|-i|-R|-y|-U|-c|-e|-v|-h] <address> [address...] [topics] [fourbytes]  
  Purpose:  Export full detail of transactions for one or more addresses.

  Where:
    addrs                  one or more addresses (0x...) to export (required)
    topics                 filter by one or more log topics (only for --logs option)
    fourbytes              filter by one or more fourbytes (only for transactions and trace options)
    -p  (--appearances)    export a list of appearances
    -r  (--receipts)       export receipts instead of transaction list
    -A  (--statements)     for use with --accounting option only, export only reconciliation statements
    -l  (--logs)           export logs instead of transaction list
    -t  (--traces)         export traces instead of transaction list
    -C  (--accounting)     export accounting records instead of transaction list
    -a  (--articulate)     articulate transactions, traces, logs, and outputs
    -i  (--cache_txs)      write transactions to the cache (see notes)
    -R  (--cache_traces)   write traces to the cache (see notes)
    -y  (--factory)        scan for contract creations from the given address(es) and report address of those contracts
        (--emitter)        for log export only, export only if one of the given export addresses emitted the event
        (--source <addr>)  for log export only, export only one of these addresses emitted the event
        (--relevant)       for log and accounting export only, if true export only logs relevant to one of the given export addresses
    -U  (--count)          only available for --appearances mode, if present return only the number of records
    -c  (--first_record <num>)the first record to process
    -e  (--max_records <num>)the maximum number of records to process before reporting
        (--clean)          clean (i.e. remove duplicate appearances) from all existing monitors

    #### Hidden options
    -f  (--freshen)        freshen but do not print the exported data
    -F  (--first_block <num>)first block to process (inclusive)
    -L  (--last_block <num>)last block to process (inclusive)
    -s  (--staging)        enable search of staging (not yet finalized) folder
    -u  (--unripe)         enable search of unripe (neither staged nor finalized) folder (assumes --staging)
        (--load <str>)     a comma separated list of dynamic traversers to load
        (--reversed)       produce results in reverse chronological order
    -b  (--by_date)        produce results sorted by date (default is to report by address)
    -z  (--summarize_by <val>)for --accounting only, summarize reconciliations by this time period, one of [yearly|quarterly|monthly|weekly|daily|hourly|blockly|tx]
    #### Hidden options

    -x  (--fmt <val>)      export format, one of [none|json*|txt|csv|api]
    -v  (--verbose)        set verbose level (optional level defaults to 1)
    -h  (--help)           display this help screen

  Notes:
    - An address must start with '0x' and be forty-two characters long.

  Configurable Items:
    - cache_txs: write transactions to the cache (see notes).
    - cache_traces: write traces to the cache (see notes).
    - skip_ddos: toggle skipping over 2016 dDos transactions ('on' by default).
    - max_traces: if --skip_ddos is on, this many traces defines what a ddos transaction
      is (default = 250).

  Powered by TrueBlocks
`
}
