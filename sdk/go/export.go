// Copyright 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	// EXISTING_CODE
	"io"
	"net/url"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	export "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ExportOptions struct {
	Addrs       []string // allow for ENS names and addresses
	Topics      []string // topics are strings
	Fourbytes   []string
	Appearances bool
	Receipts    bool
	Logs        bool
	Traces      bool
	Neighbors   bool
	Accounting  bool
	Statements  bool
	Balances    bool
	Withdrawals bool
	Articulate  bool
	CacheTraces bool
	Count       bool
	FirstRecord uint64
	MaxRecords  uint64
	Relevant    bool
	Emitter     []string // allow for ENS names and addresses
	Topic       []string // topics are strings
	Reverted    bool
	Asset       []string // allow for ENS names and addresses
	Flow        ExportFlow
	Factory     bool
	Unripe      bool
	Load        string
	Reversed    bool
	NoZero      bool
	FirstBlock  base.Blknum
	LastBlock   base.Blknum
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Export implements the chifra export command for the SDK.
func (opts *ExportOptions) Export(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	//   addrs - one or more addresses (0x...) to export (required)
	//   topics - filter by one or more log topics (only for --logs option)
	//   fourbytes - filter by one or more fourbytes (only for transactions and trace options)
	//   -p, --appearances         export a list of appearances
	//   -r, --receipts            export receipts instead of transactional data
	//   -l, --logs                export logs instead of transactional data
	//   -t, --traces              export traces instead of transactional data
	//   -n, --neighbors           export the neighbors of the given address
	//   -C, --accounting          attach accounting records to the exported data (applies to transactions export only)
	//   -A, --statements          for the accounting options only, export only statements
	//   -b, --balances            traverse the transaction history and show each change in ETH balances
	//   -i, --withdrawals         export withdrawals for the given address
	//   -a, --articulate          articulate transactions, traces, logs, and outputs
	//   -R, --cache_traces        force the transaction's traces into the cache
	//   -U, --count               for --appearances mode only, display only the count of records
	//   -c, --first_record uint   the first record to process
	//   -e, --max_records uint    the maximum number of records to process (default 250)
	//   -N, --relevant            for log and accounting export only, export only logs relevant to one of the given export addresses
	//   -m, --emitter strings     for the --logs option only, filter logs to show only those logs emitted by the given address(es)
	//   -B, --topic strings       for the --logs option only, filter logs to show only those with this topic(s)
	//   -V, --reverted            export only transactions that were reverted
	//   -P, --asset strings       for the accounting options only, export statements only for this asset
	//   -f, --flow string         for the accounting options only, export statements with incoming, outgoing, or zero value
	//                             One of [ in | out | zero ]
	//   -y, --factory             for --traces only, report addresses created by (or self-destructed by) the given address(es)
	//   -u, --unripe              export transactions labeled upripe (i.e. less than 28 blocks old)
	//   -E, --reversed            produce results in reverse chronological order
	//   -z, --no_zero             for the --count option only, suppress the display of zero appearance accounts
	//   -F, --first_block uint    first block to process (inclusive)
	//   -L, --last_block uint     last block to process (inclusive)
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return export.Export(w, values)
}

type ExportFlow int

const (
	NoEF ExportFlow = iota
	EFIn
	EFOut
	EFZero
)

func (v ExportFlow) String() string {
	return []string{
		"noef",
		"in",
		"out",
		"zero",
	}[v]
}

// EXISTING_CODE
// EXISTING_CODE

