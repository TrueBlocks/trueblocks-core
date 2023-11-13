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
	chain := utils.GetTestChain()
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
				Tags:       "50-Tokens:ERC20",
				Address:    base.HexToAddress("0x6982508145454Ce325dDbE47a25d4ec3d2311933"),
				Name:       "Pepe",
				Symbol:     "PEPE",
				Source:     "On chain",
				Decimals:   18,
				Petname:    "properly-sincere-filly",
				IsContract: true,
				IsErc20:    true,
				IsCustom:   true,
			},
		},
		// {
		// 	name: "eoa, not a token",
		// 	fields: fields{
		// 		Autoname: "0x054993ab0f2b1acc0fdc65405ee203b4271bebe6",
		// 	},
		// 	wantErr: true,
		// 	expected: &types.SimpleName{
		// 		Address:  base.HexToAddress("0x054993ab0f2b1acc0fdc65405ee203b4271bebe6"),
		// 		Name:     "M-Wallet 0x5499",
		// 		Petname:  "sadly-settling-anteater",
		// 		Source:   "EtherScan.io",
		// 		Tags:     "00-Active",
		// 		IsCustom: true,
		// 	},
		// },
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			opts := &NamesOptions{
				Autoname:     tt.fields.Autoname,
				AutonameAddr: base.HexToAddress(tt.fields.Autoname),
				Globals: globals.GlobalOptions{
					OutputOptions: output.OutputOptions{
						Chain: utils.GetTestChain(),
					},
				},
			}
			_, err := opts.readContractAndClean()
			wanted := tt.wantErr
			have := err != nil
			if (wanted && !have) || (have && !wanted) {
				t.Errorf("TestNamesOptions_autoname error = %v, wantErr %v", err, tt.wantErr)
			}
			if tt.expected != nil {
				name := names.ReadName(names.DatabaseCustom, chain, opts.AutonameAddr)
				if !reflect.DeepEqual(name, tt.expected) {
					t.Errorf("TestNamesOptions_autoname = %+v, want %+v", name, tt.expected)
				}
			}
		})
	}
}
