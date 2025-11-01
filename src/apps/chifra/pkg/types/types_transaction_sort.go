// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"

type TransactionField string

// Fields in the Transaction struct available for sorting.
const (
	TransactionBlockNumber      TransactionField = "blockNumber"
	TransactionDate             TransactionField = "date"
	TransactionFrom             TransactionField = "from"
	TransactionGas              TransactionField = "gas"
	TransactionGasPrice         TransactionField = "gasPrice"
	TransactionGasUsed          TransactionField = "gasUsed"
	TransactionHasToken         TransactionField = "hasToken"
	TransactionIsError          TransactionField = "isError"
	TransactionTimestamp        TransactionField = "timestamp"
	TransactionTo               TransactionField = "to"
	TransactionTransactionIndex TransactionField = "transactionIndex"
	TransactionType             TransactionField = "type"
	TransactionValue            TransactionField = "value"
)

// GetSortFieldsTransaction returns a []string of valid sort fields for the type.
func GetSortFieldsTransaction() []string {
	return []string{"blockNumber", "date", "from", "gas", "gasPrice", "gasUsed", "hasToken", "isError", "timestamp", "to", "transactionIndex", "type", "value"}
}

// TransactionBy returns a comparison function for sorting Transaction instances by the given field.
// These comparison functions may be strung together by the CmdTransactions function.
func TransactionBy(field TransactionField, order SortOrder) func(p1, p2 Transaction) bool {
	switch field {
	case TransactionBlockNumber: // blknum
		return func(p1, p2 Transaction) bool {
			if order == Ascending {
				return p1.BlockNumber < p2.BlockNumber
			}
			return p1.BlockNumber > p2.BlockNumber
		}
	case TransactionDate: // datetime
		return func(p1, p2 Transaction) bool {
			if order == Ascending {
				return p1.Date() < p2.Date()
			}
			return p1.Date() > p2.Date()
		}
	case TransactionFrom: // address
		return func(p1, p2 Transaction) bool {
			if order == Ascending {
				return p1.From.LessThan(p2.From)
			}
			return p2.From.LessThan(p1.From)
		}
	case TransactionGas: // gas
		return func(p1, p2 Transaction) bool {
			if order == Ascending {
				return p1.Gas < p2.Gas
			}
			return p1.Gas > p2.Gas
		}
	case TransactionGasPrice: // gas
		return func(p1, p2 Transaction) bool {
			if order == Ascending {
				return p1.GasPrice < p2.GasPrice
			}
			return p1.GasPrice > p2.GasPrice
		}
	case TransactionGasUsed: // gas
		return func(p1, p2 Transaction) bool {
			if order == Ascending {
				return p1.GasUsed < p2.GasUsed
			}
			return p1.GasUsed > p2.GasUsed
		}
	case TransactionHasToken: // bool
		return func(p1, p2 Transaction) bool {
			if order == Ascending {
				return !p1.HasToken && p2.HasToken
			}
			return p1.HasToken && !p2.HasToken
		}
	case TransactionIsError: // bool
		return func(p1, p2 Transaction) bool {
			if order == Ascending {
				return !p1.IsError && p2.IsError
			}
			return p1.IsError && !p2.IsError
		}
	case TransactionTimestamp: // timestamp
		return func(p1, p2 Transaction) bool {
			if order == Ascending {
				return p1.Timestamp < p2.Timestamp
			}
			return p1.Timestamp > p2.Timestamp
		}
	case TransactionTo: // address
		return func(p1, p2 Transaction) bool {
			if order == Ascending {
				return p1.To.LessThan(p2.To)
			}
			return p2.To.LessThan(p1.To)
		}
	case TransactionTransactionIndex: // txnum
		return func(p1, p2 Transaction) bool {
			if order == Ascending {
				return p1.TransactionIndex < p2.TransactionIndex
			}
			return p1.TransactionIndex > p2.TransactionIndex
		}
	case TransactionType: // string
		return func(p1, p2 Transaction) bool {
			if order == Ascending {
				return p1.TransactionType < p2.TransactionType
			}
			return p1.TransactionType > p2.TransactionType
		}
	case TransactionValue: // wei
		return func(p1, p2 Transaction) bool {
			if order == Ascending {
				return p1.Value.LessThan(&p2.Value)
			}
			return p2.Value.LessThan(&p1.Value)
		}

	}
	logger.ShouldNotHappen("in TransactionBy")
	return nil
}

// TransactionCmp accepts a slice and variadic comparison functions and returns a functions
// that can be used to sort the slice.
func TransactionCmp(slice []Transaction, orders ...func(p1, p2 Transaction) bool) func(i, j int) bool {
	return func(i, j int) bool {
		p1, p2 := slice[i], slice[j]
		for _, order := range orders {
			if order(p1, p2) {
				return true
			}
			if order(p2, p1) {
				return false
			}
		}
		return false
	}
}
