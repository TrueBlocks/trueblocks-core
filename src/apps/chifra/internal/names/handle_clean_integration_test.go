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
			if !reflect.DeepEqual(tt.args.name, tt.expectedName) {
				t.Errorf("cleanToken() = %+v, want %+v", tt.args.name, tt.expectedName)
			}
		})
	}
}
