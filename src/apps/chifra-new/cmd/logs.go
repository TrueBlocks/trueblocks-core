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
	"os"

	"github.com/spf13/cobra"
)

// logsCmd represents the logs command
var logsCmd = &cobra.Command{
	Use: `logs [flags] <tx_id> [tx_id...]

Arguments:
  transactions - a space-separated list of one or more transaction identifiers (required)`,
	Short: "Retrieve logs for the given transaction(s)",
	Long: `Purpose:
  Retrieve logs for the given transaction(s).
`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("logs called")
	},
}

func init() {
	rootCmd.AddCommand(logsCmd)

	logsCmd.Flags().SortFlags = false
	logsCmd.PersistentFlags().SortFlags = false
	logsCmd.SetOut(os.Stderr)
	logsCmd.Flags().StringP("topic", "t", "", "filter by one or more log topics (not implemented)")
	logsCmd.Flags().StringP("source", "s", "", "export only if the given address emitted the event (not implemented)")
	logsCmd.Flags().BoolP("articulate", "a", false, "articulate the retrieved data if ABIs can be found")
	logsCmd.Flags().MarkHidden("transactions")
}
