package slurpPkg

import (
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"net/http"
	"strconv"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type Paginator struct {
	page    int
	perPage int
}

func (opts *SlurpOptions) HandleShow() error {
	paginator := Paginator{
		page:    1,
		perPage: int(opts.PerPage),
	}
	if opts.Globals.TestMode {
		paginator.perPage = 100
	}

	chain := opts.Globals.Chain
	logger.Info("Processing", opts.Addrs, "--types:", opts.Types, opts.Blocks)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawEtherscan], errorChan chan error) {
		totalFetched := 0
		totalFiltered := 0
		for _, addr := range opts.Addrs {
			for _, tt := range opts.Types {
				done := false
				for !done {
					txs, nFetched, err := opts.GetTransactionsFromEtherscan(chain, addr, tt, &paginator)
					done = nFetched < paginator.perPage
					totalFetched += nFetched
					if err != nil {
						errorChan <- err
						continue
					}

					for _, tx := range txs {
						tx := tx
						if !opts.isInRange(uint(tx.BlockNumber), errorChan) {
							continue
						}
						modelChan <- &tx
						totalFiltered++
					}

					// Without this Etherscan chokes
					sleep := opts.Sleep
					if sleep > 0 {
						ms := time.Duration(sleep*1000) * time.Millisecond
						if !opts.Globals.TestMode {
							logger.Info(fmt.Sprintf("Sleeping for %g seconds", sleep))
						}
						time.Sleep(ms)
					}
				}
			}
		}

		if totalFiltered == 0 {
			msg := fmt.Sprintf("zero transactions reported, %d fetched", totalFetched)
			errorChan <- fmt.Errorf(msg)
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

var m = map[string]string{
	"ext":    "txlist",
	"int":    "txlistinternal",
	"token":  "tokentx",
	"nfts":   "tokennfttx",
	"1155":   "token1155tx",
	"miner":  "getminedblocks&blocktype=blocks",
	"uncles": "getminedblocks&blocktype=uncles",
}
var ss = map[string]string{
	"ext":    "asc",
	"int":    "asc",
	"token":  "asc",
	"nfts":   "asc",
	"1155":   "asc",
	"miner":  "asc",
	"uncles": "asc",
}

func (opts *SlurpOptions) getEtherscanUrl(addr string, tt string, paginator *Paginator) string {
	if ss[tt] == "" || m[tt] == "" {
		logger.Fatal("Should not happen in getEtherscanUrl", tt)
	}

	const str = "https://api.etherscan.io/api?module=account&sort=[{SORT}]&action=[{CMD}]&address=[{ADDRESS}]&page=[{PAGE}]&offset=[{PER_PAGE}]"
	ret := strings.Replace(str, "[{SORT}]", ss[tt], -1)
	ret = strings.Replace(ret, "[{CMD}]", m[tt], -1)
	ret = strings.Replace(ret, "[{ADDRESS}]", addr, -1)
	ret = strings.Replace(ret, "[{PAGE}]", fmt.Sprintf("%d", paginator.page), -1)
	ret = strings.Replace(ret, "[{PER_PAGE}]", fmt.Sprintf("%d", paginator.perPage), -1)
	paginator.page++
	return ret
}

func (opts *SlurpOptions) GetTransactionsFromEtherscan(chain string, addr, tt string, paginator *Paginator) ([]types.SimpleEtherscan, int, error) {
	url := opts.getEtherscanUrl(addr, tt, paginator)
	logger.Info("Processing", url)

	var ret []types.SimpleEtherscan

	key := config.GetRootConfig().Keys["etherscan"].ApiKey
	if key == "" {
		return ret, 0, errors.New("cannot read Etherscan API key")
	}
	url += "&apikey=" + key

	resp, err := http.Get(url)
	if err != nil {
		return ret, 0, err
	}
	defer resp.Body.Close()

	// Check server response
	if resp.StatusCode != http.StatusOK {
		return ret, 0, fmt.Errorf("etherscan API error: %s", resp.Status)
	}

	decoder := json.NewDecoder(resp.Body)
	fromEs := etherscanResponse{}
	if err = decoder.Decode(&fromEs); err != nil {
		return ret, 0, err
	}
	resp.Body.Close()

	if fromEs.Message == "NOTOK" {
		// Etherscan sends 200 OK responses even if there's an error. We want to cache the error
		// response so we don't keep asking Etherscan for the same address. The user may later
		// remove empty ABIs with chifra abis --clean.
		logger.Warn("provider responded with:", url, fromEs.Message)
		return ret, 0, nil
	}

	for _, esTx := range fromEs.Result {
		rawTx := types.RawEtherscan{
			BlockHash:        esTx.BlockHash,
			BlockNumber:      esTx.BlockNumber,
			From:             esTx.From,
			Gas:              esTx.Gas,
			GasPrice:         esTx.GasPrice,
			Hash:             esTx.Hash,
			Input:            esTx.Input,
			To:               esTx.To,
			TransactionIndex: esTx.TransactionIndex,
			Value:            esTx.Value,
		}
		// Nonce:            esTx.Nonce,

		t := types.SimpleEtherscan{
			Hash:             base.HexToHash(rawTx.Hash),
			BlockHash:        base.HexToHash(rawTx.BlockHash),
			BlockNumber:      mustParseUint(rawTx.BlockNumber),
			TransactionIndex: mustParseUint(rawTx.TransactionIndex),
			Timestamp:        mustParseInt(esTx.Timestamp),
			From:             base.HexToAddress(rawTx.From),
			To:               base.HexToAddress(rawTx.To),
			Gas:              mustParseUint(rawTx.Gas),
			GasPrice:         mustParseUint(rawTx.GasPrice),
			GasUsed:          mustParseUint(esTx.GasUsed),
			Input:            rawTx.Input,
			// Nonce:            mustParseUint(esTx.Nonce),
			// Value: big.Uint(rawTx.Value),
			// IsError:          esTx.TxReceiptStatus == "1",
			// GasCost: mustParseUint(rawTx.GasCost),
			// MaxFeePerGas         Gas             `json:"maxFeePerGas"`
			// MaxPriorityFeePerGas Gas             `json:"maxPriorityFeePerGas"`
			// HasToken             bool            `json:"hasToken,omitempty"`
		}
		t.GasCost = t.GasPrice * t.GasUsed
		t.IsError = esTx.TxReceiptStatus == "0"
		t.HasToken = tt == "nfts" || tt == "token" || tt == "1155"
		t.Value.SetString(rawTx.Value, 0)
		t.ContractAddress = base.HexToAddress(esTx.ContractAddress)
		if tt == "int" {
			// Markers to help us remove these since Etherscan doesn't send them and we don't want to make another RPC call
			t.BlockHash = base.HexToHash("0xdeadbeef")
			t.TransactionIndex = 80809
		} else if tt == "miner" {
			t.BlockHash = base.HexToHash("0xdeadbeef")
			t.TransactionIndex = 99999
			t.From = base.BlockRewardSender
			t.Value.SetString("5000000000000000000", 0)
			t.To = base.HexToAddress(addr)
		} else if tt == "uncles" {
			t.BlockHash = base.HexToHash("0xdeadbeef")
			t.TransactionIndex = 99998
			t.From = base.UncleRewardSender
			t.Value.SetString("3750000000000000000", 0)
			t.To = base.HexToAddress(addr)
		}

		// a := base.HexToAddress(esTx.ContractAddress)
		// b := mustParseUint(esTx.CumulativeGasUsed)
		// c := mustParseUint(esTx.GasUsed)
		// d := uint32(mustParseUint(esTx.TxReceiptStatus))
		// if a != base.HexToAddress("0x0") || b != 0 || c != 0 || d != 0 {
		// 	t.Receipt = &types.SimpleReceipt{
		// 		ContractAddress:   a,
		// 		CumulativeGasUsed: esTx.CumulativeGasUsed,
		// 		GasUsed:           c,
		// 		Status:            d,
		// 	}
		// 	t.Receipt.IsError = t.IsError
		// }
		t.SetRaw(&rawTx)

		ret = append(ret, t)
	}

	return ret, len(ret), nil
}

func mustParseUint(input any) (result uint64) {
	result, _ = strconv.ParseUint(fmt.Sprint(input), 0, 64)
	return
}

func mustParseInt(input any) (result int64) {
	result, _ = strconv.ParseInt(fmt.Sprint(input), 0, 64)
	return
}

func (opts *SlurpOptions) isInRange(bn uint, errorChan chan error) bool {
	chain := opts.Globals.Chain

	// Note that validation ensures that there is only a single isInRange
	if len(opts.BlockIds) == 0 {
		return true
	}

	br := opts.BlockIds[0]
	if strings.Contains(br.Orig, "-") && !strings.Contains(br.Orig, ":") {
		// a plain block range
		return br.Start.Number <= bn && bn <= br.End.Number
	}

	blockNums, err := br.ResolveBlocks(chain)
	if err != nil {
		errorChan <- err
		return false
	}
	for _, num := range blockNums {
		if uint(num) == bn {
			return true
		}
	}

	return false
}

type etherscanResponse struct {
	Message string               `json:"message"`
	Result  []types.RawEtherscan `json:"result"`
	Status  string               `json:"status"`
}
