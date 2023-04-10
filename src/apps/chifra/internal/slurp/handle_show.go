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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

type Paginator struct {
	page    int
	perPage int
}

func (opts *SlurpOptions) HandleShowSlurps() error {
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
	fetchData := func(modelChan chan types.Modeler[rawEtherscan], errorChan chan error) {
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

func (opts *SlurpOptions) GetTransactionsFromEtherscan(chain string, addr, tt string, paginator *Paginator) ([]simpleEtherscan, int, error) {
	url := opts.getEtherscanUrl(addr, tt, paginator)
	logger.Info("Processing", url)

	var ret []simpleEtherscan

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
		rawTx := rawEtherscan{
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

		t := simpleEtherscan{
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
			t.Input = "0xBlockReward"
			t.Value.SetString("5000000000000000000", 0)
			t.To = base.HexToAddress(addr)
		} else if tt == "uncles" {
			t.BlockHash = base.HexToHash("0xdeadbeef")
			t.TransactionIndex = 99998
			t.Input = "0xUncleReward"
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

type rawEtherscan struct {
	BlockHash         string `json:"blockHash"`
	BlockNumber       string `json:"blockNumber"`
	ContractAddress   string `json:"contractAddress"`
	CumulativeGasUsed string `json:"cumulativeGasUsed"`
	From              string `json:"from"`
	FunctionName      string `json:"functionName"`
	Gas               string `json:"gas"`
	GasPrice          string `json:"gasPrice"`
	GasUsed           string `json:"gasUsed"`
	HasToken          string `json:"hasToken"`
	Hash              string `json:"hash"`
	Input             string `json:"input"`
	IsError           string `json:"isError"`
	MethodId          string `json:"methodId"`
	Nonce             string `json:"nonce"`
	Timestamp         string `json:"timestamp"`
	To                string `json:"to"`
	TransactionIndex  string `json:"transactionIndex"`
	TxReceiptStatus   string `json:"txreceipt_status"`
	Value             string `json:"value"`
}

// TODO: BOGUS2 - MUST DOCUMENT
type simpleEtherscan struct {
	BlockHash        base.Hash      `json:"blockHash"`
	BlockNumber      uint64         `json:"blockNumber"`
	ContractAddress  base.Address   `json:"contractAddress"`
	Date             string         `json:"date"`
	Ether            string         `json:"ether"`
	From             base.Address   `json:"from"`
	Gas              base.Gas       `json:"gas"`
	GasPrice         base.Gas       `json:"gasPrice"`
	GasUsed          base.Gas       `json:"gasUsed"`
	GasCost          base.Gas       `json:"gasCost"`
	HasToken         bool           `json:"hasToken"`
	Hash             base.Hash      `json:"hash"`
	Input            string         `json:"input"`
	IsError          bool           `json:"isError"`
	Timestamp        base.Timestamp `json:"timestamp"`
	To               base.Address   `json:"to"`
	TransactionIndex uint64         `json:"transactionIndex"`
	Value            base.Wei       `json:"value"`
	raw              *rawEtherscan  `json:"-"`
}

func (s *simpleEtherscan) Raw() *rawEtherscan {
	return s.raw
}

func (s *simpleEtherscan) SetRaw(r *rawEtherscan) {
	s.raw = r
}

func (s *simpleEtherscan) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	to := hexutil.Encode(s.To.Bytes())
	if to == "0x0000000000000000000000000000000000000000" {
		to = "0x0" // weird special case to preserve what RPC does
	}

	model = map[string]interface{}{
		"blockNumber": s.BlockNumber,
		"date":        s.Date,
		"ether":       s.Ether,
		"from":        s.From,
		"timestamp":   s.Timestamp,
		"to":          s.To,
		"value":       s.Value.String(),
	}
	model["date"] = utils.FormattedDate(s.Timestamp)

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
			"blockNumber",
			"transactionIndex",
			"timestamp",
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
	if s.BlockHash != base.HexToHash("0xdeadbeef") {
		model["blockHash"] = s.BlockHash
	}
	if s.TransactionIndex != 80809 {
		model["transactionIndex"] = s.TransactionIndex
	}

	if format == "json" {
		a := s.ContractAddress.Hex()
		if strings.HasPrefix(a, "0x") && len(a) == 42 {
			model["contractAddress"] = a
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

func (s *simpleEtherscan) SetGasCost() base.Gas {
	s.GasCost = s.GasPrice * s.GasUsed
	return s.GasCost
}

type etherscanResponse struct {
	Message string         `json:"message"`
	Result  []rawEtherscan `json:"result"`
	Status  string         `json:"status"`
}
