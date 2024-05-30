//go:build integration
// +build integration

// Copyright 2023 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package namesPkg

import (
	"context"
	"os"
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func init() {
	// We need this so the tests don't fail when the provider is not available on github
	// actions. Note that these settings on get set when running unit tests.
	os.Setenv("TB_NO_PROVIDER_CHECK", "true")
}

func Test_cleanName(t *testing.T) {
	type args struct {
		chain string
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
			name: "was contract",
			args: args{
				chain: "mainnet",
				name: &types.Name{
					IsContract: true,
					Address:    base.HexToAddress("0x139e20773accc72ef75765fdff158244845c3888"),
				},
			},
			wantModified: true,
			expectedName: &types.Name{
				Address:    base.HexToAddress("0x139e20773accc72ef75765fdff158244845c3888"),
				Petname:    "remarkably-solid-cardinal",
				IsContract: true,
				Tags:       "37-SelfDestructed",
			},
		},
		{
			name: "was NOT contract",
			args: args{
				chain: "mainnet",
				name: &types.Name{
					IsContract: false,
					Address:    base.HexToAddress("0x139e20773accc72ef75765fdff158244845c3888"),
				},
			},
			wantModified: true,
			expectedName: &types.Name{
				Address: base.HexToAddress("0x139e20773accc72ef75765fdff158244845c3888"),
				Petname: "remarkably-solid-cardinal",
				Tags:    "90-Individuals:Other",
			},
		},
		{
			name: "not modified",
			args: args{
				chain: "mainnet",
				name: &types.Name{
					Address:    base.HexToAddress("0xe77d387b4be1076891868060c32e81bc3b89c730"),
					Decimals:   18,
					IsContract: false,
					Name:       "0xe77d387b4be1076891868060c32e81bc3b89c730",
					Petname:    "overly-legal-lynx",
					Source:     "TrueBlocks.io",
					Tags:       "37-SelfDestructed",
				},
			},
			wantModified: false,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			gotModified, err := cleanName(tt.args.chain, tt.args.name)
			if (err != nil) != tt.wantErr {
				t.Errorf("cleanName() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if gotModified != tt.wantModified {
				t.Errorf("cleanName() = %v, want %v", gotModified, tt.wantModified)
			}
			if tt.wantModified && !reflect.DeepEqual(tt.args.name, tt.expectedName) {
				t.Errorf("cleanToken() = %+v, want %+v", tt.args.name, tt.expectedName)
			}
		})
	}
}

func Test_cleanName_edgeCases(t *testing.T) {
	type args struct {
		chain string
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
			name: "was contract",
			args: args{
				chain: "mainnet",
				name: &types.Name{
					IsContract: true,
					IsErc20:    true,
					IsErc721:   true,
					Decimals:   1,
					Name:       "DAO Drain 089",
					Petname:    "ideally-funky-turkey",
					Source:     "GethSource Ether Camp",
					Tags:       "80-Malicious:DaoDrain",
					Address:    base.HexToAddress("0xbc07118b9ac290e4622f5e77a0853539789effbe"),
				},
			},
			wantModified: false,
		},
		// 0x971a6ff4f5792f3e0288f093340fb36a826aae96
		{
			name: "was contract",
			args: args{
				chain: "mainnet",
				name: &types.Name{
					IsContract: true,
					IsErc20:    true,
					IsErc721:   true,
					Decimals:   18,
					Symbol:     "ALTNOUNS",
					Name:       "AltNouns",
					Petname:    "promptly-integral-rat",
					Source:     "On chain",
					Tags:       "50-Tokens:ERC721",
					Address:    base.HexToAddress("0x971a6ff4f5792f3e0288f093340fb36a826aae96"),
				},
			},
			wantModified: false,
		},
		{
			name: "name fourbyte collision",
			args: args{
				chain: "mainnet",
				name: &types.Name{
					IsContract: true,
					Petname:    "solely-caring-basilisk",
					Tags:       "90-Individuals:Twitter",
					Address:    base.HexToAddress("0xac4df82fe37ea2187bc8c011a23d743b4f39019a"),
				},
			},
			wantModified: false,
		},
		// This contract has double space in name()
		{
			name: "double space",
			args: args{
				chain: "mainnet",
				name: &types.Name{
					Address:    base.HexToAddress("0x3591ac25cd3dc66ed67eed87c8507c965f167258"),
					Decimals:   18,
					IsContract: true,
					IsErc20:    true,
					Name:       "LEE Chain V1.0",
					Petname:    "vastly-pet-quetzal",
					Source:     "On chain",
					Symbol:     "LEE",
					Tags:       "50-Tokens:ERC20",
				},
			},
			wantModified: false,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			gotModified, err := cleanName(tt.args.chain, tt.args.name)
			if (err != nil) != tt.wantErr {
				t.Errorf("cleanName() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if gotModified != tt.wantModified {
				t.Errorf("cleanName() = %v, want %v", gotModified, tt.wantModified)
			}
			if tt.wantModified && !reflect.DeepEqual(tt.args.name, tt.expectedName) {
				t.Errorf("cleanToken() = %+v, want %+v", tt.args.name, tt.expectedName)
			}
		})
	}
}

var benchmarkLimit = 50

func BenchmarkCleanSync(b *testing.B) {
	chain := utils.GetTestChain()
	parts := names.Regular

	for i := 0; i < b.N; i++ {
		b.StopTimer()
		allNames, err := names.LoadNamesMap("mainnet", parts, []string{})
		if err != nil {
			b.Fatal(err)
		}
		prefundMap, err := preparePrefunds("mainnet")
		if err != nil {
			b.Fatal(err)
		}

		count := 0

		b.StartTimer()
		for _, name := range allNames {
			count++

			// Benchmark break
			if count == benchmarkLimit {
				break
			}

			modified, err := cleanName("mainnet", &name)
			if err != nil {
				b.Fatal(err)
			}
			if isPrefund := prefundMap[name.Address]; isPrefund != name.IsPrefund {
				name.IsPrefund = isPrefund
				modified = true
			}

			if !modified {
				continue
			}

			// Update modified (no disk writes yet)
			if err = names.UpdateName(names.DatabaseRegular, chain, &name); err != nil {
				b.Fatal(err)
			}
		}
	}
}

func BenchmarkCleanConcurrent(b *testing.B) {
	chain := utils.GetTestChain()
	parts := names.Regular

	for i := 0; i < b.N; i++ {
		b.StopTimer()
		allNames, err := names.LoadNamesMap("mainnet", parts, []string{})
		if err != nil {
			b.Fatal(err)
		}
		prefundMap, err := preparePrefunds("mainnet")
		if err != nil {
			b.Fatal(err)
		}

		appMap := make(map[base.Address]types.Name, benchmarkLimit)
		count := 0
		for addr, name := range allNames {
			if count == benchmarkLimit {
				break
			}
			count++
			appMap[addr] = name
		}

		b.StartTimer()

		iterFunc := func(key base.Address, name types.Name) error {
			modified, err := cleanName("mainnet", &name)
			if err != nil {
				panic(err)
			}
			if isPrefund := prefundMap[name.Address]; isPrefund != name.IsPrefund {
				name.IsPrefund = isPrefund
				modified = true
			}

			if !modified {
				return nil
			}

			// Update modified (no disk writes yet)
			if err = names.UpdateName(names.DatabaseRegular, chain, &name); err != nil {
				panic(err)
			}
			return nil
		}

		iterErrorChan := make(chan error)
		iterCtx, iterCancel := context.WithCancel(context.Background())
		defer iterCancel()
		go utils.IterateOverMap(iterCtx, iterErrorChan, appMap, iterFunc)
		if err := <-iterErrorChan; err != nil {
			b.Fatal(err)
		}
	}
}
