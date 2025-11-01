// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"encoding/json"
	"fmt"
	"io"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/cache"
)

type StorageSlot struct {
	Address     base.Address `json:"address"`
	StorageKeys []base.Hash  `json:"storageKeys"`
}

type Rewards struct {
	Block  base.Wei `json:"block"`
	Nephew base.Wei `json:"nephew"`
	TxFee  base.Wei `json:"txFee"`
	Uncle  base.Wei `json:"uncle"`
}

func NewReward(block, nephew, txFee, uncle *base.Wei) (Rewards, base.Wei) {
	total := new(base.Wei).Add(block, nephew)
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

type Transaction struct {
	ArticulatedTx        *Function         `json:"articulatedTx"`
	BlockHash            base.Hash         `json:"blockHash"`
	BlockNumber          base.Blknum       `json:"blockNumber"`
	From                 base.Address      `json:"from"`
	Gas                  base.Gas          `json:"gas"`
	GasPrice             base.Gas          `json:"gasPrice"`
	GasUsed              base.Gas          `json:"gasUsed"`
	HasToken             bool              `json:"hasToken"`
	Hash                 base.Hash         `json:"hash"`
	Input                string            `json:"input"`
	IsError              bool              `json:"isError"`
	MaxFeePerGas         base.Gas          `json:"maxFeePerGas"`
	MaxPriorityFeePerGas base.Gas          `json:"maxPriorityFeePerGas"`
	Nonce                base.Value        `json:"nonce"`
	Receipt              *Receipt          `json:"receipt"`
	Timestamp            base.Timestamp    `json:"timestamp"`
	To                   base.Address      `json:"to"`
	Traces               []Trace           `json:"traces"`
	TransactionIndex     base.Txnum        `json:"transactionIndex"`
	TransactionType      string            `json:"type"`
	Value                base.Wei          `json:"value"`
	Calcs                *TransactionCalcs `json:"calcs,omitempty"`
	// EXISTING_CODE
	Message    string       `json:"-"`
	Rewards    *Rewards     `json:"-"`
	Statements *[]Statement `json:"statements"`
	// EXISTING_CODE
}

func (s Transaction) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Transaction) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{
		NewLabeler(s.From, "from"),
		NewLabeler(s.To, "to"),
	}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"blockNumber",
		"transactionIndex",
		"timestamp",
		"date",
		"from",
		"to",
		"value",
		"gasPrice",
		"gasUsed",
		"gasCost",
		"hash",
		"isError",
		"encoding",
	}

	if extraOpts["articulate"] == true && s.ArticulatedTx != nil && format != "json" {
		order = append(order, "compressedTx")
	}

	if format != "json" {
		order = append(order, "type")
		if extraOpts["traces"] == true {
			order = append(order, "nTraces")
		}
	}

	asEther := true // special case for transactions, we always show --ether -- extraOpts["ether"] == true
	if asEther {
		order = append(order, "ether")
	}

	// Add receipt log addresses to rawNames for labeling
	if s.Receipt != nil && len(s.Receipt.Logs) > 0 {
		for i, log := range s.Receipt.Logs {
			logName := fmt.Sprintf("log%dAddress", i)
			rawNames = append(rawNames, NewLabeler(log.Address, logName))
		}
	}
	// EXISTING_CODE

	for _, item := range rawNames {
		key := item.name + "Name"
		if _, exists := model[key]; exists {
			order = append(order, key)
		}
	}
	order = reorderFields(order)

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this Transaction.
func (s *Transaction) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"blockNumber":      s.BlockNumber,
		"from":             s.From,
		"gasPrice":         s.GasPrice,
		"gasUsed":          s.GasUsed,
		"hash":             s.Hash,
		"timestamp":        s.Timestamp,
		"transactionIndex": s.TransactionIndex,
		"value":            s.Value.String(),
		// EXISTING_CODE
	}

	// EXISTING_CODE
	to := s.To.Hex()
	if to == "0x0000000000000000000000000000000000000000" {
		to = "0x0" // weird special case to preserve what RPC does
	}
	model["to"] = to

	if p.Format == "json" {
		model["blockHash"] = s.BlockHash
		if s.Nonce > 0 {
			model["nonce"] = s.Nonce
		}
		model["gas"] = s.Gas
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
	} else {
		if s.TransactionType != "0x0" {
			model["type"] = s.TransactionType
		} else {
			model["type"] = ""
		}
		model["isError"] = s.IsError
	}
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *Transaction) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		"date":    s.Date(),
		"gasCost": s.GasCost(),
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// TODO: Shouldn't this use the Function model - the answer is yes?
	var articulatedTx map[string]any
	isArticulated := p.ExtraOpts["articulate"] == true && s.ArticulatedTx != nil
	if isArticulated {
		articulatedTx = map[string]any{
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

	if p.Format == "json" {
		if s.Statements != nil {
			statements := make([]map[string]any, 0, len(*s.Statements))
			for _, statement := range *s.Statements {
				statements = append(statements, statement.Model(p.Chain, p.Format, p.Verbose, p.ExtraOpts).Data)
			}
			model["statements"] = statements
		}

		if s.Receipt != nil && !s.Receipt.IsDefault() {
			contractAddress := s.Receipt.ContractAddress.Hex()

			// TODO: This is quite odd. Why?
			status := &s.Receipt.Status
			if s.BlockNumber < base.KnownBlock(p.Chain, base.Byzantium) || *status == 4294967295-1 {
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
				if p.ExtraOpts["articulate"] == true && log.ArticulatedLog != nil {
					inputModels := parametersToMap(log.ArticulatedLog.Inputs)
					articulatedLog := map[string]any{
						"name":   log.ArticulatedLog.Name,
						"inputs": inputModels,
					}
					logModel["articulatedLog"] = articulatedLog
				}
				if name, loaded, found := labelAddress(p.ExtraOpts, log.Address); found {
					logModel["addressName"] = name.Name
				} else if loaded && p.Format != "json" {
					logModel["addressName"] = ""
				}
				logs = append(logs, logModel)
			}
			receiptModel["logs"] = logs
			model["receipt"] = receiptModel
		} else {
			model["receipt"] = map[string]any{}
		}

		if p.ExtraOpts["traces"] == true && len(s.Traces) > 0 {
			traceModels := make([]map[string]any, 0, len(s.Traces))
			for _, trace := range s.Traces {
				traceModels = append(traceModels, trace.Model(p.Chain, p.Format, p.Verbose, p.ExtraOpts).Data)
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
		ethGasPrice := base.NewWei(0).SetUint64(uint64(s.GasPrice)).ToFloatString(18)
		model["ethGasPrice"] = ethGasPrice

		if p.ExtraOpts["articulate"] == true && s.ArticulatedTx != nil {
			model["encoding"] = s.ArticulatedTx.Encoding
		}

		model["compressedTx"] = ""
		enc := s.Input
		if len(s.Input) >= 10 {
			enc = s.Input[:10]
		}
		model["encoding"] = enc

		if isArticulated {
			model["compressedTx"] = MakeCompressed(articulatedTx)
		} else if s.Message != "" {
			model["encoding"] = ""
			model["compressedTx"] = s.Message
		}

		if p.ExtraOpts["traces"] == true {
			model["nTraces"] = len(s.Traces)
		}
	}

	asEther := true // special case for transactions, we always show --ether -- p.ExtraOpts["ether"] == true
	if asEther {
		model["ether"] = s.Value.ToFloatString(18)
	}
	// EXISTING_CODE

	return model
}

func (s *Transaction) Date() string {
	return base.FormattedDate(s.Timestamp)
}

func (s *Transaction) CacheLocations() (string, string, string) {
	paddedId := fmt.Sprintf("%09d-%05d", s.BlockNumber, s.TransactionIndex)
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]
	subFolder := strings.ToLower("Transaction") + "s"
	directory := filepath.Join(subFolder, filepath.Join(parts...))
	return directory, paddedId, "bin"
}

func (s *Transaction) MarshalCache(writer io.Writer) (err error) {
	// ArticulatedTx
	optArticulatedTx := &cache.Optional[Function]{
		Value: s.ArticulatedTx,
	}
	if err = base.WriteValue(writer, optArticulatedTx); err != nil {
		return err
	}

	// BlockHash
	if err = base.WriteValue(writer, &s.BlockHash); err != nil {
		return err
	}

	// BlockNumber
	if err = base.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// From
	if err = base.WriteValue(writer, s.From); err != nil {
		return err
	}

	// Gas
	if err = base.WriteValue(writer, s.Gas); err != nil {
		return err
	}

	// GasPrice
	if err = base.WriteValue(writer, s.GasPrice); err != nil {
		return err
	}

	// GasUsed
	if err = base.WriteValue(writer, s.GasUsed); err != nil {
		return err
	}

	// HasToken
	if err = base.WriteValue(writer, s.HasToken); err != nil {
		return err
	}

	// Hash
	if err = base.WriteValue(writer, &s.Hash); err != nil {
		return err
	}

	// Input
	if err = base.WriteValue(writer, s.Input); err != nil {
		return err
	}

	// IsError
	if err = base.WriteValue(writer, s.IsError); err != nil {
		return err
	}

	// MaxFeePerGas
	if err = base.WriteValue(writer, s.MaxFeePerGas); err != nil {
		return err
	}

	// MaxPriorityFeePerGas
	if err = base.WriteValue(writer, s.MaxPriorityFeePerGas); err != nil {
		return err
	}

	// Nonce
	if err = base.WriteValue(writer, s.Nonce); err != nil {
		return err
	}

	// Receipt
	optReceipt := &cache.Optional[Receipt]{
		Value: s.Receipt,
	}
	if err = base.WriteValue(writer, optReceipt); err != nil {
		return err
	}

	// Timestamp
	if err = base.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	// To
	if err = base.WriteValue(writer, s.To); err != nil {
		return err
	}

	// TransactionIndex
	if err = base.WriteValue(writer, s.TransactionIndex); err != nil {
		return err
	}

	// TransactionType
	if err = base.WriteValue(writer, s.TransactionType); err != nil {
		return err
	}

	// Value
	if err = base.WriteValue(writer, &s.Value); err != nil {
		return err
	}

	return nil
}

func (s *Transaction) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// ArticulatedTx
	optArticulatedTx := &cache.Optional[Function]{
		Value: s.ArticulatedTx,
	}
	if err = base.ReadValue(reader, optArticulatedTx, fileVersion); err != nil {
		return err
	}
	s.ArticulatedTx = optArticulatedTx.Get()

	// BlockHash
	if err = base.ReadValue(reader, &s.BlockHash, fileVersion); err != nil {
		return err
	}

	// BlockNumber
	if err = base.ReadValue(reader, &s.BlockNumber, fileVersion); err != nil {
		return err
	}

	// From
	if err = base.ReadValue(reader, &s.From, fileVersion); err != nil {
		return err
	}

	// Gas
	if err = base.ReadValue(reader, &s.Gas, fileVersion); err != nil {
		return err
	}

	// GasPrice
	if err = base.ReadValue(reader, &s.GasPrice, fileVersion); err != nil {
		return err
	}

	// GasUsed
	if err = base.ReadValue(reader, &s.GasUsed, fileVersion); err != nil {
		return err
	}

	// HasToken
	if err = base.ReadValue(reader, &s.HasToken, fileVersion); err != nil {
		return err
	}

	// Hash
	if err = base.ReadValue(reader, &s.Hash, fileVersion); err != nil {
		return err
	}

	// Input
	if err = base.ReadValue(reader, &s.Input, fileVersion); err != nil {
		return err
	}

	// IsError
	if err = base.ReadValue(reader, &s.IsError, fileVersion); err != nil {
		return err
	}

	// MaxFeePerGas
	if err = base.ReadValue(reader, &s.MaxFeePerGas, fileVersion); err != nil {
		return err
	}

	// MaxPriorityFeePerGas
	if err = base.ReadValue(reader, &s.MaxPriorityFeePerGas, fileVersion); err != nil {
		return err
	}

	// Nonce
	if err = base.ReadValue(reader, &s.Nonce, fileVersion); err != nil {
		return err
	}

	// Receipt
	optReceipt := &cache.Optional[Receipt]{
		Value: s.Receipt,
	}
	if err = base.ReadValue(reader, optReceipt, fileVersion); err != nil {
		return err
	}
	s.Receipt = optReceipt.Get()

	// Timestamp
	if err = base.ReadValue(reader, &s.Timestamp, fileVersion); err != nil {
		return err
	}

	// To
	if err = base.ReadValue(reader, &s.To, fileVersion); err != nil {
		return err
	}

	// TransactionIndex
	if err = base.ReadValue(reader, &s.TransactionIndex, fileVersion); err != nil {
		return err
	}

	// TransactionType
	if err = base.ReadValue(reader, &s.TransactionType, fileVersion); err != nil {
		return err
	}

	// Value
	if err = base.ReadValue(reader, &s.Value, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Transaction) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// TransactionCalcs holds lazy-loaded calculated fields for Transaction
type TransactionCalcs struct {
	// EXISTING_CODE
	Date          string                 `json:"date"`
	GasCost       base.Wei               `json:"gasCost"`
	ArticulatedTx map[string]interface{} `json:"articulatedTx,omitempty"`
	Statements    []map[string]any       `json:"statements,omitempty"`
	Receipt       map[string]interface{} `json:"receipt,omitempty"`
	Traces        []map[string]any       `json:"traces,omitempty"`
	Message       string                 `json:"message,omitempty"`
	EthGasPrice   string                 `json:"ethGasPrice,omitempty"`
	Encoding      string                 `json:"encoding,omitempty"`
	CompressedTx  string                 `json:"compressedTx,omitempty"`
	NTraces       int                    `json:"nTraces,omitempty"`
	Ether         string                 `json:"ether"`
	// EXISTING_CODE
}

func (s *Transaction) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
	_ = fieldFilter // delint
	if s.Calcs != nil {
		return nil
	}

	calcMap := s.CalcMap(p)
	if len(calcMap) == 0 {
		return nil
	}

	jsonBytes, err := json.Marshal(calcMap)
	if err != nil {
		return err
	}

	s.Calcs = &TransactionCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
//

func IsTokenFunction(needle string) bool {
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

func (s *Transaction) GasCost() base.Gas {
	if s.Receipt == nil {
		return 0
	}
	return s.GasPrice * s.Receipt.GasUsed
}

// EXISTING_CODE
