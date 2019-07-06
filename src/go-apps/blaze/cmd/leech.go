package cmd

import (
	"fmt"

	"github.com/spf13/cobra"
)

// leechCmd represents the leech command
var leechCmd = &cobra.Command{
	Use:   "leech",
	Short: "Retreive part or all of the index from peers via IPFS.",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("leech called")
	},
}

func init() {
	rootCmd.AddCommand(leechCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// leechCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// leechCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
