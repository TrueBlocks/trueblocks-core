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

// EXISTING_CODE

type RawSlurp struct {
	BlockHash         string `json:"blockHash"`
	BlockNumber       string `json:"blockNumber"`
	ContractAddress   string `json:"contractAddress"`
	CumulativeGasUsed string `json:"cumulativeGasUsed"`
	Ether             string `json:"ether"`
	From              string `json:"from"`
	FunctionName      string `json:"functionName"`
	Gas               string `json:"gas"`
	GasCost           string `json:"gasCost"`
	GasPrice          string `json:"gasPrice"`
	GasUsed           string `json:"gasUsed"`
	HasToken          string `json:"hasToken"`
	Hash              string `json:"hash"`
	Input             string `json:"input"`
	MethodId          string `json:"methodId"`
	Nonce             string `json:"nonce"`
	Timestamp         string `json:"timestamp"`
	To                string `json:"to"`
	TransactionIndex  string `json:"transactionIndex"`
	TxReceiptStatus   string `json:"txReceiptStatus"`
	Value             string `json:"value"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleSlurp struct {
	ArticulatedTx     *SimpleFunction `json:"articulatedTx"`
	BlockHash         base.Hash       `json:"blockHash"`
	BlockNumber       base.Blknum     `json:"blockNumber"`
	CompressedTx      string          `json:"compressedTx"`
	ContractAddress   base.Address    `json:"contractAddress"`
	CumulativeGasUsed string          `json:"cumulativeGasUsed"`
	Ether             string          `json:"ether"`
	From              base.Address    `json:"from"`
	FunctionName      string          `json:"functionName"`
	Gas               base.Gas        `json:"gas"`
	GasCost           base.Gas        `json:"gasCost"`
	GasPrice          base.Gas        `json:"gasPrice"`
	GasUsed           base.Gas        `json:"gasUsed"`
	HasToken          bool            `json:"hasToken"`
	Hash              base.Hash       `json:"hash"`
	Input             string          `json:"input"`
	IsError           bool            `json:"isError"`
	MethodId          string          `json:"methodId"`
	Nonce             uint64          `json:"nonce"`
	Timestamp         base.Timestamp  `json:"timestamp"`
	To                base.Address    `json:"to"`
	TransactionIndex  base.Blknum     `json:"transactionIndex"`
	TxReceiptStatus   string          `json:"txReceiptStatus"`
	Value             base.Wei        `json:"value"`
	raw               *RawSlurp       `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleSlurp) Raw() *RawSlurp {
	return s.raw
}

func (s *SimpleSlurp) SetRaw(raw *RawSlurp) {
	s.raw = raw
}

func (s *SimpleSlurp) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleSlurp) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

//- cacheable by addr_and_tx as group
type SimpleSlurpGroup struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Address          base.Address
	Slurps           []SimpleSlurp
}

func (s *SimpleSlurpGroup) CacheName() string {
	return "Slurp"
}

func (s *SimpleSlurpGroup) CacheId() string {
	return fmt.Sprintf("%s-%09d-%05d", s.Address.Hex()[2:], s.BlockNumber, s.TransactionIndex)
}

func (s *SimpleSlurpGroup) CacheLocation() (directory string, extension string) {
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

func (s *SimpleSlurpGroup) MarshalCache(writer io.Writer) (err error) {
	return cache.WriteValue(writer, s.Slurps)
}

func (s *SimpleSlurpGroup) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	return cache.ReadValue(reader, &s.Slurps, version)
}

func (s *SimpleSlurp) MarshalCache(writer io.Writer) (err error) {
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

	// CompressedTx
	if err = cache.WriteValue(writer, s.CompressedTx); err != nil {
		return err
	}

	// ContractAddress
	if err = cache.WriteValue(writer, s.ContractAddress); err != nil {
		return err
	}

	// CumulativeGasUsed
	if err = cache.WriteValue(writer, s.CumulativeGasUsed); err != nil {
		return err
	}

	// Ether
	if err = cache.WriteValue(writer, s.Ether); err != nil {
		return err
	}

	// From
	if err = cache.WriteValue(writer, s.From); err != nil {
		return err
	}

	// FunctionName
	if err = cache.WriteValue(writer, s.FunctionName); err != nil {
		return err
	}

	// Gas
	if err = cache.WriteValue(writer, s.Gas); err != nil {
		return err
	}

	// GasCost
	if err = cache.WriteValue(writer, s.GasCost); err != nil {
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

	// MethodId
	if err = cache.WriteValue(writer, s.MethodId); err != nil {
		return err
	}

	// Nonce
	if err = cache.WriteValue(writer, s.Nonce); err != nil {
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

	// TxReceiptStatus
	if err = cache.WriteValue(writer, s.TxReceiptStatus); err != nil {
		return err
	}

	// Value
	if err = cache.WriteValue(writer, &s.Value); err != nil {
		return err
	}

	return nil
}

func (s *SimpleSlurp) UnmarshalCache(version uint64, reader io.Reader) (err error) {
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

	// CompressedTx
	if err = cache.ReadValue(reader, &s.CompressedTx, version); err != nil {
		return err
	}

	// ContractAddress
	if err = cache.ReadValue(reader, &s.ContractAddress, version); err != nil {
		return err
	}

	// CumulativeGasUsed
	if err = cache.ReadValue(reader, &s.CumulativeGasUsed, version); err != nil {
		return err
	}

	// Ether
	if err = cache.ReadValue(reader, &s.Ether, version); err != nil {
		return err
	}

	// From
	if err = cache.ReadValue(reader, &s.From, version); err != nil {
		return err
	}

	// FunctionName
	if err = cache.ReadValue(reader, &s.FunctionName, version); err != nil {
		return err
	}

	// Gas
	if err = cache.ReadValue(reader, &s.Gas, version); err != nil {
		return err
	}

	// GasCost
	if err = cache.ReadValue(reader, &s.GasCost, version); err != nil {
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

	// MethodId
	if err = cache.ReadValue(reader, &s.MethodId, version); err != nil {
		return err
	}

	// Nonce
	if err = cache.ReadValue(reader, &s.Nonce, version); err != nil {
		return err
	}

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

	// TxReceiptStatus
	if err = cache.ReadValue(reader, &s.TxReceiptStatus, version); err != nil {
		return err
	}

	// Value
	if err = cache.ReadValue(reader, &s.Value, version); err != nil {
		return err
	}

	return nil
}

// EXISTING_CODE
// EXISTING_CODE
