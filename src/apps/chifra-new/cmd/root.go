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

	homedir "github.com/mitchellh/go-homedir"
	"github.com/spf13/viper"
)

var cfgFile string

// rootOptionsType Structure to carry command line and config file options
type rootOptTypes struct {
	fmt     string
	verbose uint
	help    bool
}

var RootOpts rootOptTypes

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:   "chifra",
	Short: "A brief description of your application",
	Long: `A longer description that spans multiple lines and likely contains
examples and usage of using your application. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	// Uncomment the following line if your bare application
	// has an action associated with it:
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println(getHelpTextRoot())
	},
	Version: "Powered by TrueBlocks (GHC-TrueBlocks//0.12.1-alpha-7c5fb3f2a-20210923)",
}

// Execute adds all child commands to the root command and sets flags appropriately.
// This is called by main.main(). It only needs to happen once to the rootCmd.
func Execute() {
	if err := rootCmd.Execute(); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}

func init() {
	rootCmd.Flags().SortFlags = false
	rootCmd.PersistentFlags().SortFlags = false
	rootCmd.SetOut(os.Stderr)
	rootCmd.PersistentFlags().StringVarP(&RootOpts.fmt, "fmt", "x", "", "export format, one of [none|json*|txt|csv|api]")
	rootCmd.PersistentFlags().UintVarP(&RootOpts.verbose, "verbose", "v", 0, "set verbose level (optional level defaults to 1)")
	rootCmd.PersistentFlags().BoolVarP(&RootOpts.help, "help", "h", false, "display this help screen")

	cobra.OnInitialize(initConfig)
}

// initConfig reads in config file and ENV variables if set.
func initConfig() {
	if cfgFile != "" {
		// Use config file from the flag.
		viper.SetConfigFile(cfgFile)
	} else {
		// Find home directory.
		home, err := homedir.Dir()
		if err != nil {
			fmt.Println(err)
			os.Exit(1)
		}

		// Search config in home directory with name ".chifra" (without extension).
		viper.AddConfigPath(home)
		viper.SetConfigName(".chifra")
	}

	viper.AutomaticEnv() // read in environment variables that match

	// If a config file is found, read it in.
	if err := viper.ReadInConfig(); err == nil {
		fmt.Println("Using config file:", viper.ConfigFileUsed())
	}
}

func getHelpTextRoot() string {
	return `

  Usage:    chifra command  
  Purpose:  Access to all TrueBlocks tools (chifra <cmd> --help for more).

  Where:  
     ACCOUNTS
       list          list every appearance of an address anywhere on the chain
       export        export full detail of transactions for one or more addresses
       monitors      add, remove, clean, and list address monitors
       names         query addresses or names of well known accounts
       abis          fetches the ABI for a smart contract
     CHAIN DATA
       blocks        retrieve one or more blocks from the chain or local cache
       transactions  retrieve one or more transactions from the chain or local cache
       receipts      retrieve receipts for the given transaction(s)
       logs          retrieve logs for the given transaction(s)
       traces        retrieve traces for the given transaction(s)
       when          find block(s) based on date, blockNum, timestamp, or 'special'
     CHAIN STATE
       state         retrieve account balance(s) for one or more addresses at given block(s)
       tokens        retrieve token balance(s) for one or more addresses at given block(s)
     ADMIN
       status        report on the status of the TrueBlocks system
       serve         serve the TrueBlocks API using the flame server
       scrape        scan the chain and update the TrueBlocks index of appearances
       init          initialize the index of appearances by downloading Bloom filters
       pins          manage pinned index of appearances and associated Bloom filters
     OTHER
       quotes        freshen and/or display Ethereum price data
       explore       open an explorer for a given address, block, or transaction
       slurp         fetch data from EtherScan for any address
     

  Powered by TrueBlocks
`
}

// enums
// https://pkg.go.dev/github.com/thediveo/enumflag
