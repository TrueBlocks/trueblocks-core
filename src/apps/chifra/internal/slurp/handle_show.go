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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/bykof/gostradamus"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

func (opts *SlurpOptions) HandleShowSlurps() error {
	chain := opts.Globals.Chain
	logger.Info("Processing", opts.Addrs, "--types:", opts.Types, opts.Blocks)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[RawEtherscan], errorChan chan error) {
		cnt := 0
		for _, addr := range opts.Addrs {
			for _, tt := range opts.Types {
				txs, err := opts.GetTransactionsFromEtherscan(chain, addr, tt)
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
					cnt++
				}
				if opts.Globals.TestMode {
					// Without this Etherscan chokes
					time.Sleep(250 * time.Millisecond)
				}
			}
		}

		if cnt == 0 {
			errorChan <- fmt.Errorf("no transactions found")
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

func (opts *SlurpOptions) getEtherscanUrl(addr string, tt string, page int) string {
	if ss[tt] == "" || m[tt] == "" {
		logger.Fatal("Should not happen in getEtherscanUrl", tt)
	}
	const str = "https://api.etherscan.io/api?module=account&sort=[{SORT}]&action=[{CMD}]&address=[{ADDRESS}]&page=[{PAGE}]&offset=5000"
	ret := strings.Replace(str, "[{SORT}]", ss[tt], -1)
	ret = strings.Replace(ret, "[{CMD}]", m[tt], -1)
	ret = strings.Replace(ret, "[{ADDRESS}]", addr, -1)
	ret = strings.Replace(ret, "[{PAGE}]", fmt.Sprintf("%d", page), -1)
	return ret
}

func (opts *SlurpOptions) GetTransactionsFromEtherscan(chain string, addr, tt string) ([]SimpleEtherscan, error) {
	url := opts.getEtherscanUrl(addr, tt, 1)
	logger.Info("Processing", url)

	var ret []SimpleEtherscan

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
	fromEs := EtherscanResponse{}
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
		rawTx := RawEtherscan{
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

		t := SimpleEtherscan{
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
		t.ContractAddress = types.HexToAddress(esTx.ContractAddress)
		if tt == "int" {
			// Markers to help us remove these since Etherscan doesn't send them and we don't want to make another RPC call
			t.BlockHash = common.HexToHash("0xdeadbeef")
			t.TransactionIndex = 80809
		} else if tt == "miner" {
			t.BlockHash = common.HexToHash("0xdeadbeef")
			t.TransactionIndex = 99999
			t.Input = "0xBlockReward"
			t.Value.SetString("5000000000000000000", 0)
			t.To = types.HexToAddress(addr)
		} else if tt == "uncles" {
			t.BlockHash = common.HexToHash("0xdeadbeef")
			t.TransactionIndex = 99998
			t.Input = "0xUncleReward"
			t.Value.SetString("3750000000000000000", 0)
			t.To = types.HexToAddress(addr)
		}

		// a := types.HexToAddress(esTx.ContractAddress)
		// b := mustParseUint(esTx.CumulativeGasUsed)
		// c := mustParseUint(esTx.GasUsed)
		// d := uint32(mustParseUint(esTx.TxReceiptStatus))
		// if a != types.HexToAddress("0x0") || b != 0 || c != 0 || d != 0 {
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

	return ret, nil
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
	// Note that validation ensures that there is only a single isInRange
	if len(opts.BlockIds) == 0 {
		return true
	}

	br := opts.BlockIds[0]
	if strings.Contains(br.Orig, "-") && !strings.Contains(br.Orig, ":") {
		// a plain block range
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

	return false
}

type RawEtherscan struct {
	BlockHash         string `json:"blockHash"`
	BlockNumber       string `json:"blockNumber"`
	ContractAddress   string `json:"contractAddress"`
	CumulativeGasUsed string `json:"cumulativeGasUsed"`
	From              string `json:"from"`
	Gas               string `json:"gas"`
	GasPrice          string `json:"gasPrice"`
	GasUsed           string `json:"gasUsed"`
	HasToken          string `json:"hasToken"`
	Hash              string `json:"hash"`
	Input             string `json:"input"`
	IsError           string `json:"isError"`
	Timestamp         string `json:"timestamp"`
	To                string `json:"to"`
	TransactionIndex  string `json:"transactionIndex"`
	TxReceiptStatus   string `json:"txreceipt_status"`
	Value             string `json:"value"`
	// FunctionName      string `json:"functionName"`
	// MethodId         string `json:"methodId"`
	// Nonce            string `json:"nonce"`
}

type SimpleEtherscan struct {
	BlockHash        common.Hash     `json:"blockHash"`
	BlockNumber      uint64          `json:"blockNumber"`
	ContractAddress  types.Address   `json:"contractAddress"`
	Date             string          `json:"date"`
	Ether            string          `json:"ether"`
	From             types.Address   `json:"from"`
	Gas              types.Gas       `json:"gas"`
	GasPrice         types.Gas       `json:"gasPrice"`
	GasUsed          types.Gas       `json:"gasUsed"`
	GasCost          types.Gas       `json:"gasCost"`
	HasToken         bool            `json:"hasToken"`
	Hash             common.Hash     `json:"hash"`
	Input            string          `json:"input"`
	IsError          bool            `json:"isError"`
	Timestamp        types.Timestamp `json:"timestamp"`
	To               types.Address   `json:"to"`
	TransactionIndex uint64          `json:"transactionIndex"`
	Value            types.Wei       `json:"value"`
	raw              *RawEtherscan
	// ArticulatedTx    SimpleFunction `json:"articulatedTx"`
	// CompressedTx     string         `json:"compressedTx"`
	// EtherGasPrice    string      `json:"etherGasPrice"`
	// ExtraValue1      Wei         `json:"extraValue1"`
	// ExtraValue2      Wei         `json:"extraValue2"`
}

func (s *SimpleEtherscan) Raw() *RawEtherscan {
	return s.raw
}

func (s *SimpleEtherscan) SetRaw(raw *RawEtherscan) {
	s.raw = raw
}

func (s *SimpleEtherscan) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	to := hexutil.Encode(s.To.Bytes())
	if to == "0x0000000000000000000000000000000000000000" {
		to = "0x0" // weird special case to preserve what RPC does
	}

	date := gostradamus.FromUnixTimestamp(s.Timestamp)

	model := map[string]interface{}{
		"blockNumber": s.BlockNumber,
		"date":        s.Date,
		"ether":       s.Ether,
		"from":        s.From,
		"timestamp":   s.Timestamp,
		"to":          s.To,
		"value":       s.Value.String(),
		// "hash":        s.Hash,
		// "gas":         s.Gas,
		// "gasCost":     s.GasCost,
		// "gasPrice":    s.GasPrice,
		// "gasUsed":     s.GasUsed,
		// "blockHash":        s.BlockHash,
		// "transactionIndex": s.TransactionIndex,
		// "input":            s.Input,
		// "hasToken":         s.HasToken,
		// "isError":          s.IsError,
		// "contractAddress":  s.ContractAddress,
		// "articulatedTx":    s.ArticulatedTx,
		// "compressedTx":     s.CompressedTx,
		// "etherGasPrice":    s.EtherGasPrice,
		// "extraValue1":      s.ExtraValue1,
		// "extraValue2":      s.ExtraValue2,
	}

	var order []string

	model["date"] = date.Format("2006-01-02 15:04:05") + " UTC"
	if strings.Contains(s.Input, "Reward") {
		model["from"] = s.Input
		s.Input = ""
		order = []string{
			"blockNumber",
			"timestamp",
			"date",
			"from",
			"to",
			"value",
			"ether",
		}

	} else {
		order = []string{
			// "blockHash",
			"blockNumber",
			"transactionIndex",
			"timestamp",
			// "contractAddress",
			"date",
			"from",
			"to",
			"hasToken",
			"isError",
			"hash",
			"gasPrice",
			"gasUsed",
			"gasCost",
			"value",
			"ether",
			"input",
			// "articulatedTx",
			// "compressedTx",
			// "etherGasPrice",
			// "extraValue1",
			// "extraValue2",
		}

		model["gas"] = s.Gas
		model["gasCost"] = s.SetGasCost()
		model["gasPrice"] = s.GasPrice
		model["gasUsed"] = s.GasUsed
		model["hash"] = s.Hash
	}

	if s.HasToken {
		model["hasToken"] = s.HasToken
	}
	if s.IsError {
		model["isError"] = s.IsError
	}
	model["ether"] = utils.WeiToEther(&s.Value).Text('f', 18)
	if s.BlockHash != common.HexToHash("0xdeadbeef") {
		model["blockHash"] = s.BlockHash
	}
	if s.TransactionIndex != 80809 {
		model["transactionIndex"] = s.TransactionIndex
	}

	if format == "json" {
		if validate.IsValidAddress(s.ContractAddress.Hex()) {
			model["contractAddress"] = s.ContractAddress.Hex()
		}
		if len(s.Input) > 0 && s.Input != "deprecated" {
			model["input"] = s.Input
		}
	} else {
		model["hasToken"] = s.HasToken
		model["isError"] = s.IsError
		if s.Input == "deprecated" {
			s.Input = "0x"
		}
		model["input"] = s.Input
	}

	return types.Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleEtherscan) SetGasCost() types.Gas {
	s.GasCost = s.GasPrice * s.GasUsed
	return s.GasCost
}

type EtherscanResponse struct {
	Message string         `json:"message"`
	Result  []RawEtherscan `json:"result"`
	Status  string         `json:"status"`
}
