package abi

import (
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"net/http"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var perfTiming bool

func init() {
	perfTiming = os.Getenv("TB_TIMER_ON") == "true"
}

var AbiNotFound = `[{"name":"AbiNotFound","type":"function"}]`

// DownloadAbi downloads the ABI for the given address and saves it to the cache.
// TODO: This function should be easy to replace with "ABI providers" (different services like
// Sourcify or custom ones configured by the user)
func DownloadAbi(chain string, address base.Address, destination *FunctionSyncMap) error {
	if address.IsZero() {
		return errors.New("address is 0x0")
	}

	// C++ code used do check if the address is contract in 2 places: here and in handle_addresses. We
	// check only in handle_addresses.

	key := config.GetKey("etherscan").ApiKey
	if key == "" {
		return errors.New("cannot read Etherscan API key")
	}
	url := fmt.Sprintf(
		"https://api.etherscan.io/api?module=contract&action=getabi&address=%s&apikey=%s",
		address.Hex(),
		key,
	)

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
		if !perfTiming {
			logger.Warn("provider responded with:", address.Hex(), data["message"])
		}

		reader := strings.NewReader(AbiNotFound)
		_ = fromJson(reader, destination)
		if _, err = reader.Seek(0, io.SeekStart); err != nil {
			return err
		}
		if err = insertAbi(chain, address, reader); err != nil {
			return err
		}
		return nil
	}

	reader := strings.NewReader(data["result"])
	_ = fromJson(reader, destination)
	if _, err = reader.Seek(0, io.SeekStart); err != nil {
		return err
	}

	// Write the body to file
	if err = insertAbi(chain, address, reader); err != nil {
		return err
	}
	return nil
}
