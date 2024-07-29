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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// EXISTING_CODE

type Receipt struct {
	BlockHash         base.Hash    `json:"blockHash,omitempty"`
	BlockNumber       base.Blknum  `json:"blockNumber"`
	ContractAddress   base.Address `json:"contractAddress,omitempty"`
	CumulativeGasUsed base.Gas     `json:"cumulativeGasUsed,omitempty"`
	EffectiveGasPrice base.Gas     `json:"effectiveGasPrice,omitempty"`
	From              base.Address `json:"from,omitempty"`
	GasUsed           base.Gas     `json:"gasUsed"`
	IsError           bool         `json:"isError,omitempty"`
	Logs              []Log        `json:"logs"`
	Status            base.Value   `json:"status"`
	To                base.Address `json:"to,omitempty"`
	TransactionHash   base.Hash    `json:"transactionHash"`
	TransactionIndex  base.Txnum   `json:"transactionIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Receipt) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Receipt) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
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
			model["logs"] = []Log{}
		} else {
			logs := make([]map[string]any, 0, len(s.Logs))
			for _, log := range s.Logs {
				logs = append(logs, log.Model(chain, format, verbose, extraOpts).Data)
			}
			model["logs"] = logs
		}

		if verbose {
			model["blockHash"] = s.BlockHash
			order = append(order, "blockHash")

			model["cumulativeGasUsed"] = s.CumulativeGasUsed
			order = append(order, "cumulativeGasUsed")
		}
		if !s.From.IsZero() {
			model["from"] = s.From
		}
		if !s.To.IsZero() {
			model["to"] = s.To
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

	items := []namer{
		{addr: s.ContractAddress, name: "contractName"},
		{addr: s.From, name: "fromName"},
		{addr: s.To, name: "toName"},
	}
	for _, item := range items {
		if name, loaded, found := nameAddress(extraOpts, item.addr); found {
			model[item.name] = name.Name
			order = append(order, item.name)
		} else if loaded && format != "json" {
			model[item.name] = ""
			order = append(order, item.name)
		}
	}
	order = reorderOrdering(order)
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *ReceiptGroup) CacheName() string {
	return "Receipt"
}

func (s *ReceiptGroup) CacheId() string {
	return fmt.Sprintf("%09d", s.BlockNumber)
}

func (s *ReceiptGroup) CacheLocation() (directory string, extension string) {
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

type ReceiptGroup struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Receipts         []Receipt
}

func (s *ReceiptGroup) MarshalCache(writer io.Writer) (err error) {
	return cache.WriteValue(writer, s.Receipts)
}

func (s *ReceiptGroup) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	return cache.ReadValue(reader, &s.Receipts, vers)
}

func (s *Receipt) MarshalCache(writer io.Writer) (err error) {
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

func (s *Receipt) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// BlockHash
	if err = cache.ReadValue(reader, &s.BlockHash, vers); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.ReadValue(reader, &s.BlockNumber, vers); err != nil {
		return err
	}

	// ContractAddress
	if err = cache.ReadValue(reader, &s.ContractAddress, vers); err != nil {
		return err
	}

	// CumulativeGasUsed
	vCumulativeGasUsed := version.NewVersion("2.5.8")
	if vers <= vCumulativeGasUsed.Uint64() {
		var val string
		if err = cache.ReadValue(reader, &val, vers); err != nil {
			return err
		}
		s.CumulativeGasUsed = base.MustParseGas(val)
	} else {
		// CumulativeGasUsed
		if err = cache.ReadValue(reader, &s.CumulativeGasUsed, vers); err != nil {
			return err
		}
	}

	// EffectiveGasPrice
	if err = cache.ReadValue(reader, &s.EffectiveGasPrice, vers); err != nil {
		return err
	}

	// From
	if err = cache.ReadValue(reader, &s.From, vers); err != nil {
		return err
	}

	// GasUsed
	if err = cache.ReadValue(reader, &s.GasUsed, vers); err != nil {
		return err
	}

	// IsError
	if err = cache.ReadValue(reader, &s.IsError, vers); err != nil {
		return err
	}

	// Logs
	s.Logs = make([]Log, 0)
	if err = cache.ReadValue(reader, &s.Logs, vers); err != nil {
		return err
	}

	// Status
	vStatus := version.NewVersion("2.5.9")
	if vers <= vStatus.Uint64() {
		var val uint32
		if err = cache.ReadValue(reader, &val, vers); err != nil {
			return err
		}
		s.Status = base.Value(val)
	} else {
		// Status
		if err = cache.ReadValue(reader, &s.Status, vers); err != nil {
			return err
		}
	}

	// To
	if err = cache.ReadValue(reader, &s.To, vers); err != nil {
		return err
	}

	// TransactionHash
	if err = cache.ReadValue(reader, &s.TransactionHash, vers); err != nil {
		return err
	}

	// TransactionIndex
	if err = cache.ReadValue(reader, &s.TransactionIndex, vers); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Receipt) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

func (s *Receipt) IsDefault() bool {
	a := s.ContractAddress.IsZero()
	b := s.EffectiveGasPrice == 0
	c := s.GasUsed == 0
	d := len(s.Logs) == 0
	return a && b && c && d
}

// EXISTING_CODE
