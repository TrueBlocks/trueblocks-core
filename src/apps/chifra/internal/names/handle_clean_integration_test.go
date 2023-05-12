//go:build integration
// +build integration

// Copyright 2023 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package namesPkg

import (
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func Test_cleanName(t *testing.T) {
	type args struct {
		chain string
		name  *types.SimpleName
	}
	tests := []struct {
		name         string
		args         args
		wantModified bool
		wantErr      bool
		expectedName *types.SimpleName
	}{
		{
			name: "was contract",
			args: args{
				chain: "mainnet",
				name: &types.SimpleName{
					IsContract: true,
					Address:    base.HexToAddress("0x139e20773accc72ef75765fdff158244845c3888"),
				},
			},
			wantModified: true,
			expectedName: &types.SimpleName{
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
				name: &types.SimpleName{
					IsContract: false,
					Address:    base.HexToAddress("0x139e20773accc72ef75765fdff158244845c3888"),
				},
			},
			wantModified: true,
			expectedName: &types.SimpleName{
				Address: base.HexToAddress("0x139e20773accc72ef75765fdff158244845c3888"),
				Petname: "remarkably-solid-cardinal",
				Tags:    "90-Individuals:Other",
			},
		},
		{
			name: "not modified",
			args: args{
				chain: "mainnet",
				name: &types.SimpleName{
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

func TestTemp(t *testing.T) {
	m, err := names.LoadNamesMap("mainnet", names.Regular, nil)
	if err != nil {
		t.Fatal(err)
	}

	// 0xf53ad2c6851052a81b42133467480961b2321c09 -- DS Token
	// 0x461733c17b0755ca5649b6db08b3e213fcf22546 -- DS Token
	// 0x4c8692cc08b14fa8b74b6f313394b8997aad721d
	name, ok := m[base.HexToAddress("0xce5c603c78d047ef43032e96b5b785324f753a4f")]
	if !ok {
		t.Fatal("no name")
	}
	mod, err := cleanName("mainnet", &name)
	if err != nil {
		t.Fatal(err)
	}

	if mod {
		t.Fatal("modified")
	}
}
