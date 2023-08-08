package namesPkg

import (
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func Test_cleanCommon(t *testing.T) {
	type args struct {
		name *types.SimpleName
	}
	tests := []struct {
		name         string
		args         args
		wantModified bool
	}{
		{
			name: "Tag lower than 8",
			args: args{
				name: &types.SimpleName{
					Tags: "80",
				},
			},
			wantModified: false,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if gotModified := cleanCommon(tt.args.name); gotModified != tt.wantModified {
				t.Errorf("cleanCommon() = %v, want %v", gotModified, tt.wantModified)
			}
		})
	}
}

func Test_cleanNonContract(t *testing.T) {
	type args struct {
		name        *types.SimpleName
		wasContract bool
	}
	tests := []struct {
		name         string
		args         args
		wantModified bool
		expectedName *types.SimpleName
	}{
		{
			name: "Humanity DAO",
			args: args{
				name: &types.SimpleName{
					Tags: "30-Contracts:Humanity DAO",
				},
			},
			wantModified: true,
			expectedName: &types.SimpleName{
				Tags: "90-Individuals:Humanity DAO",
			},
		},
		{
			name: "was contract = true",
			args: args{
				name:        &types.SimpleName{},
				wasContract: true,
			},
			wantModified: true,
			expectedName: &types.SimpleName{
				Tags:       "37-SelfDestructed",
				IsContract: true,
			},
		},
		{
			name: "empty tags",
			args: args{
				name: &types.SimpleName{},
			},
			wantModified: true,
			expectedName: &types.SimpleName{
				Tags: "90-Individuals:Other",
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if gotModified := cleanNonContract(tt.args.name, tt.args.wasContract); gotModified != tt.wantModified {
				t.Errorf("cleanNonContract() = %v, want %v", gotModified, tt.wantModified)
			}
			if !reflect.DeepEqual(tt.args.name, tt.expectedName) {
				t.Errorf("cleanNonContract() = %v, want %v", tt.args.name, tt.expectedName)
			}
		})
	}
}

func Test_cleanToken(t *testing.T) {
	type args struct {
		token *types.SimpleToken
		name  *types.SimpleName
	}
	tests := []struct {
		name         string
		args         args
		wantModified bool
		expectedName *types.SimpleName
	}{
		{
			name: "ERC-20 token",
			args: args{
				name: &types.SimpleName{},
				token: &types.SimpleToken{
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.SimpleName{
				IsErc20: true,
				Source:  "On chain",
				Tags:    "50-Tokens:ERC20",
			},
		},
		{
			name: "NFT (ERC-721)",
			args: args{
				name: &types.SimpleName{},
				token: &types.SimpleToken{
					TokenType: types.TokenErc721,
				},
			},
			wantModified: true,
			expectedName: &types.SimpleName{
				IsErc721: true,
				Source:   "On chain",
				Tags:     "50-Tokens:ERC721",
			},
		},
		{
			name: "Fix airdrops tag",
			args: args{
				name: &types.SimpleName{
					Tags: "60-Airdrops",
				},
				token: &types.SimpleToken{
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.SimpleName{
				IsErc20: true,
				Source:  "On chain",
				Tags:    "50-Tokens:ERC20",
			},
		},
		{
			name: "Fix token tag",
			args: args{
				name: &types.SimpleName{
					Tags: "token",
				},
				token: &types.SimpleToken{
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.SimpleName{
				IsErc20: true,
				Source:  "On chain",
				Tags:    "50-Tokens:ERC20",
			},
		},
		{
			name: "Fix contracts tag",
			args: args{
				name: &types.SimpleName{
					Tags: "30-contracts",
				},
				token: &types.SimpleToken{
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.SimpleName{
				IsErc20: true,
				Source:  "On chain",
				Tags:    "50-Tokens:ERC20",
			},
		},
		{
			name: "Fix defi tag",
			args: args{
				name: &types.SimpleName{
					Tags: "55-defi",
				},
				token: &types.SimpleToken{
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.SimpleName{
				IsErc20: true,
				Source:  "On chain",
				Tags:    "50-Tokens:ERC20",
			},
		},
		{
			name: "Fill token name",
			args: args{
				name: &types.SimpleName{},
				token: &types.SimpleToken{
					Name:      "TrueBlocksCoin",
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.SimpleName{
				Name:    "TrueBlocksCoin",
				IsErc20: true,
				Source:  "On chain",
				Tags:    "50-Tokens:ERC20",
			},
		},
		{
			name: "Fill token symbol",
			args: args{
				name: &types.SimpleName{},
				token: &types.SimpleToken{
					Symbol:    "TBC",
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.SimpleName{
				Symbol:  "TBC",
				IsErc20: true,
				Source:  "On chain",
				Tags:    "50-Tokens:ERC20",
			},
		},
		{
			name: "Fill token decimals",
			args: args{
				name: &types.SimpleName{},
				token: &types.SimpleToken{
					Decimals:  18,
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.SimpleName{
				Decimals: 18,
				IsErc20:  true,
				Source:   "On chain",
				Tags:     "50-Tokens:ERC20",
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if gotModified := cleanToken(tt.args.name, tt.args.token); gotModified != tt.wantModified {
				t.Errorf("cleanToken() = %v, want %v", gotModified, tt.wantModified)
			}
			if !reflect.DeepEqual(tt.args.name, tt.expectedName) {
				t.Errorf("cleanToken() = %+v, want %+v", tt.args.name, tt.expectedName)
			}
		})
	}
}

func Test_cleanContract(t *testing.T) {
	type args struct {
		token   *types.SimpleToken
		address base.Address
		name    *types.SimpleName
	}
	tests := []struct {
		name         string
		args         args
		wantModified bool
		wantErr      bool
		expectedName *types.SimpleName
	}{
		{
			name: "Fix IsContract",
			args: args{
				name: &types.SimpleName{},
			},
			wantModified: true,
			wantErr:      false,
			expectedName: &types.SimpleName{
				IsContract: true,
				Tags:       "30-Contracts",
			},
		},
		{
			name: "Trim name and symbol",
			args: args{
				name: &types.SimpleName{
					Name:   "  Name with spaces  ",
					Symbol: "WEIRD   ",
				},
			},
			wantModified: true,
			wantErr:      false,
			expectedName: &types.SimpleName{
				Name:       "Name with spaces",
				Symbol:     "WEIRD",
				IsContract: true,
				Tags:       "30-Contracts",
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			gotModified, err := cleanContract(tt.args.token, tt.args.address, tt.args.name)
			if (err != nil) != tt.wantErr {
				t.Errorf("cleanContract() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if gotModified != tt.wantModified {
				t.Errorf("cleanContract() = %v, want %v", gotModified, tt.wantModified)
			}
			if !reflect.DeepEqual(tt.args.name, tt.expectedName) {
				t.Errorf("cleanContract() = %+v, want %+v", tt.args.name, tt.expectedName)
			}
		})
	}
}
