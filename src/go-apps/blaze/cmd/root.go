package cmd

import (
	"fmt"
	"os"
	"strings"

	"github.com/spf13/cobra"

	homedir "github.com/mitchellh/go-homedir"
	"github.com/spf13/viper"
)

var cfgFile string

// Verbose print more detailed output
var Verbose bool

// RPCProvider location of node's RPC interface
var RPCProvider string

// CachePath location of TrueBlocks' cache
var CachePath string

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:   "blaze",
	Short: "Build, query, or share an index of Ethereum addresses per block",
	Long: `
Description:
  Blaze watches the front edge of the Ethereum blockchain and accumulates a
  list of every addresses as it appears anywhere on the chain. You may query
  this index to produce a list of <blockNumber,transactionId> pairs many orders
  of magnitude faster than querying the blockchain directly. Armed with this
  list of pairs, you may then call "acctExport" to detail the full transaction
  history for any account including external and internal transactions, logs,
  and traces. Blaze also allows you to seed (share the index) or leech (retrieve
  the index from others) via IPFS.`,
	Version: "GHC-TrueBlocks, LLC//0.0.1-alpha",
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
	cobra.OnInitialize(initConfig)
	rootCmd.PersistentFlags().StringVar(&cfgFile, "config", "", "config file (default is $HOME/.quickBlocks/quickBlocks.toml)")
	rootCmd.PersistentFlags().BoolVarP(&Verbose, "verbose", "v", false, "print more detailed output of command")
	rootCmd.PersistentFlags().String("rpcProvider", "http://localhost:8545", "The location of the node's RPC")
	rootCmd.PersistentFlags().String("cachePath", "", "The location of TrueBlocks' cache")
	rootCmd.PersistentFlags().IntP("startBlock", "s", -1, "First block to visit")
	rootCmd.PersistentFlags().IntP("nBlocks", "n", 5000, "The number of blocks to scrape during this invocation")
	rootCmd.PersistentFlags().IntP("nBlockChans", "", 50, "The number of block processor go routines to create")
	rootCmd.PersistentFlags().IntP("nAddrChans", "", 100, "The number of address processor go routines to create")
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

		// Search config in home directory with name ".blaze" (without extension).
		viper.AddConfigPath(home + "/.quickBlocks")
		viper.SetConfigType("toml")
		viper.SetConfigName("quickBlocks")
	}

	// If a config file is found, read it in.
	if err := viper.ReadInConfig(); err == nil {
		//		fmt.Println("Using config file:", viper.ConfigFileUsed())
	} else {
		fmt.Println(err)
	}

	viper.SetEnvPrefix("TB")
	viper.BindEnv("cachePath")
	viper.BindEnv("rpcProvider")
	viper.AutomaticEnv() // read in environment variables that match
	viper.BindPFlag("settings.rpcProvider", rootCmd.PersistentFlags().Lookup("rpcProvider"))
	viper.BindPFlag("settings.cachePath", rootCmd.PersistentFlags().Lookup("cachePath"))
	viper.BindPFlag("startBlock", rootCmd.PersistentFlags().Lookup("startBlock"))
	viper.BindPFlag("nBlocks", rootCmd.PersistentFlags().Lookup("nBlocks"))
	viper.BindPFlag("nBlockChans", rootCmd.PersistentFlags().Lookup("nBlockChans"))
	viper.BindPFlag("nAddrChans", rootCmd.PersistentFlags().Lookup("nAddrChans"))
	viper.SetEnvKeyReplacer(strings.NewReplacer("SETTINGS.", ""))
}
