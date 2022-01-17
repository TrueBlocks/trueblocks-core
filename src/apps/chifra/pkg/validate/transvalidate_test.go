// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package validate

import (
	"testing"
)

func TestIsTransHash(t *testing.T) {
	if IsTransHash("fe611cbee814a8e84c3339186b6dc973947c2dc058cd41a5e3669f3b7f4c980d") {
		t.Error("Passes for hash without 0x")
	}

	if IsTransHash("0xfe611cbee814a8e84c3339186b6dc973947c2dc058") {
		t.Error("Passes for short hash")
	}

	if IsTransHash("0xgg611cbee814a8e84c3339186b6dc973947c2dc058cd41a5e3669f3b7f4c980d") {
		t.Error("Passes for invalid hash")
	}

	if !IsTransHash("0x060e4cf9fa8d34a8b423b5b3691b2541255ff7974ff16699e104edcfb63bd521") {
		t.Error("Fails for a valid transaction hash")
	}

	// TODO: We don't check if it's a valid transaction hash
	// if IsTransHash("0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53") {
	// 	t.Error("Passes for a block hash")
	// }
}

func TestIsTransBlockNumAndIndex(t *testing.T) {
	if IsTransBlockNumAndId("notanumber") {
		t.Error("Passes for invalid transaction id")
	}

	if IsTransBlockNumAndId("") {
		t.Error("Passes for empty transaction id")
	}

	if !IsTransBlockNumAndId("0.1234") {
		t.Error("Fails at zero block")
	}

	if !IsTransBlockNumAndId("0xff.0") {
		t.Error("Fails for hex.num")
	}

	if !IsTransBlockNumAndId("0.0xf") {
		t.Error("Fails for num.hex")
	}

	if !IsTransBlockNumAndId("0xff.0xf") {
		t.Error("Fails for hex.hex")
	}

	if !IsTransBlockNumAndId("1000.0") {
		t.Error("Fails for num.num")
	}
}

func TestIsBlockHashAndId(t *testing.T) {
	if IsTransBlockHashAndId("notanumber") {
		t.Error("Passes for invalid transaction id")
	}

	if IsTransBlockHashAndId("") {
		t.Error("Passes for empty transaction id")
	}

	if IsTransBlockHashAndId("0xd4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8.1234") {
		t.Error("Passes with too short hash block")
	}

	if IsTransBlockHashAndId("0xd4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8fa3a3.1234") {
		t.Error("Passes with too long hash")
	}

	if !IsTransBlockHashAndId("0xd4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8fa3.0x0") {
		t.Error("Fails for hash.hex")
	}

	if !IsTransBlockHashAndId("0xd4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8fa3.10") {
		t.Error("Fails for hash.num")
	}
}

func TestValidateTransIdentifiers(t *testing.T) {
	type args struct {
		identifiers []string
		validTypes  ValidArgumentType
		maxRanges   int
	}
	tests := []struct {
		name    string
		args    args
		wantErr bool
	}{
		{
			name: "correct transaction hashes",
			args: args{
				identifiers: []string{
					"0xd3b9663a5f2367cb1ebeff5eab7d45cc24931678a1e96348291db13057ad438f",
					"0x98862798ac03f688cba584931e0fe42bf17f37a7c48f905c6fc4e9f2a0ec7cb4",
				},
				validTypes: ValidArgumentTransHash,
				maxRanges:  1,
			},
			wantErr: false,
		},
		{
			name: "correct blockNum.transId",
			args: args{
				identifiers: []string{
					"10.0",
					"100.1",
					"0x100.2",
				},
				validTypes: ValidArgumentTransBlockNumberAndId,
				maxRanges:  1,
			},
			wantErr: false,
		},
		{
			name: "correct blockNum.transId",
			args: args{
				identifiers: []string{
					"0xd3b9663a5f2367cb1ebeff5eab7d45cc24931678a1e96348291db13057ad438f.0x0",
					"0xd3b9663a5f2367cb1ebeff5eab7d45cc24931678a1e96348291db13057ad438f.1",
				},
				validTypes: ValidArgumentTransBlockHashAndId,
				maxRanges:  1,
			},
			wantErr: false,
		},
		{
			name: "correct mixed identifiers",
			args: args{
				identifiers: []string{
					"0xd3b9663a5f2367cb1ebeff5eab7d45cc24931678a1e96348291db13057ad438f",
					"0x98862798ac03f688cba584931e0fe42bf17f37a7c48f905c6fc4e9f2a0ec7cb4",
					"10.0",
					"100.1",
					"0x100.2",
					"0xd3b9663a5f2367cb1ebeff5eab7d45cc24931678a1e96348291db13057ad438f.0x0",
					"0xd3b9663a5f2367cb1ebeff5eab7d45cc24931678a1e96348291db13057ad438f.1",
				},
				validTypes: ValidArgumentTransHash | ValidArgumentTransBlockHashAndId | ValidArgumentTransBlockNumberAndId,
				maxRanges:  1,
			},
			wantErr: false,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if err := ValidateIdentifiers(
				"mainnet",
				tt.args.identifiers,
				tt.args.validTypes,
				tt.args.maxRanges,
			); (err != nil) != tt.wantErr {
				t.Errorf("ValidateIdentifiers() error = %v, wantErr %v", err, tt.wantErr)
			}
		})
	}
}
