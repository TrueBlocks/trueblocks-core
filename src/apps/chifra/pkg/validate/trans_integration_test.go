//go:build integration
// +build integration

package validate

import (
	"fmt"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
)

func Test_TransactionIds(t *testing.T) {
	for i, item := range testTxs {
		if !item.enabled {
			continue
		}
		// if strings.Contains(item.input, ":next") || strings.Contains(item.input, ":prev") {
		// 	continue
		// }
		fmt.Println("-----", i, "----->", item.input)
		inputs := strings.Split(item.input, " ")
		expecteds := strings.Split(item.expected, " ")
		for i, e := range expecteds {
			fmt.Println(i, e)
		}
		var results []blockRange.Identifier
		var err error
		err = ValidateIdentifiers(
			"mainnet",
			inputs,
			ValidTransId,
			100,
			&results,
		)
		if err != nil {
			// we're not testing validation here, so there won't be any errors here, but if
			// there is we want to know about them so we can fix the test
			fmt.Println(colors.Red, item.input, err, colors.Off)
			continue
		}

		for i, br := range results {
			fmt.Println(br)
			txList, err := br.ResolveTxs(GetTestChain())
			if err != nil {
				t.Error(colors.Red, br)
				t.Error(err, colors.Off)
			}
			for j, tx := range txList {
				res := fmt.Sprintf("%d.%d", tx.BlockNumber, tx.TransactionIndex)
				index := i
				if len(expecteds) > 1 && j > 0 {
					index = j
				}
				fmt.Println(res, expecteds[index], res == expecteds[index])
				if res != expecteds[index] {
					fmt.Printf("%sgot: %d.%d expected: %s%s", colors.Red, tx.BlockNumber, tx.TransactionIndex, expecteds[index], colors.Off)
				} else {
					fmt.Printf("%s%d.%d%s", colors.Green, tx.BlockNumber, tx.TransactionIndex, colors.Off)
					if len(results) > 0 {
						fmt.Println()
					}
				}
			}
			if len(txList) == 0 {
				fmt.Printf("%sgot: nothing expected: %s%s", colors.Red, expecteds[i], colors.Off)
			}
			fmt.Println()
		}
	}
}

var testTxs = []TestCase{
	{
		input:    "1001001.0",
		expected: "1001001.0",
		enabled:  true,
	},
	{
		input:    "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.2",
		expected: "1001001.2",
		enabled:  true,
	},
	{
		input:    "0xc20a01b9d0bc87268376d189044e2c76cb2b34dda31e5525cbef45b3c30849e6",
		expected: "1001000.2",
		enabled:  true,
	},
	{
		input:    "0x730724cb08a6eb17bf6b3296359d261570d343ea7944a17a9d7287d77900db08 0xef2ea39c20ba09553b2f3cf02380406ac766039ca56612937eed5e7f3503fb3a 0xc20a01b9d0bc87268376d189044e2c76cb2b34dda31e5525cbef45b3c30849e6 0x5352c80aa2073e21ce6c4aa5488c38455f3519955ece7dca5af3e326797bcc63",
		expected: "1001001.0 1001001.1 1001000.2 1001001.2",
		enabled:  true,
	},
	{
		input:    "1001001.*",
		expected: "1001001.0 1001001.1 1001001.2 1001001.3",
		enabled:  true,
	},
	{
		input:    "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.*",
		expected: "1001001.0 1001001.1 1001001.2 1001001.3",
		enabled:  true,
	},
	// TODO: next and previous skip markers
	// {
	// 	input:    "1001001.0:next",
	// 	expected: "1001001.1",
	// 	enabled:  false,
	// },
	// {
	// 	input:    "1001001.0:prev",
	// 	expected: "1001000.2",
	// 	enabled:  true,
	// },
	// {
	// 	input:    "0xc20a01b9d0bc87268376d189044e2c76cb2b34dda31e5525cbef45b3c30849e6:next",
	// 	expected: "1001001.0",
	// 	enabled:  true,
	// },
	// {
	// 	input:    "1001001.0 1001001.0:next 1001001.0:prev 1001001.2",
	// 	expected: "1001001.0 1001001.1 1001000.2 1001001.2",
	// 	enabled:  true,
	// },
}
