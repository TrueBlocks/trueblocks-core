//go:build integration
// +build integration

package rpc

import (
	"fmt"
	"reflect"
	"strconv"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func TestGetState(t *testing.T) {
	chain := utils.GetTestChain()
	conn := TempConnection(chain)

	type args struct {
		chain       string
		mode        StatePart
		address     base.Address
		blockNumber base.Blknum
		filters     StateFilters
	}
	tests := []struct {
		name      string
		args      args
		wantState *types.State
		wantErr   bool
	}{
		{
			name: "balance only",
			args: args{
				chain:       chain,
				mode:        Balance,
				address:     base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
				blockNumber: 15531843,
			},
			wantState: &types.State{
				Address:     base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
				BlockNumber: 15531843,
				Balance:     func() base.Wei { b, _ := base.NewWei(0).SetString("57006123709077586392", 10); return *b }(),
				Deployed:    base.NOPOS,
			},
		},
		{
			name: "balance and nonce and code",
			args: args{
				chain:       chain,
				mode:        Balance | Nonce | Code,
				address:     base.HexToAddress("0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359"),
				blockNumber: 15531843,
			},
			wantState: &types.State{
				Address:     base.HexToAddress("0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359"),
				BlockNumber: 15531843,
				Balance: func() base.Wei {
					b, _ := base.NewWei(0).SetString("32460000000000000021", 10)
					return *b
				}(),
				Nonce: 0,
				Code: func() string {
					code, err := conn.GetContractCodeAt(base.HexToAddress("0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359"), uint64(15531843))
					if err != nil {
						t.Fatal("error when fetching code for smart contract:", err)
					}
					if len(code) == 0 {
						t.Fatal("got empty code for smart contract")
					}
					return "0x" + base.Bytes2Hex(code)
				}(),
				Deployed: func() uint64 {
					n, _ := strconv.ParseUint("18446744073709551615", 10, 64)
					return n
				}(),
			},
		},
		{
			name: "deployed only",
			args: args{
				chain:       chain,
				mode:        Deployed,
				address:     base.HexToAddress("0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359"),
				blockNumber: 15531843,
			},
			wantState: &types.State{
				Address:     base.HexToAddress("0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359"),
				BlockNumber: 15531843,
				Deployed:    988725,
			},
		},
		{
			name: "proxy and type",
			args: args{
				chain:       chain,
				mode:        Proxy | Type,
				address:     base.HexToAddress("0x4Fabb145d64652a948d72533023f6E7A623C7C53"),
				blockNumber: 15531843,
			},
			wantState: &types.State{
				Address:     base.HexToAddress("0x4Fabb145d64652a948d72533023f6E7A623C7C53"),
				BlockNumber: 15531843,
				Proxy:       base.HexToAddress("0x5864c777697bf9881220328bf2f16908c9afcd7e"),
				AccountType: "Proxy",
				Deployed:    base.NOPOS,
			},
		},
		{
			name: "balance filter",
			args: args{
				chain:       chain,
				mode:        Balance,
				address:     base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
				blockNumber: 15531843,
				filters: StateFilters{
					Balance: func(address base.Address, balance *base.Wei) bool {
						return balance == base.NewWei(0)
					},
				},
			},
			wantState: nil,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			gotState, err := conn.GetState(tt.args.mode, tt.args.address, tt.args.blockNumber, tt.args.filters)
			if (err != nil) != tt.wantErr {
				t.Errorf("GetState() error = %v, wantErr %v", err, tt.wantErr)
				return
			}

			// We need to shorten Code value, otherwise the output of a failed test is unreadable.
			// Comment the two `if`s below if you really want whole code
			if gotState != nil {
				if vl := len(gotState.Code); vl > 64 {
					gotState.Code = strings.Join([]string{gotState.Code[:8], gotState.Code[vl-9 : vl-1]}, "...")
				}
				if vl := len(tt.wantState.Code); vl > 64 {
					tt.wantState.Code = strings.Join([]string{tt.wantState.Code[:8], tt.wantState.Code[vl-9 : vl-1]}, "...")
				}
			}

			if !reflect.DeepEqual(gotState, tt.wantState) {
				msg := ""
				if gotState.AccountType != tt.wantState.AccountType {
					msg += fmt.Sprintf("AccountType: %s -- %s\n", gotState.AccountType, tt.wantState.AccountType)
				}
				if gotState.Address != tt.wantState.Address {
					msg += fmt.Sprintf("Address:     %s -- %s\n", gotState.Address, tt.wantState.Address)
				}
				if gotState.Balance.String() != tt.wantState.Balance.String() {
					msg += fmt.Sprintf("Balance:     %s -- %s\n", gotState.Balance.String(), tt.wantState.Balance.String())
				}
				if gotState.BlockNumber != tt.wantState.BlockNumber {
					msg += fmt.Sprintf("BlockNumber: %d -- %d\n", gotState.BlockNumber, tt.wantState.BlockNumber)
				}
				if gotState.Code != tt.wantState.Code {
					msg += fmt.Sprintf("Code:        %s -- %s\n", gotState.Code, tt.wantState.Code)
				}
				if gotState.Deployed != tt.wantState.Deployed {
					msg += fmt.Sprintf("Deployed:    %d -- %d\n", gotState.Deployed, tt.wantState.Deployed)
				}
				if gotState.Nonce != tt.wantState.Nonce {
					msg += fmt.Sprintf("Nonce:       %d -- %d\n", gotState.Nonce, tt.wantState.Nonce)
				}
				if gotState.Proxy != tt.wantState.Proxy {
					msg += fmt.Sprintf("Proxy:       %s -- %s\n", gotState.Proxy, tt.wantState.Proxy)
				}
				if gotState.Timestamp != tt.wantState.Timestamp {
					msg += fmt.Sprintf("Timestamp:   %d -- %d\n", gotState.Timestamp, tt.wantState.Timestamp)
				}
				t.Errorf(fmt.Sprintf("Test %s\n", tt.name), msg)
			}
		})
	}
}
