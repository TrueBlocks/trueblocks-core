package abi

import (
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"net/http"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/debug"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/utils"
)

var perfTiming bool

func init() {
	perfTiming = os.Getenv("TB_TIMER_ON") == "true"
}

// DO NOT CHANGE THIS VALUE. It is used to cache the error response from Etherscan.
// It's length is 42 bytes and is used to test for this case

var AbiNotFound = `[{"name":"AbiNotFound","type":"function"}]`

// downloadAbi downloads the ABI for the given address and saves it to the cache.
// TODO: This function should be easy to replace with "ABI providers" (different services like
// Sourcify or custom ones configured by the user)
func (abiMap *SelectorSyncMap) downloadAbi(conn *rpc.Connection, address base.Address) error {
	if address.IsZero() {
		return errors.New("address is 0x0 in downloadAbi")
	}

	// C++ code used do check if the address is contract in 2 places: here and in handle_addresses. We
	// check only in handle_addresses.

	key := config.GetKey("etherscan").ApiKey
	if key == "" {
		return errors.New("cannot read Etherscan API key")
	}
	url := fmt.Sprintf(
		"https://api.etherscan.io/v2/api?chainid=1&module=contract&action=getabi&address=%s&apikey=%s",
		address.Hex(),
		key,
	)

	debug.DebugCurlStr(url)
	resp, err := http.Get(url)
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	// Check server response
	if resp.StatusCode != http.StatusOK {
		return fmt.Errorf("etherscan API error: %s", resp.Status)
	}

	data := map[string]string{}
	decoder := json.NewDecoder(resp.Body)
	if err = decoder.Decode(&data); err != nil {
		return err
	}
	resp.Body.Close()

	if data["message"] == "NOTOK" {
		// Etherscan sends 200 OK responses even if there's an error. We want to cache the error
		// response so we don't keep asking Etherscan for the same address. The user may later
		// remove empty ABIs with chifra abis --clean.
		if !perfTiming && !base.IsTestMode() && !utils.IsFuzzing() {
			logger.Warn("provider responded with:", address.Hex(), data["message"], ss)
		}

		reader := strings.NewReader(AbiNotFound)
		_ = parseAbiIntoMap(reader, abiMap)
		if _, err = reader.Seek(0, io.SeekStart); err != nil {
			return err
		}
		if err = insertAbiNative(conn, address, reader); err != nil {
			return err
		}
		return nil
	}

	reader := strings.NewReader(data["result"])
	_ = parseAbiIntoMap(reader, abiMap)
	if _, err = reader.Seek(0, io.SeekStart); err != nil {
		return err
	}

	// Write the body to file using native caching
	if err = insertAbiNative(conn, address, reader); err != nil {
		return err
	}

	return nil
}

var ss = strings.Repeat(" ", 40)
