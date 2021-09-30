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
	"errors"
	"log"

	"github.com/spf13/cobra"
	"github.com/spf13/cobra/doc"
)

// autodocCmd represents the autodoc command
var autodocCmd = &cobra.Command{
	Use:   "autodoc",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		err := doc.GenMarkdownTree(rootCmd, "./docs")
		if err != nil {
			log.Fatal(err)
		}
		header := &doc.GenManHeader{
			Title:   "MINE",
			Section: "3",
		}
		err = doc.GenManTree(rootCmd, header, "./man")
		if err != nil {
			log.Fatal(err)
		}
	},
	Args: func(cmd *cobra.Command, args []string) error {
		if len(args) < 1 {
			return errors.New(Red + "\n A destination folder is required\n" + Off)
		}
		return nil
	},
}

var Off string = "\033[0m"
var Red string = "\033[31m"
var Green string = "\033[32m"
var Yellow string = "\033[33m"
var Blue string = "\033[34m"
var Purple string = "\033[35m"
var Cyan string = "\033[36m"
var White string = "\033[37m"
var Black string = "\033[30m"

func init() {
	rootCmd.AddCommand(autodocCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// autodocCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// autodocCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
