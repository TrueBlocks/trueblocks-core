//go:build integration
// +build integration

package rpc

import (
	"math/big"
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
		wantState *types.SimpleState
		wantErr   bool
	}{
		{
			name: "balance only",
			args: args{
				chain:       chain,
				mode:        Balance,
				address:     base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
				blockNumber: uint64(15531843),
			},
			wantState: &types.SimpleState{
				Address:     base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
				BlockNumber: uint64(15531843),
				Balance:     func() big.Int { b, _ := big.NewInt(0).SetString("57006123709077586392", 10); return *b }(),
				Deployed:    utils.NOPOS,
			},
		},
		{
			name: "balance and nonce and code",
			args: args{
				chain:       chain,
				mode:        Balance | Nonce | Code,
				address:     base.HexToAddress("0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359"),
				blockNumber: uint64(15531843),
			},
			wantState: &types.SimpleState{
				Address:     base.HexToAddress("0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359"),
				BlockNumber: uint64(15531843),
				Balance: func() big.Int {
					b, _ := big.NewInt(0).SetString("32460000000000000021", 10)
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
				blockNumber: uint64(15531843),
			},
			wantState: &types.SimpleState{
				Address:     base.HexToAddress("0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359"),
				BlockNumber: uint64(15531843),
				Deployed:    988725,
			},
		},
		{
			name: "proxy and type",
			args: args{
				chain:       chain,
				mode:        Proxy | Type,
				address:     base.HexToAddress("0x4Fabb145d64652a948d72533023f6E7A623C7C53"),
				blockNumber: uint64(15531843),
			},
			wantState: &types.SimpleState{
				Address:     base.HexToAddress("0x4Fabb145d64652a948d72533023f6E7A623C7C53"),
				BlockNumber: uint64(15531843),
				Proxy:       base.HexToAddress("0x5864c777697bf9881220328bf2f16908c9afcd7e"),
				AccountType: "Proxy",
				Deployed:    utils.NOPOS,
			},
		},
		{
			name: "balance filter",
			args: args{
				chain:       chain,
				mode:        Balance,
				address:     base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
				blockNumber: uint64(15531843),
				filters: StateFilters{
					Balance: func(address base.Address, balance *big.Int) bool {
						return balance == big.NewInt(0)
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
				t.Errorf("GetState() = %+v, want %+v", gotState, tt.wantState)
			}
		})
	}
}
