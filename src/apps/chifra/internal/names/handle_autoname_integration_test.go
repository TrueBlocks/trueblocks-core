//go:build integration
// +build integration

// Copyright 2023 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package namesPkg

import (
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func TestNamesOptions_autoname(t *testing.T) {
	type fields struct {
		Autoname string
	}
	tests := []struct {
		name     string
		fields   fields
		wantErr  bool
		expected *types.SimpleName
	}{
		{
			name: "a token",
			fields: fields{
				// PEPE
				Autoname: "0x6982508145454Ce325dDbE47a25d4ec3d2311933",
			},
			expected: &types.SimpleName{
				Address:    base.HexToAddress("0x6982508145454Ce325dDbE47a25d4ec3d2311933"),
				Name:       "Pepe",
				IsContract: true,
				IsErc20:    true,
				Source:     "On chain",
				Symbol:     "PEPE",
				Decimals:   18,
				Petname:    "properly-sincere-filly",
				Tags:       "50-Tokens:ERC20",
			},
		},
		{
			name: "contract, not a token",
			fields: fields{
				// Unchained Index v1
				Autoname: "0x0c316b7042b419d07d343f2f4f5bd54ff731183d",
			},
			expected: &types.SimpleName{
				Address:    base.HexToAddress("0x0c316b7042b419d07d343f2f4f5bd54ff731183d"),
				Name:       "Unchained Index (v1.0)",
				IsContract: true,
				Petname:    "brightly-alert-bluegill",
				Tags:       "30-Contracts",
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			opts := &NamesOptions{
				Autoname: tt.fields.Autoname,
				Globals: globals.GlobalOptions{
					OutputOptions: output.OutputOptions{
						Chain: utils.GetTestChain(),
					},
				},
			}
			if _, err := opts.readContractAndClean(); (err != nil) != tt.wantErr {
				t.Errorf("NamesOptions.readContractAndClean() error = %v, wantErr %v", err, tt.wantErr)
			}
			if tt.expected != nil {
				name := names.ReadRegularName(base.HexToAddress(tt.fields.Autoname))
				if !reflect.DeepEqual(name, tt.expected) {
					t.Errorf("NamesOptions.readContractAndClean() = %+v, want %+v", name, tt.expected)
				}
			}
		})
	}
}
