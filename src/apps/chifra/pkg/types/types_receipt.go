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
	"fmt"
	"io"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// EXISTING_CODE

type RawReceipt struct {
	BlockHash         string   `json:"blockHash"`
	BlockNumber       string   `json:"blockNumber"`
	ContractAddress   string   `json:"contractAddress"`
	CumulativeGasUsed string   `json:"cumulativeGasUsed"`
	EffectiveGasPrice string   `json:"effectiveGasPrice"`
	From              string   `json:"from"`
	GasUsed           string   `json:"gasUsed"`
	Logs              []RawLog `json:"logs"`
	LogsBloom         string   `json:"logsBloom"`
	Status            string   `json:"status"`
	To                string   `json:"to"`
	TransactionHash   string   `json:"transactionHash"`
	TransactionIndex  string   `json:"transactionIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleReceipt struct {
	BlockHash         base.Hash    `json:"blockHash,omitempty"`
	BlockNumber       base.Blknum  `json:"blockNumber"`
	ContractAddress   base.Address `json:"contractAddress,omitempty"`
	CumulativeGasUsed string       `json:"cumulativeGasUsed,omitempty"`
	EffectiveGasPrice base.Gas     `json:"effectiveGasPrice,omitempty"`
	From              base.Address `json:"from,omitempty"`
	GasUsed           base.Gas     `json:"gasUsed"`
	IsError           bool         `json:"isError,omitempty"`
	Logs              []SimpleLog  `json:"logs"`
	Status            uint32       `json:"status"`
	To                base.Address `json:"to,omitempty"`
	TransactionHash   base.Hash    `json:"transactionHash"`
	TransactionIndex  base.Blknum  `json:"transactionIndex"`
	raw               *RawReceipt  `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleReceipt) Raw() *RawReceipt {
	return s.raw
}

func (s *SimpleReceipt) SetRaw(raw *RawReceipt) {
	s.raw = raw
}

func (s *SimpleReceipt) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"blockNumber":      s.BlockNumber,
		"gasUsed":          s.GasUsed,
		"status":           s.Status,
		"transactionHash":  s.TransactionHash,
		"transactionIndex": s.TransactionIndex,
	}

	order = []string{
		"blockNumber",
		"transactionIndex",
		"transactionHash",
		"status",
		"gasUsed",
	}

	if format == "json" {
		if !s.ContractAddress.IsZero() {
			model["contractAddress"] = s.ContractAddress
		}

		if s.IsError {
			model["isError"] = s.IsError
			order = append(order, "isError")
		}

		if s.Logs == nil {
			model["logs"] = []SimpleLog{}
		} else {
			logs := make([]map[string]any, 0, len(s.Logs))
			for _, log := range s.Logs {
				logs = append(logs, log.Model(chain, format, verbose, extraOptions).Data)
			}
			model["logs"] = logs
		}

		if verbose {
			model["blockHash"] = s.BlockHash
			order = append(order, "blockHash")

			model["cumulativeGasUsed"] = s.CumulativeGasUsed
			order = append(order, "cumulativeGasUsed")

			if !s.From.IsZero() {
				model["from"] = s.From
			}

			if !s.To.IsZero() {
				model["to"] = s.To
			}
		}
	} else {
		model["logsCnt"] = len(s.Logs)
		order = append(order, "logsCnt")

		model["isError"] = s.IsError
		order = append(order, "isError")

		if verbose {
			model["contractAddress"] = s.ContractAddress.Hex()
			order = append(order, "contractAddress")
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// --> cacheable by block as group

type SimpleReceiptGroup struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Receipts         []SimpleReceipt
}

func (s *SimpleReceiptGroup) CacheName() string {
	return "Receipt"
}

func (s *SimpleReceiptGroup) CacheId() string {
	return fmt.Sprintf("%09d", s.BlockNumber)
}

func (s *SimpleReceiptGroup) CacheLocation() (directory string, extension string) {
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

func (s *SimpleReceiptGroup) MarshalCache(writer io.Writer) (err error) {
	return cache.WriteValue(writer, s.Receipts)
}

func (s *SimpleReceiptGroup) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	return cache.ReadValue(reader, &s.Receipts, version)
}

func (s *SimpleReceipt) MarshalCache(writer io.Writer) (err error) {
	// BlockHash
	if err = cache.WriteValue(writer, &s.BlockHash); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.WriteValue(writer, s.BlockNumber); err != nil {
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

	// EffectiveGasPrice
	if err = cache.WriteValue(writer, s.EffectiveGasPrice); err != nil {
		return err
	}

	// From
	if err = cache.WriteValue(writer, s.From); err != nil {
		return err
	}

	// GasUsed
	if err = cache.WriteValue(writer, s.GasUsed); err != nil {
		return err
	}

	// IsError
	if err = cache.WriteValue(writer, s.IsError); err != nil {
		return err
	}

	// Logs
	logs := make([]cache.Marshaler, 0, len(s.Logs))
	for _, log := range s.Logs {
		logs = append(logs, &log)
	}
	if err = cache.WriteValue(writer, logs); err != nil {
		return err
	}

	// Status
	if err = cache.WriteValue(writer, s.Status); err != nil {
		return err
	}

	// To
	if err = cache.WriteValue(writer, s.To); err != nil {
		return err
	}

	// TransactionHash
	if err = cache.WriteValue(writer, &s.TransactionHash); err != nil {
		return err
	}

	// TransactionIndex
	if err = cache.WriteValue(writer, s.TransactionIndex); err != nil {
		return err
	}

	return nil
}

func (s *SimpleReceipt) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	// BlockHash
	if err = cache.ReadValue(reader, &s.BlockHash, version); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.ReadValue(reader, &s.BlockNumber, version); err != nil {
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

	// EffectiveGasPrice
	if err = cache.ReadValue(reader, &s.EffectiveGasPrice, version); err != nil {
		return err
	}

	// From
	if err = cache.ReadValue(reader, &s.From, version); err != nil {
		return err
	}

	// GasUsed
	if err = cache.ReadValue(reader, &s.GasUsed, version); err != nil {
		return err
	}

	// IsError
	if err = cache.ReadValue(reader, &s.IsError, version); err != nil {
		return err
	}

	// Logs
	s.Logs = make([]SimpleLog, 0)
	if err = cache.ReadValue(reader, &s.Logs, version); err != nil {
		return err
	}

	// Status
	if err = cache.ReadValue(reader, &s.Status, version); err != nil {
		return err
	}

	// To
	if err = cache.ReadValue(reader, &s.To, version); err != nil {
		return err
	}

	// TransactionHash
	if err = cache.ReadValue(reader, &s.TransactionHash, version); err != nil {
		return err
	}

	// TransactionIndex
	if err = cache.ReadValue(reader, &s.TransactionIndex, version); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *SimpleReceipt) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

func (s *SimpleReceipt) IsDefault() bool {
	a := s.ContractAddress.IsZero()
	b := s.EffectiveGasPrice == 0
	c := s.GasUsed == 0
	d := len(s.Logs) == 0
	return a && b && c && d
}

func (r *RawReceipt) RawToSimple(vals map[string]any) (SimpleReceipt, error) {
	logs := []SimpleLog{}
	for _, rawLog := range r.Logs {
		simpleLog, _ := rawLog.RawToSimple(vals)
		logs = append(logs, simpleLog)
	}

	cumulativeGasUsed, err := hexutil.DecodeUint64(r.CumulativeGasUsed)
	if err != nil {
		return SimpleReceipt{}, err
	}

	receipt := SimpleReceipt{
		BlockHash:         base.HexToHash(r.BlockHash),
		BlockNumber:       utils.MustParseUint(r.BlockNumber),
		ContractAddress:   base.HexToAddress(r.ContractAddress),
		CumulativeGasUsed: fmt.Sprint(cumulativeGasUsed),
		EffectiveGasPrice: utils.MustParseUint(r.EffectiveGasPrice),
		GasUsed:           utils.MustParseUint(r.GasUsed),
		Status:            uint32(utils.MustParseUint(r.Status)),
		IsError:           utils.MustParseUint(r.Status) == 0,
		TransactionHash:   base.HexToHash(r.TransactionHash),
		TransactionIndex:  utils.MustParseUint(r.TransactionIndex),
		Logs:              logs,
		raw:               r,
	}

	return receipt, nil
}

// EXISTING_CODE
