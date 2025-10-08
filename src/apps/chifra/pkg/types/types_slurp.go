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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

// EXISTING_CODE

type Slurp struct {
	ArticulatedTx     *Function      `json:"articulatedTx"`
	BlockHash         base.Hash      `json:"blockHash"`
	BlockNumber       base.Blknum    `json:"blockNumber"`
	ContractAddress   base.Address   `json:"contractAddress"`
	CumulativeGasUsed string         `json:"cumulativeGasUsed"`
	From              base.Address   `json:"from"`
	FunctionName      string         `json:"functionName"`
	Gas               base.Gas       `json:"gas"`
	GasPrice          base.Gas       `json:"gasPrice"`
	GasUsed           base.Gas       `json:"gasUsed"`
	HasToken          bool           `json:"hasToken"`
	Hash              base.Hash      `json:"hash"`
	Input             string         `json:"input"`
	IsError           bool           `json:"-"`
	MethodId          string         `json:"methodId"`
	Nonce             base.Value     `json:"nonce"`
	Timestamp         base.Timestamp `json:"timestamp"`
	To                base.Address   `json:"to"`
	TransactionIndex  base.Txnum     `json:"transactionIndex"`
	TxReceiptStatus   string         `json:"txReceiptStatus"`
	ValidatorIndex    base.Value     `json:"validatorIndex"`
	Value             base.Wei       `json:"value"`
	WithdrawalIndex   base.Value     `json:"withdrawalIndex"`
	Calcs             *SlurpCalcs    `json:"calcs,omitempty"`
	// EXISTING_CODE
	Amount base.Wei `json:"amount"`
	// EXISTING_CODE
}

func (s Slurp) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Slurp) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{
		NewLabeler(s.ContractAddress, "contractAddress"),
		NewLabeler(s.From, "from"),
		NewLabeler(s.To, "to"),
	}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	switch s.From {
	case base.BlockRewardSender, base.UncleRewardSender:
		order = []string{
			"blockNumber",
			"timestamp",
			"date",
			"from",
			"to",
			"value",
		}

	case base.WithdrawalSender:
		order = []string{
			"blockNumber",
			"validatorIndex",
			"withdrawalIndex",
			"timestamp",
			"date",
			"from",
			"to",
			"value",
		}

	default:
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
			"input",
		}
	}

	isArticulated := extraOpts["articulate"] == true && s.ArticulatedTx != nil
	if isArticulated && format != "json" {
		order = append(order, "compressedTx")
	}

	asEther := true // like transactions, we always export ether for slurps -- extraOpts["ether"] == true
	if asEther {
		order = append(order, "ether")
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

// RawMap returns a map containing only the raw/base fields for this Slurp.
func (s *Slurp) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"blockNumber":      s.BlockNumber,
		"from":             s.From,
		"timestamp":        s.Timestamp,
		"to":               s.To,
		"value":            s.Value.String(),
		"transactionIndex": s.TransactionIndex,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	switch s.From {
	case base.BlockRewardSender, base.UncleRewardSender:
		model["from"] = s.From.Hex()

	case base.WithdrawalSender:
		model["from"] = s.From.Hex()
		model["withdrawalIndex"] = s.WithdrawalIndex
		model["validatorIndex"] = s.ValidatorIndex

	default:
		model["gas"] = s.Gas
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
	if s.BlockHash != base.HexToHash("0xdeadbeef") && !s.BlockHash.IsZero() {
		model["blockHash"] = s.BlockHash
	}
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *Slurp) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		"date": s.Date(),
		// EXISTING_CODE
	}

	// EXISTING_CODE
	switch s.From {
	case base.BlockRewardSender, base.UncleRewardSender:
		// No additional calculated fields for rewards

	case base.WithdrawalSender:
		// No additional calculated fields for withdrawals

	default:
		model["gasCost"] = s.GasCost()
	}

	// TODO: Turn this back on
	var articulatedTx map[string]any
	isArticulated := p.ExtraOpts["articulate"] == true && s.ArticulatedTx != nil

	// TODO: ARTICULATE SLURP
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
		a := s.ContractAddress.Hex()
		if strings.HasPrefix(a, "0x") && len(a) == 42 {
			model["contractAddress"] = a
		}
		// etherscan sometimes returns this word instead of a hex address
		if len(s.Input) > 2 && s.Input != "deprecated" {
			model["input"] = s.Input
		}

		// TODO: ARTICULATE SLURP
		if isArticulated {
			model["articulatedTx"] = articulatedTx
			// } else {
			// 	if s.Message != "" {
			// 		model["message"] = s.Message
			// 	}
		}

	} else {
		model["hasToken"] = s.HasToken
		model["isError"] = s.IsError
		// etherscan sometimes returns this word instead of a hex address
		input := s.Input
		if input == "deprecated" {
			input = "0x"
		}
		model["input"] = input

		// TODO: ARTICULATE SLURP
		model["compressedTx"] = ""
		enc := input
		if len(input) >= 10 {
			enc = input[:10]
		}
		model["encoding"] = enc

		if isArticulated {
			model["compressedTx"] = MakeCompressed(articulatedTx)
			// } else if s.Message != "" {
			// 	model["encoding"] = ""
			// 	model["compressedTx"] = s.Message
		}
	}

	asEther := true // like transactions, we always export ether for slurps -- extraOpts["ether"] == true
	if asEther {
		model["ether"] = s.Value.ToFloatString(18)
	}
	// EXISTING_CODE

	return model
}

func (s *Slurp) Date() string {
	return base.FormattedDate(s.Timestamp)
}

func (s *SlurpGroup) CacheLocations() (string, string, string) {
	paddedId := fmt.Sprintf("%s-%09d-%05d", s.Address.Hex()[2:], s.BlockNumber, s.TransactionIndex)
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]
	subFolder := strings.ToLower("Slurp") + "s"
	directory := filepath.Join(subFolder, filepath.Join(parts...))
	return directory, paddedId, "bin"
}

type SlurpGroup struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Address          base.Address
	Slurps           []Slurp
}

func (s *SlurpGroup) MarshalCache(writer io.Writer) (err error) {
	return base.WriteValue(writer, s.Slurps)
}

func (s *SlurpGroup) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	return base.ReadValue(reader, &s.Slurps, fileVersion)
}

func (s *Slurp) MarshalCache(writer io.Writer) (err error) {
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

	// ContractAddress
	if err = base.WriteValue(writer, s.ContractAddress); err != nil {
		return err
	}

	// CumulativeGasUsed
	if err = base.WriteValue(writer, s.CumulativeGasUsed); err != nil {
		return err
	}

	// From
	if err = base.WriteValue(writer, s.From); err != nil {
		return err
	}

	// FunctionName
	if err = base.WriteValue(writer, s.FunctionName); err != nil {
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

	// MethodId
	if err = base.WriteValue(writer, s.MethodId); err != nil {
		return err
	}

	// Nonce
	if err = base.WriteValue(writer, s.Nonce); err != nil {
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

	// TxReceiptStatus
	if err = base.WriteValue(writer, s.TxReceiptStatus); err != nil {
		return err
	}

	// ValidatorIndex
	if err = base.WriteValue(writer, s.ValidatorIndex); err != nil {
		return err
	}

	// Value
	if err = base.WriteValue(writer, &s.Value); err != nil {
		return err
	}

	// WithdrawalIndex
	if err = base.WriteValue(writer, s.WithdrawalIndex); err != nil {
		return err
	}

	return nil
}

func (s *Slurp) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
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

	// ContractAddress
	if err = base.ReadValue(reader, &s.ContractAddress, fileVersion); err != nil {
		return err
	}

	// CumulativeGasUsed
	if err = base.ReadValue(reader, &s.CumulativeGasUsed, fileVersion); err != nil {
		return err
	}

	// From
	if err = base.ReadValue(reader, &s.From, fileVersion); err != nil {
		return err
	}

	// FunctionName
	if err = base.ReadValue(reader, &s.FunctionName, fileVersion); err != nil {
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

	// MethodId
	if err = base.ReadValue(reader, &s.MethodId, fileVersion); err != nil {
		return err
	}

	// Nonce
	if err = base.ReadValue(reader, &s.Nonce, fileVersion); err != nil {
		return err
	}

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

	// TxReceiptStatus
	if err = base.ReadValue(reader, &s.TxReceiptStatus, fileVersion); err != nil {
		return err
	}

	// ValidatorIndex
	if err = base.ReadValue(reader, &s.ValidatorIndex, fileVersion); err != nil {
		return err
	}

	// Value
	if err = base.ReadValue(reader, &s.Value, fileVersion); err != nil {
		return err
	}

	// WithdrawalIndex
	if err = base.ReadValue(reader, &s.WithdrawalIndex, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Slurp) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// SlurpCalcs holds lazy-loaded calculated fields for Slurp
type SlurpCalcs struct {
	// EXISTING_CODE
	Date            string      `json:"date"`
	GasCost         base.Wei    `json:"gasCost,omitempty"`
	ContractAddress string      `json:"contractAddress,omitempty"`
	Input           string      `json:"input,omitempty"`
	ArticulatedTx   interface{} `json:"articulatedTx,omitempty"`
	HasToken        bool        `json:"hasToken,omitempty"`
	IsError         bool        `json:"isError,omitempty"`
	// EXISTING_CODE
}

func (s *Slurp) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
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

	s.Calcs = &SlurpCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
//

func (s *Slurp) GasCost() base.Gas {
	return s.GasPrice * s.GasUsed
}

// EXISTING_CODE
