package namesPkg

import (
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func Test_cleanCommon(t *testing.T) {
	type args struct {
		name *types.Name
	}
	tests := []struct {
		name         string
		args         args
		wantModified bool
	}{
		{
			name: "Tags lower than 8",
			args: args{
				name: &types.Name{
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
		name *types.Name
		// wasContract bool
	}
	tests := []struct {
		name         string
		args         args
		wantModified bool
		expectedName *types.Name
	}{
		{
			name: "Humanity DAO",
			args: args{
				name: &types.Name{
					Tags: "30-Contracts:Humanity DAO",
				},
			},
			wantModified: true,
			expectedName: &types.Name{
				Tags: "90-Individuals:Humanity DAO",
			},
		},
		{
			name: "empty tags",
			args: args{
				name: &types.Name{},
			},
			wantModified: true,
			expectedName: &types.Name{
				Tags: "90-Individuals:Other",
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if gotModified := cleanNonContract(tt.args.name); gotModified != tt.wantModified {
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
		token *types.Token
		name  *types.Name
	}
	tests := []struct {
		name         string
		args         args
		wantModified bool
		expectedName *types.Name
	}{
		{
			name: "ERC-20 token",
			args: args{
				name: &types.Name{},
				token: &types.Token{
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.Name{
				IsErc20: true,
				Source:  "On chain",
				Tags:    "50-Tokens:ERC20",
			},
		},
		{
			name: "NFT (ERC-721)",
			args: args{
				name: &types.Name{},
				token: &types.Token{
					TokenType: types.TokenErc721,
				},
			},
			wantModified: true,
			expectedName: &types.Name{
				IsErc721: true,
				Source:   "On chain",
				Tags:     "50-Tokens:ERC721",
			},
		},
		{
			name: "Fix airdrops tags",
			args: args{
				name: &types.Name{
					Tags: "60-Airdrops",
				},
				token: &types.Token{
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.Name{
				IsErc20: true,
				Source:  "On chain",
				Tags:    "50-Tokens:ERC20",
			},
		},
		{
			name: "Fix token tags",
			args: args{
				name: &types.Name{
					Tags: "token",
				},
				token: &types.Token{
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.Name{
				IsErc20: true,
				Source:  "On chain",
				Tags:    "50-Tokens:ERC20",
			},
		},
		{
			name: "Fix contracts tags",
			args: args{
				name: &types.Name{
					Tags: "30-contracts",
				},
				token: &types.Token{
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.Name{
				IsErc20: true,
				Source:  "On chain",
				Tags:    "50-Tokens:ERC20",
			},
		},
		{
			name: "Fix defi tags",
			args: args{
				name: &types.Name{
					Tags: "55-defi",
				},
				token: &types.Token{
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.Name{
				IsErc20: true,
				Source:  "On chain",
				Tags:    "50-Tokens:ERC20",
			},
		},
		{
			name: "Fill token name",
			args: args{
				name: &types.Name{},
				token: &types.Token{
					Name:      "TrueBlocksCoin",
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.Name{
				Name:    "TrueBlocksCoin",
				IsErc20: true,
				Source:  "On chain",
				Tags:    "50-Tokens:ERC20",
			},
		},
		{
			name: "Fill token symbol",
			args: args{
				name: &types.Name{},
				token: &types.Token{
					Symbol:    "TBC",
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.Name{
				Symbol:  "TBC",
				IsErc20: true,
				Source:  "On chain",
				Tags:    "50-Tokens:ERC20",
			},
		},
		{
			name: "Fill token decimals",
			args: args{
				name: &types.Name{},
				token: &types.Token{
					Decimals:  18,
					TokenType: types.TokenErc20,
				},
			},
			wantModified: true,
			expectedName: &types.Name{
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
		token *types.Token
		name  *types.Name
	}
	tests := []struct {
		name         string
		args         args
		wantModified bool
		wantErr      bool
		expectedName *types.Name
	}{
		{
			name: "Fix IsContract",
			args: args{
				name: &types.Name{},
			},
			wantModified: true,
			wantErr:      false,
			expectedName: &types.Name{
				IsContract: true,
				Tags:       "30-Contracts",
			},
		},
		{
			name: "Trim name and symbol",
			args: args{
				name: &types.Name{
					Name:   "  Name with spaces  ",
					Symbol: "WEIRD   ",
				},
			},
			wantModified: true,
			wantErr:      false,
			expectedName: &types.Name{
				Name:       "Name with spaces",
				Symbol:     "WEIRD",
				IsContract: true,
				Tags:       "30-Contracts",
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			gotModified, err := cleanContract(tt.args.token, tt.args.name)
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
