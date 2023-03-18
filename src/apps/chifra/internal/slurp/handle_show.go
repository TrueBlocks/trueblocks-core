package slurpPkg

import (
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"net/http"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
)

func (opts *SlurpOptions) HandleShowSlurps() error {
	chain := opts.Globals.Chain
	logger.Info("Processing", opts.Addrs, "--types:", opts.Types, opts.Blocks)

	if len(opts.Types) == 0 {
		opts.Types = append(opts.Types, "txlist")
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawTransaction], errorChan chan error) {
		for _, addr := range opts.Addrs {
			for _, tt := range opts.Types {
				url := opts.getEtherscanUrl(addr, tt, 1)
				logger.Info("Processing", url)
				txs, err := GetTransactionsFromEtherscan(chain, url)
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
				}
			}
		}
	}

	return output.StreamMany(ctx, fetchData, output.OutputOptions{
		Writer:     opts.Globals.Writer,
		Chain:      opts.Globals.Chain,
		TestMode:   opts.Globals.TestMode,
		NoHeader:   opts.Globals.NoHeader,
		ShowRaw:    opts.Globals.ShowRaw,
		Verbose:    opts.Globals.Verbose,
		LogLevel:   opts.Globals.LogLevel,
		Format:     opts.Globals.Format,
		OutputFn:   opts.Globals.OutputFn,
		Append:     opts.Globals.Append,
		JsonIndent: "  ",
	})
}

var m = map[string]string{
	"ext":    "txlist",
	"int":    "txlistinternal",
	"nfts":   "tokennfttx",
	"token":  "tokentx",
	"uncles": "getminedblocks&blocktype=uncles",
}

func (opts *SlurpOptions) getEtherscanUrl(addr string, tt string, page int) string {
	const str = "https://api.etherscan.io/api?module=account&sort=asc&action=[{CMD}]&address=[{ADDRESS}]&page=[{PAGE}]&offset=5000"
	ret := strings.Replace(str, "[{CMD}]", m[tt], -1)
	ret = strings.Replace(ret, "[{ADDRESS}]", addr, -1)
	ret = strings.Replace(ret, "[{PAGE}]", fmt.Sprintf("%d", page), -1)
	return ret
}

func GetTransactionsFromEtherscan(chain string, url string) ([]types.SimpleTransaction, error) {
	var ret []types.SimpleTransaction

	key := config.GetRootConfig().Keys["etherscan"].ApiKey
	if key == "" {
		return ret, errors.New("cannot read Etherscan API key")
	}
	url += "&apikey=" + key

	resp, err := http.Get(url)
	if err != nil {
		return ret, err
	}
	defer resp.Body.Close()

	// Check server response
	if resp.StatusCode != http.StatusOK {
		return ret, fmt.Errorf("etherscan API error: %s", resp.Status)
	}

	decoder := json.NewDecoder(resp.Body)
	fromEs := FromEtherscan{}
	if err = decoder.Decode(&fromEs); err != nil {
		return ret, err
	}
	resp.Body.Close()

	if fromEs.Message == "NOTOK" {
		// Etherscan sends 200 OK responses even if there's an error. We want to cache the error
		// response so we don't keep asking Etherscan for the same address. The user may later
		// remove empty ABIs with chifra abis --clean.
		logger.Warn("provider responded with:", url, fromEs.Message)
		return ret, nil
	}

	for _, esTx := range fromEs.Result {
		rawTx := types.RawTransaction{
			BlockHash:        esTx.BlockHash,
			BlockNumber:      esTx.BlockNumber,
			From:             esTx.From,
			Gas:              esTx.Gas,
			GasPrice:         esTx.GasPrice,
			Hash:             esTx.Hash,
			Input:            esTx.Input,
			Nonce:            esTx.Nonce,
			To:               esTx.To,
			TransactionIndex: esTx.TransactionIndex,
			Value:            esTx.Value,
		}

		t := types.SimpleTransaction{
			Hash:             common.HexToHash(rawTx.Hash),
			BlockHash:        common.HexToHash(rawTx.BlockHash),
			BlockNumber:      mustParseUint(rawTx.BlockNumber),
			TransactionIndex: mustParseUint(rawTx.TransactionIndex),
			Timestamp:        mustParseInt(esTx.Timestamp),
			From:             types.HexToAddress(rawTx.From),
			To:               types.HexToAddress(rawTx.To),
			Gas:              mustParseUint(rawTx.Gas),
			GasPrice:         mustParseUint(rawTx.GasPrice),
			GasUsed:          mustParseUint(esTx.GasUsed),
			Nonce:            mustParseUint(esTx.Nonce),
			Input:            rawTx.Input,
			// Value: big.Uint(rawTx.Value),
			// IsError:          esTx.TxReceiptStatus == "1",
			// GasCost: mustParseUint(rawTx.GasCost),
			// MaxFeePerGas         Gas             `json:"maxFeePerGas"`
			// MaxPriorityFeePerGas Gas             `json:"maxPriorityFeePerGas"`
			// HasToken             bool            `json:"hasToken,omitempty"`
		}
		t.GasCost = t.GasPrice * t.GasUsed
		t.IsError = esTx.TxReceiptStatus == "1"
		t.Value.SetString(rawTx.Value, 0)

		a := types.HexToAddress(esTx.ContractAddress)
		b := mustParseUint(esTx.CumulativeGasUsed)
		c := mustParseUint(esTx.GasUsed)
		d := uint32(mustParseUint(esTx.TxReceiptStatus))
		if a != types.HexToAddress("0x0") || b != 0 || c != 0 || d != 0 {
			t.Receipt = &types.SimpleReceipt{
				ContractAddress:   a,
				CumulativeGasUsed: esTx.CumulativeGasUsed,
				GasUsed:           c,
				Status:            d,
			}
			t.Receipt.IsError = t.IsError
		}
		t.SetRaw(&rawTx)

		ret = append(ret, t)
	}

	return ret, nil
}

type SimpleEtherscan struct {
	BlockHash         string `json:"blockHash"`
	BlockNumber       string `json:"blockNumber"`
	ContractAddress   string `json:"contractAddress"`
	CumulativeGasUsed string `json:"cumulativeGasUsed"`
	From              string `json:"from"`
	FunctionName      string `json:"functionName"`
	Gas               string `json:"gas"`
	GasPrice          string `json:"gasPrice"`
	GasUsed           string `json:"gasUsed"`
	Hash              string `json:"hash"`
	Input             string `json:"input"`
	IsError           string `json:"isError"`
	MethodId          string `json:"methodId"`
	Nonce             string `json:"nonce"`
	Timestamp         string `json:"timeStamp"`
	To                string `json:"To"`
	TransactionIndex  string `json:"transactionIndex"`
	TxReceiptStatus   string `json:"txreceipt_status"`
	Value             string `json:"value"`
}

type FromEtherscan struct {
	Message string            `json:"message"`
	Result  []SimpleEtherscan `json:"result"`
	Status  string            `json:"status"`
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
	for _, br := range opts.BlockIds {
		// fmt.Println(br)
		if strings.Contains(br.Orig, "-") && !strings.Contains(br.Orig, ":") {
			// a plain block range
			// fmt.Println("range", br.Start.Number <= bn, bn <= br.End.Number, br.Start.Number <= bn && bn <= br.End.Number)
			return br.Start.Number <= bn && bn <= br.End.Number
		}

		blockNums, err := br.ResolveBlocks(opts.Globals.Chain)
		if err != nil {
			errorChan <- err
			return false
		}
		for _, num := range blockNums {
			if uint(num) == bn {
				return true
			}
		}
	}
	return false
}
