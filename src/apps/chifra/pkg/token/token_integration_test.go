package token

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

var tokenAddress = base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f")

// func TestGetState(t *testing.T) {
// 	abis := make(abi.AbiInterfaceMap)
// 	if err := abi.LoadAbi("mainnet", tokenAddress, abis); err != nil {
// 		t.Fatal("while reading ABI:", err)
// 	}

// 	type args struct {
// 		selector TokenStateSelector
// 		blockNum string
// 	}
// 	tests := []struct {
// 		name    string
// 		args    args
// 		want    string
// 		wantErr bool
// 	}{
// 		{
// 			name: "total supply",
// 			args: args{
// 				selector: TokenStateTotalSupply,
// 				blockNum: "0xd59f80", // 14000000
// 			},
// 			want: "9118918230822796234900723527",
// 		},
// 		{
// 			name: "decimals",
// 			args: args{
// 				selector: TokenStateDecimals,
// 				blockNum: "latest",
// 			},
// 			want: "18",
// 		},
// 		{
// 			name: "symbol",
// 			args: args{
// 				selector: TokenStateSymbol,
// 				blockNum: "latest",
// 			},
// 			want: "DAI",
// 		},
// 		{
// 			name: "name",
// 			args: args{
// 				selector: TokenStateName,
// 				blockNum: "latest",
// 			},
// 			want: "Dai Stablecoin",
// 		},
// 	}
// 	for _, tt := range tests {
// 		t.Run(tt.name, func(t *testing.T) {
// 			got, err := GetState("mainnet", tokenAddress, tt.args.selector, abis, tt.args.blockNum)
// 			if (err != nil) != tt.wantErr {
// 				t.Errorf("GetState() error = %v, wantErr %v", err, tt.wantErr)
// 				return
// 			}
// 			if got != tt.want {
// 				t.Errorf("GetState() = %v, want %v", got, tt.want)
// 			}
// 		})
// 	}
// }

// func TestGetState(t *testing.T) {
// 	type args struct {
// 		tokenAddr base.Address
// 		blockNum  string
// 	}
// 	tests := []struct {
// 		name      string
// 		args      args
// 		wantToken *Token
// 		wantErr   bool
// 	}{
// 		{
// 			name: "total supply",
// 			args: args{
// 				blockNum: "0xd59f80", // 14000000
// 			},
// 			want: "9118918230822796234900723527",
// 		},
// 		{
// 			name: "decimals",
// 			args: args{
// 				selector: TokenStateDecimals,
// 				blockNum: "latest",
// 			},
// 			want: "18",
// 		},
// 		{
// 			name: "symbol",
// 			args: args{
// 				selector: TokenStateSymbol,
// 				blockNum: "latest",
// 			},
// 			want: "DAI",
// 		},
// 		{
// 			name: "name",
// 			args: args{
// 				selector: TokenStateName,
// 				blockNum: "latest",
// 			},
// 			want: "Dai Stablecoin",
// 		},
// 	}
// 	for _, tt := range tests {
// 		t.Run(tt.name, func(t *testing.T) {
// 			gotToken, err := GetState("mainnet", tt.args.tokenAddr, tt.args.blockNum)
// 			if (err != nil) != tt.wantErr {
// 				t.Errorf("GetState() error = %v, wantErr %v", err, tt.wantErr)
// 				return
// 			}
// 			if !reflect.DeepEqual(gotToken, tt.wantToken) {
// 				t.Errorf("GetState() = %v, want %v", gotToken, tt.wantToken)
// 			}
// 		})
// 	}
// }

func TestGetState(t *testing.T) {
	abis := make(abi.AbiInterfaceMap)
	if err := abi.LoadAbi("mainnet", tokenAddress, abis); err != nil {
		t.Fatal("while reading ABI:", err)
	}

	blockNumber := "0xd59f80" // 14000000
	token, err := GetState("mainnet", tokenAddress, abis, blockNumber)
	if err != nil {
		t.Fatal(err)
	}

	if !token.IsErc20() {
		t.Fatal("token reported as non-ERC20")
	}

	// if !token.Erc721 {
	// 	t.Fatal("token reported as non-ERC721")
	// }

	if token.Name != "Dai Stablecoin" {
		t.Fatal("wrong name:", token.Name)
	}

	if token.Symbol != "DAI" {
		t.Fatal("wrong symbol:", token.Symbol)
	}

	if token.Decimals != 18 {
		t.Fatal("wrong decimals:", token.Decimals)
	}

	if token.TotalSupply != "9118918230822796234900723527" {
		t.Fatal("wrong total supply:", token.TotalSupply)
	}
}
