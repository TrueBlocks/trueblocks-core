// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"fmt"
	"io"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type StorageSlot struct {
	Address     base.Address `json:"address"`
	StorageKeys []base.Hash  `json:"storageKeys"`
}

type Rewards struct {
	Block  base.MyWei `json:"block"`
	Nephew base.MyWei `json:"nephew"`
	TxFee  base.MyWei `json:"txFee"`
	Uncle  base.MyWei `json:"uncle"`
}

func NewReward(block, nephew, txFee, uncle *base.MyWei) (Rewards, base.MyWei) {
	total := new(base.MyWei).Add(block, nephew)
	total = total.Add(total, txFee)
	total = total.Add(total, uncle)
	return Rewards{
		Block:  *block,
		Nephew: *nephew,
		TxFee:  *txFee,
		Uncle:  *uncle,
	}, *total
}

// EXISTING_CODE

type RawTransaction struct {
	AccessList           []StorageSlot `json:"accessList"`
	BlockHash            string        `json:"blockHash"`
	BlockNumber          string        `json:"blockNumber"`
	ChainId              string        `json:"chainId"`
	From                 string        `json:"from"`
	Gas                  string        `json:"gas"`
	GasPrice             string        `json:"gasPrice"`
	Hash                 string        `json:"hash"`
	Input                string        `json:"input"`
	MaxFeePerGas         string        `json:"maxFeePerGas"`
	MaxPriorityFeePerGas string        `json:"maxPriorityFeePerGas"`
	Nonce                string        `json:"nonce"`
	To                   string        `json:"to"`
	TransactionIndex     string        `json:"transactionIndex"`
	TransactionType      string        `json:"type"`
	Value                string        `json:"value"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleTransaction struct {
	ArticulatedTx        *SimpleFunction `json:"articulatedTx"`
	BlockHash            base.Hash       `json:"blockHash"`
	BlockNumber          base.Blknum     `json:"blockNumber"`
	CompressedTx         string          `json:"compressedTx"`
	From                 base.Address    `json:"from"`
	Gas                  base.Gas        `json:"gas"`
	GasPrice             base.Gas        `json:"gasPrice"`
	GasUsed              base.Gas        `json:"gasUsed"`
	HasToken             bool            `json:"hasToken"`
	Hash                 base.Hash       `json:"hash"`
	Input                string          `json:"input"`
	IsError              bool            `json:"isError"`
	MaxFeePerGas         base.Gas        `json:"maxFeePerGas"`
	MaxPriorityFeePerGas base.Gas        `json:"maxPriorityFeePerGas"`
	Nonce                uint64          `json:"nonce"`
	Receipt              *SimpleReceipt  `json:"receipt"`
	Timestamp            base.Timestamp  `json:"timestamp"`
	To                   base.Address    `json:"to"`
	Traces               []SimpleTrace   `json:"traces"`
	TransactionIndex     base.Blknum     `json:"transactionIndex"`
	TransactionType      string          `json:"type"`
	Value                base.Wei        `json:"value"`
	raw                  *RawTransaction `json:"-"`
	// EXISTING_CODE
	Message    string             `json:"-"`
	Rewards    *Rewards           `json:"-"`
	Statements *[]SimpleStatement `json:"statements"`
	// EXISTING_CODE
}

func (s *SimpleTransaction) Raw() *RawTransaction {
	return s.raw
}

func (s *SimpleTransaction) SetRaw(raw *RawTransaction) {
	s.raw = raw
}

func (s *SimpleTransaction) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	to := s.To.Hex()
	if to == "0x0000000000000000000000000000000000000000" {
		to = "0x0" // weird special case to preserve what RPC does
	}

	asEther := extraOptions["ether"] == true
	model = map[string]interface{}{
		"blockNumber":      s.BlockNumber,
		"from":             s.From,
		"gasPrice":         s.GasPrice,
		"gasUsed":          s.GasUsed,
		"hash":             s.Hash,
		"timestamp":        s.Timestamp,
		"date":             s.Date(),
		"to":               to,
		"transactionIndex": s.TransactionIndex,
		"value":            base.FormattedValue(&s.Value, asEther, 18),
	}

	order = []string{
		"blockNumber",
		"transactionIndex",
		"timestamp",
		"date",
		"from",
		"to",
		"ether",
		"gasPrice",
		"gasUsed",
		"gasCost",
		"hash",
		"isError",
		"encoding",
	}

	model["gasCost"] = s.GasCost()

	// TODO: Shouldn't this use the SimpleFunction model - the answer is yes?
	var articulatedTx map[string]interface{}
	isArticulated := extraOptions["articulate"] == true && s.ArticulatedTx != nil
	if isArticulated && format != "json" {
		order = append(order, "compressedTx")
	}
	if isArticulated {
		articulatedTx = map[string]interface{}{
			"name": s.ArticulatedTx.Name,
		}
		inputModels := parametersToMap(s.ArticulatedTx.Inputs)
		if inputModels != nil {
			articulatedTx["inputs"] = inputModels
		}
		outputModels := parametersToMap(s.ArticulatedTx.Outputs)
		if outputModels != nil {
			articulatedTx["outputs"] = outputModels
		}
		sm := s.ArticulatedTx.StateMutability
		if sm != "" && sm != "nonpayable" && sm != "view" {
			articulatedTx["stateMutability"] = sm
		}
	}

	if format == "json" {
		if s.Statements != nil {
			statements := make([]map[string]any, 0, len(*s.Statements))
			for _, statement := range *s.Statements {
				statements = append(statements, statement.Model(chain, format, verbose, extraOptions).Data)
			}
			model["statements"] = statements
		}
		model["blockHash"] = s.BlockHash
		if s.Nonce > 0 {
			model["nonce"] = s.Nonce
		}
		model["value"] = base.FormattedValue(&s.Value, asEther, 18)
		model["gas"] = s.Gas

		model["ether"] = base.FormattedValue(&s.Value, true, 18)
		if s.MaxFeePerGas > 0 {
			model["maxFeePerGas"] = s.MaxFeePerGas
		}
		if s.MaxPriorityFeePerGas > 0 {
			model["maxPriorityFeePerGas"] = s.MaxPriorityFeePerGas
		}
		if len(s.TransactionType) > 0 && s.TransactionType != "0x0" {
			model["type"] = s.TransactionType
		}
		if len(s.Input) > 2 {
			model["input"] = s.Input
		}
		if s.HasToken {
			model["hasToken"] = s.HasToken
		}
		if s.IsError {
			model["isError"] = s.IsError
		}

		if s.Receipt != nil && !s.Receipt.IsDefault() {
			contractAddress := s.Receipt.ContractAddress.Hex()

			// TODO: This is quite odd. Why?
			status := &s.Receipt.Status
			if s.BlockNumber < base.KnownBlock(chain, base.Byzantium) || *status == 4294967295-1 {
				status = nil
			}

			receiptModel := map[string]any{
				"contractAddress":   contractAddress,
				"effectiveGasPrice": s.Receipt.EffectiveGasPrice,
				"gasUsed":           s.Receipt.GasUsed,
				"status":            status,
			}

			// TODO: We've already made a copy of the data that we've queried from the chain,
			// TODO: why are we copying it yet again? Can't we use pointers to the one copy of the data?
			logs := make([]map[string]any, 0, len(s.Receipt.Logs))
			for _, log := range s.Receipt.Logs {
				logModel := map[string]any{
					"address":   log.Address.Hex(),
					"logIndex":  log.LogIndex,
					"topics":    log.Topics,
					"data":      log.Data,
					"timestamp": s.Timestamp,
					"date":      s.Date(),
				}
				if extraOptions["articulate"] == true && log.ArticulatedLog != nil {
					inputModels := parametersToMap(log.ArticulatedLog.Inputs)
					articulatedLog := map[string]any{
						"name":   log.ArticulatedLog.Name,
						"inputs": inputModels,
					}
					logModel["articulatedLog"] = articulatedLog
				}
				logs = append(logs, logModel)
			}
			receiptModel["logs"] = logs
			model["receipt"] = receiptModel
		} else {
			model["receipt"] = map[string]interface{}{}
		}

		if extraOptions["traces"] == true && len(s.Traces) > 0 {
			traceModels := make([]map[string]any, 0, len(s.Traces))
			for _, trace := range s.Traces {
				traceModels = append(traceModels, trace.Model(chain, format, verbose, extraOptions).Data)
			}
			model["traces"] = traceModels
		} else {
			model["traces"] = make([]map[string]any, 0)
		}

		if isArticulated {
			model["articulatedTx"] = articulatedTx

		} else {
			if s.Message != "" {
				model["message"] = s.Message
			}
		}

	} else {
		if s.TransactionType != "0x0" {
			model["type"] = s.TransactionType
		} else {
			model["type"] = ""
		}
		order = append(order, "type")
		model["ether"] = base.FormattedValue(&s.Value, true, 18)
		ethGasPrice := base.FormattedValue(base.NewWei(0).SetUint64(s.GasPrice), true, 18)
		model["ethGasPrice"] = ethGasPrice
		model["isError"] = s.IsError

		if extraOptions["articulate"] == true && s.ArticulatedTx != nil {
			model["encoding"] = s.ArticulatedTx.Encoding
		}

		model["compressedTx"] = ""
		enc := s.Input
		if len(s.Input) >= 10 {
			enc = s.Input[:10]
		}
		model["encoding"] = enc

		if isArticulated {
			model["compressedTx"] = makeCompressed(articulatedTx)
		} else if s.Message != "" {
			model["encoding"] = ""
			model["compressedTx"] = s.Message
		}

		if extraOptions["traces"] == true {
			model["nTraces"] = len(s.Traces)
			order = append(order, "nTraces")
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleTransaction) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

// --> cacheable by tx
func (s *SimpleTransaction) CacheName() string {
	return "Transaction"
}

func (s *SimpleTransaction) CacheId() string {
	return fmt.Sprintf("%09d-%05d", s.BlockNumber, s.TransactionIndex)
}

func (s *SimpleTransaction) CacheLocation() (directory string, extension string) {
	paddedId := s.CacheId()
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]

	subFolder := strings.ToLower(s.CacheName()) + "s"
	directory = filepath.Join(subFolder, filepath.Join(parts...))
	extension = "bin"

	return
}

func (s *SimpleTransaction) MarshalCache(writer io.Writer) (err error) {
	// ArticulatedTx
	optArticulatedTx := &cache.Optional[SimpleFunction]{
		Value: s.ArticulatedTx,
	}
	if err = cache.WriteValue(writer, optArticulatedTx); err != nil {
		return err
	}

	// BlockHash
	if err = cache.WriteValue(writer, &s.BlockHash); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// From
	if err = cache.WriteValue(writer, s.From); err != nil {
		return err
	}

	// Gas
	if err = cache.WriteValue(writer, s.Gas); err != nil {
		return err
	}

	// GasPrice
	if err = cache.WriteValue(writer, s.GasPrice); err != nil {
		return err
	}

	// GasUsed
	if err = cache.WriteValue(writer, s.GasUsed); err != nil {
		return err
	}

	// HasToken
	if err = cache.WriteValue(writer, s.HasToken); err != nil {
		return err
	}

	// Hash
	if err = cache.WriteValue(writer, &s.Hash); err != nil {
		return err
	}

	// Input
	if err = cache.WriteValue(writer, s.Input); err != nil {
		return err
	}

	// IsError
	if err = cache.WriteValue(writer, s.IsError); err != nil {
		return err
	}

	// MaxFeePerGas
	if err = cache.WriteValue(writer, s.MaxFeePerGas); err != nil {
		return err
	}

	// MaxPriorityFeePerGas
	if err = cache.WriteValue(writer, s.MaxPriorityFeePerGas); err != nil {
		return err
	}

	// Nonce
	if err = cache.WriteValue(writer, s.Nonce); err != nil {
		return err
	}

	// Receipt
	optReceipt := &cache.Optional[SimpleReceipt]{
		Value: s.Receipt,
	}
	if err = cache.WriteValue(writer, optReceipt); err != nil {
		return err
	}

	// Timestamp
	if err = cache.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	// To
	if err = cache.WriteValue(writer, s.To); err != nil {
		return err
	}

	// TransactionIndex
	if err = cache.WriteValue(writer, s.TransactionIndex); err != nil {
		return err
	}

	// TransactionType
	if err = cache.WriteValue(writer, s.TransactionType); err != nil {
		return err
	}

	// Value
	if err = cache.WriteValue(writer, &s.Value); err != nil {
		return err
	}

	return nil
}

func (s *SimpleTransaction) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	// ArticulatedTx
	optArticulatedTx := &cache.Optional[SimpleFunction]{
		Value: s.ArticulatedTx,
	}
	if err = cache.ReadValue(reader, optArticulatedTx, version); err != nil {
		return err
	}
	s.ArticulatedTx = optArticulatedTx.Get()

	// BlockHash
	if err = cache.ReadValue(reader, &s.BlockHash, version); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.ReadValue(reader, &s.BlockNumber, version); err != nil {
		return err
	}

	// From
	if err = cache.ReadValue(reader, &s.From, version); err != nil {
		return err
	}

	// Gas
	if err = cache.ReadValue(reader, &s.Gas, version); err != nil {
		return err
	}

	// GasPrice
	if err = cache.ReadValue(reader, &s.GasPrice, version); err != nil {
		return err
	}

	// GasUsed
	if err = cache.ReadValue(reader, &s.GasUsed, version); err != nil {
		return err
	}

	// HasToken
	if err = cache.ReadValue(reader, &s.HasToken, version); err != nil {
		return err
	}

	// Hash
	if err = cache.ReadValue(reader, &s.Hash, version); err != nil {
		return err
	}

	// Input
	if err = cache.ReadValue(reader, &s.Input, version); err != nil {
		return err
	}

	// IsError
	if err = cache.ReadValue(reader, &s.IsError, version); err != nil {
		return err
	}

	// MaxFeePerGas
	if err = cache.ReadValue(reader, &s.MaxFeePerGas, version); err != nil {
		return err
	}

	// MaxPriorityFeePerGas
	if err = cache.ReadValue(reader, &s.MaxPriorityFeePerGas, version); err != nil {
		return err
	}

	// Nonce
	if err = cache.ReadValue(reader, &s.Nonce, version); err != nil {
		return err
	}

	// Receipt
	optReceipt := &cache.Optional[SimpleReceipt]{
		Value: s.Receipt,
	}
	if err = cache.ReadValue(reader, optReceipt, version); err != nil {
		return err
	}
	s.Receipt = optReceipt.Get()

	// Timestamp
	if err = cache.ReadValue(reader, &s.Timestamp, version); err != nil {
		return err
	}

	// To
	if err = cache.ReadValue(reader, &s.To, version); err != nil {
		return err
	}

	// TransactionIndex
	if err = cache.ReadValue(reader, &s.TransactionIndex, version); err != nil {
		return err
	}

	// TransactionType
	if err = cache.ReadValue(reader, &s.TransactionType, version); err != nil {
		return err
	}

	// Value
	if err = cache.ReadValue(reader, &s.Value, version); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

func (s *SimpleTransaction) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

// NewRawTransactionFromMap is useful when we get a map of transaction properties, e.g.
// from a call to eth_getBlockByHash [0x..., true]
func NewRawTransactionFromMap(input map[string]any) (r *RawTransaction) {
	r = &RawTransaction{}

	// TODO: I wonder why we make copies here
	r.BlockHash = fmt.Sprint(input["blockHash"])
	r.BlockNumber = fmt.Sprint(input["blockNumber"])
	// Missing in block query
	if _, ok := input["chainId"]; ok {
		r.ChainId = fmt.Sprint(input["chainId"])
	}
	r.From = fmt.Sprint(input["from"])
	r.Gas = fmt.Sprint(input["gas"])
	r.GasPrice = fmt.Sprint(input["gasPrice"])
	r.Hash = fmt.Sprint(input["hash"])
	r.Input = fmt.Sprint(input["input"])
	r.MaxFeePerGas = fmt.Sprint(input["maxFeePerGas"])
	r.MaxPriorityFeePerGas = fmt.Sprint(input["maxPriorityFeePerGas"])
	r.Nonce = fmt.Sprint(input["nonce"])
	r.To = fmt.Sprint(input["to"])
	r.TransactionIndex = fmt.Sprint(input["transactionIndex"])
	r.Value = fmt.Sprint(input["value"])
	r.TransactionType = fmt.Sprint(input["type"])

	return
}

// NewSimpleTransaction builds SimpleTransaction using data from raw and receipt. Receipt can be nil.
// Transaction timestamp and HasToken flag will be set to timestamp and hasToken.
func NewSimpleTransaction(raw *RawTransaction, receipt *SimpleReceipt, timestamp base.Timestamp) (s *SimpleTransaction) {
	hasToken := isTokenFunction(raw.Input)
	s = &SimpleTransaction{}

	// TODO: use RawTransaction methods
	s.Hash = base.HexToHash(raw.Hash)
	s.BlockHash = base.HexToHash(raw.BlockHash)
	s.BlockNumber = utils.MustParseUint(raw.BlockNumber)
	s.TransactionIndex = utils.MustParseUint(raw.TransactionIndex)
	s.Nonce = utils.MustParseUint(raw.Nonce)
	s.Timestamp = timestamp
	s.From = base.HexToAddress(raw.From)
	s.To = base.HexToAddress(raw.To)
	s.Value.SetString(raw.Value, 0)
	s.Gas = utils.MustParseUint(raw.Gas)
	s.GasPrice = utils.MustParseUint(raw.GasPrice)
	s.MaxFeePerGas = utils.MustParseUint(raw.MaxFeePerGas)
	s.MaxPriorityFeePerGas = utils.MustParseUint(raw.MaxPriorityFeePerGas)
	s.Input = raw.Input
	s.TransactionType = raw.TransactionType

	s.HasToken = hasToken
	if receipt != nil {
		s.GasUsed = receipt.GasUsed
		s.IsError = receipt.IsError
		s.Receipt = receipt
	}
	s.SetRaw(raw)

	return
}

func isTokenFunction(needle string) bool {
	var tokenRelated = map[string]bool{
		"0x095ea7b3": true, // approve(address spender, uint256 value)
		"0xa9059cbb": true, // transfer(address from, uint256 to);
		"0x23b872dd": true, // transferFrom(address from, address to, uint256 value)
		"0xb3e1c718": true, // _safeMint(address, uint256)
		"0x6a4f832b": true, // _safeMint(address, uint256, bytes)
		"0xa1448194": true, // safeMint(address, uint256)
		"0x8832e6e3": true, // safeMint(address, uint256, bytes)
		"0x4e6ec247": true, // _mint(address, uint256)
		"0x4cd4edcb": true, // _mint(address, uint256, bytes, bytes)
		"0x40c10f19": true, // mint(address, uint256)
		"0xcfa84fc1": true, // mint(uint256, address[], uint256[])
		"0x278d9c41": true, // mintEventToManyUsers(uint256, address[])
		"0x78b27221": true, // mintFungible(uint256, address[], uint256[])
		"0xf9419088": true, // mintNonFungible(uint256, address[])
		"0xf190ac5f": true, // mintToAddresses(address[], uint256)
		"0xa140ae23": true, // mintToken(uint256, address)
		"0xf980f3dc": true, // mintUserToManyEvents(uint256[], address)
		"0x14004ef3": true, // multimint(address[], uint256[])
		"0x6a627842": true, // mint(address)
		"0xa0712d68": true, // mint(uint256)
		"0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef": true, // Transfer(address from, address to, uint256 value)
		"0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925": true, // Approval(address owner, address spender, uint256 value)
		"0xd4735d920b0f87494915f556dd9b54c8f309026070caea5c737245152564d266": true, // Transfer(bytes32 node, address owner)
		"0x30385c845b448a36257a6a1716e6ad2e1bc2cbe333cde1e69fe849ad6511adfe": true, // Minted(address,uint256)
	}

	if len(needle) < 10 {
		return false
	}

	return tokenRelated[needle] || tokenRelated[needle[:10]]
}

func (s *SimpleTransaction) GasCost() base.Gas {
	if s.Receipt == nil {
		return 0
	}
	return s.GasPrice * s.Receipt.GasUsed
}

// EXISTING_CODE

