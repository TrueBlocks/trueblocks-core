package main

import (
	"fmt"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v5"
)

func downloadAbis() error {
	logger.SetTestMode(false)
	defer logger.SetTestMode(true)

	logger.Info(colors.Yellow + "Downloading Abis..." + colors.Off)

	addrs := []string{
		"0x98ee18d7a1f7510b78b36f5a16471c7cd0c1c531",
		"0xd9db270c1b5e3bd161e8c8503c55ceabee709552",
		"0x7d655c57f71464b6f83811c55d84009cd9f5221c",
		"0x45f783cce6b7ff23b2ab2d70e416cdb7d6055f51",
		"0x9ba00d6856a4edf4665bca2c2309936572473b7e",
		"0xdbd27635a534a3d3169ef0498beb56fb9c937489",
		"0xbb9bc244d798123fde783fcc1c72d3bb8c189413",
		"0x8d12a197cb00d4747a1fe03395095ce2a5cc6819",
		"0xbb2b8038a1640196fbe3e38816f3e67cba72d940",
		"0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f",
		"0x2260fac5e5542a773aa44fbcfedf7c193bc2c599",
		"0xffa93aacf49297d51e211817452839052fdfb961",
		"0xef638b4305b8a1620f4e0e562e127f1181ae16d2",
	}

	validCount := 0
	invalidCount := 0
	invalidAddrs := []string{}
	abiCounts := make(map[string]int)

	logger.InfoY("Checking all ABIs...")
	for _, addr := range addrs {
		opts := sdk.AbisOptions{
			Addrs: []string{addr},
		}
		if abis, _, err := opts.Abis(); err != nil {
			logger.Fatal(err)
		} else {
			funcCount := len(abis)
			abiCounts[addr] = funcCount
			if funcCount > 1 {
				validCount++
			} else {
				invalidCount++
				invalidAddrs = append(invalidAddrs, addr)
			}
		}
	}

	logger.InfoY("First pass summary:")
	logger.InfoG(fmt.Sprintf("    valid abis:   %d", validCount))
	logger.InfoG(fmt.Sprintf("    invalid ABIs: %d", invalidCount))

	if len(invalidAddrs) > 0 {
		for _, addr := range invalidAddrs {
			if funcCount, err := redownloadAbi(addr); err != nil {
				logger.Fatal(err)
			} else {
				abiCounts[addr] = funcCount
			}
		}
	}

	logger.InfoY("Report on Abis:")
	for addr, count := range abiCounts {
		if count > 1 {
			logger.InfoG(fmt.Sprintf("    ABI for %s has %d functions", addr, count))
		} else {
			logger.InfoR(fmt.Sprintf("    ABI for %s is invalid", addr))
		}
	}

	return nil
}

func redownloadAbi(addr string) (int, error) {
	opts := sdk.AbisOptions{
		Addrs: []string{addr},
	}

	logger.InfoBY(fmt.Sprintf("    Decaching invalid ABI %s", addr))
	opts.Globals.Decache = true
	if _, _, err := opts.Abis(); err != nil {
		return 0, err
	}

	logger.InfoBY(fmt.Sprintf("    Redownloading %s...", addr))
	opts.Globals.Decache = false
	abis, _, err := opts.Abis()
	if err != nil {
		return 0, err
	}

	funcCount := len(abis)

	// If still invalid, try again after sleeping
	if funcCount < 2 {
		logger.InfoC("    Possibly rate limited. Sleeping...")
		time.Sleep(3 * time.Second)

		logger.InfoC("    Trying again...")
		opts.Globals.Decache = true
		if _, _, err := opts.Abis(); err != nil {
			return 0, err
		}

		opts.Globals.Decache = false
		abis, _, err = opts.Abis()
		if err != nil {
			return 0, err
		}

		funcCount = len(abis)

		// If still invalid after second attempt, fail
		if funcCount < 2 {
			return funcCount, fmt.Errorf("failed to download valid ABI for %s after multiple attempts", addr)
		}
	}
	// logger.InfoG(fmt.Sprintf("Successfully downloaded ABI for %s with %d functions", addr, funcCount))
	time.Sleep(500 * time.Millisecond)
	return funcCount, nil
}

func clearCache() error {
	logger.SetTestMode(false)
	defer logger.SetTestMode(true)

	// Addresses to decache from export
	exportAddrs := []string{
		"0x08166f02313feae18bb044e7877c808b55b5bf58",
		"0x0ba45a8b5d5575935b8158a88c631e9f9c95a2e5",
		"0x65b0d5e1dc0dee0704f53f660aa865c72e986fc7",
		"0x868b8fd259abfcfdf9634c343593b34ef359641d",
		"0xc713e5e149d5d0715dcd1c156a020976e7e56b88",
		"0xd7e30ae310c1d1800f5b641baa7af95b2e1fd98c",
		"0xec3ef464bf821c3b10a18adf9ac7177a628e87cc",
	}

	// Transaction logs to decache
	logItems := []string{
		"0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.1",
		"0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.5",
	}

	// Part (a): Decache export data for addresses
	logger.InfoY("Decaching export data...")
	exportOpts := sdk.ExportOptions{
		Addrs: exportAddrs,
	}
	exportOpts.Globals.Decache = true
	if _, _, err := exportOpts.Export(); err != nil {
		return fmt.Errorf("error decaching export data: %w", err)
	}
	logger.InfoG("Successfully decached export data")

	// Part (b): Decache logs data
	logger.InfoY("Decaching logs data...")
	logsOpts := sdk.LogsOptions{
		TransactionIds: logItems,
	}
	logsOpts.Globals.Decache = true
	if _, _, err := logsOpts.Logs(); err != nil {
		return fmt.Errorf("error decaching logs data: %w", err)
	}
	logger.InfoG("Successfully decached logs data")

	return nil
}
