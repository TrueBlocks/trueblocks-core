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
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/utils"
	"github.com/spf13/cobra"
)

func validateScrapeArgs(cmd *cobra.Command, args []string) error {
	if len(args) == 0 {
		return nil // defaults to indexer
	} else {
		for _, arg := range args {
			err := validate.ValidateEnum("mode", arg, "[indexer|monitors|both]")
			if err != nil {
				return err
			}
		}
	}
	err := validate.ValidateEnum("action", ScrapeOpts.action, "[toggle|run|restart|pause|quit]")
	if err != nil {
		return err
	}

	fmt.Println("action: ", ScrapeOpts.action)
	if len(ScrapeOpts.action) > 0 {
		return validate.Usage("the --action option is currently not implemented")
	}

	if !utils.IsTestMode() && ScrapeOpts.publish {
		return validate.Usage("the --publish option is current not implemented")
	}
	if !utils.IsTestMode() && ScrapeOpts.pin {
		return validate.Usage("the --pin option is current not implemented")
	}
	if !utils.IsTestMode() && len(ScrapeOpts.port) > 0 {
		return validate.Usage("the --publish option is current not implemented")
	}

	// need api keys and/or IPFS running
	// bool &ScrapeOpts.pin, "pin", "p", false, "pin chunks (and blooms) to IPFS as they are created (requires pinning service)")
	// bool &ScrapeOpts.publish, "publish", "u", false, "after pinning the chunk, publish it to UnchainedIndex (hidden)")

	// must start with :
	// string &ScrapeOpts.port, "port", "o", ":8081", "specify the server's port (:8081 default) (hidden)")

	// uint64 &ScrapeOpts.n_blocks, "n_blocks", "n", 2000, "maximum number of blocks to process per pass")
	// uint64 &ScrapeOpts.n_block_procs, "n_block_procs", "b", 10, "number of concurrent block processing channels (hidden)")
	// uint64 &ScrapeOpts.n_addr_procs, "n_addr_procs", "a", 20, "number of concurrent address processing channels (hidden)")

	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}
	return nil
}

func runScrape(cmd *cobra.Command, args []string) {
	options := ""
	if len(args) == 0 {
		options += " --scrape"

	} else {
		// assert(len(args) == 1)
		if args[0] == "index" || args[0] == "both" {
			options += " --scrape"
		}

		if args[0] == "monitors" || args[0] == "both" {
			options += " --monitor"
		}
	}

	if ScrapeOpts.sleep != 14. && ScrapeOpts.sleep > 0. {
		options += " --sleep " + fmt.Sprintf("%.1f", ScrapeOpts.sleep)
	}
	if ScrapeOpts.pin {
		options += " --pin"
	}
	if ScrapeOpts.publish {
		options += " --publish"
	}
	if len(ScrapeOpts.port) > 0 {
		options += " --port " + ScrapeOpts.port
	}
	if ScrapeOpts.n_blocks != 2000 {
		options += " --n_blocks " + fmt.Sprintf("%d", ScrapeOpts.n_blocks)
	}
	if ScrapeOpts.n_block_procs != 10 {
		options += " --n_block_procs " + fmt.Sprintf("%d", ScrapeOpts.n_block_procs)
	}
	if ScrapeOpts.n_addr_procs != 20 {
		options += " --n_addr_procs " + fmt.Sprintf("%d", ScrapeOpts.n_addr_procs)
	}
	// arguments := ""
	// for _, arg := range args {
	// 	arguments += " " + arg
	// }
	if !utils.IsTestMode() {
		PassItOn("flame-scrape", options, "")
	} else {
		fmt.Println("flame-scrape", options)
		fmt.Printf("%s\n", "{")
		fmt.Printf("  %s\n", "\"message\": \"Testing only\",")
		fmt.Printf("  %s\n", "\"n_blocks\": 2000,")
		fmt.Printf("  %s\n", "\"n_block_procs\": 10,")
		fmt.Printf("  %s\n", "\"n_addr_procs\": 20,")
		fmt.Printf("  %s\n", "\"pin\": 1,")
		fmt.Printf("%s\n", "}")
	}
}
