// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package validate

import (
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func TestIsBlockHash(t *testing.T) {
	if IsBlockHash("fe611cbee814a8e84c3339186b6dc973947c2dc058cd41a5e3669f3b7f4c980d") {
		t.Error("Passes even if 0x is missing")
	}

	if IsBlockHash("0xfe611cbee814a8e84c3339186b6dc973947c2dc058") {
		t.Error("Passes for too short strings")
	}

	if IsBlockHash("0xgg611cbee814a8e84c3339186b6dc973947c2dc058cd41a5e3669f3b7f4c980d") {
		t.Error("Passes for non-hex strings")
	}

	if !IsBlockHash("0xfe611cbee814a8e84c3339186b6dc973947c2dc058cd41a5e3669f3b7f4c980d") {
		t.Error("Fails for a valid block hash")
	}
}

func TestIsBlockNumber(t *testing.T) {
	if check, _ := IsBlockNumber("notanumber"); check {
		t.Error("Passes for strings")
	}

	if check, _ := IsBlockNumber(""); check {
		t.Error("Passes for empty string")
	}

	// We only support decimal numbers for block numbers
	if check, _ := IsBlockNumber("0xff"); !check {
		t.Error("Fails for hex")
	}

	if check, _ := IsBlockNumber("0"); !check {
		t.Error("Fails for 0")
	}

	if check, _ := IsBlockNumber("1000"); !check {
		t.Error("Fails for numbers")
	}

	check, value := IsBlockNumber("12147043")
	if !check || value != uint32(12147043) {
		t.Fatal("Failed for correct input:", check, value)
	}
}

func TestIsBlockNumberList(t *testing.T) {
	type args struct {
		strs []string
	}
	tests := []struct {
		name  string
		args  args
		want  bool
		want1 []blknum_t
	}{
		{
			name: "list of valid blocknumbers",
			args: args{
				strs: []string{"12147043", "12147042", "12147041"},
			},
			want:  true,
			want1: []blknum_t{12147043, 12147042, 12147041},
		},
		{
			name: "list of invalid blocknumbers",
			args: args{
				strs: []string{"0xfff", "latest", "newest"},
			},
			want:  false,
			want1: nil,
		},
		{
			name: "list of mixed blocknumbers",
			args: args{
				strs: []string{"12147043", "latest", "newest"},
			},
			want:  false,
			want1: nil,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, got1 := IsBlockNumberList(tt.args.strs)
			if got != tt.want {
				t.Errorf("IsBlockNumberList() got = %v, want %v", got, tt.want)
			}
			if !reflect.DeepEqual(got1, tt.want1) {
				t.Errorf("IsBlockNumberList() got1 = %v, want %v", got1, tt.want1)
			}
		})
	}
}

func TestIsSpecialBlock(t *testing.T) {
	if tslib.IsSpecialBlock(utils.GetTestChain(), "london byzantium") {
		t.Error("Passes for invalid string (space)")
	}

	if tslib.IsSpecialBlock(utils.GetTestChain(), "lądon") {
		t.Error("Passes for string with invalid characters")
	}

	if tslib.IsSpecialBlock(utils.GetTestChain(), "123") {
		t.Error("Passes for number")
	}

	if tslib.IsSpecialBlock(utils.GetTestChain(), "ab") {
		t.Error("Passes for too short strings")
	}

	if !tslib.IsSpecialBlock(utils.GetTestChain(), "london") {
		t.Error("Fails for valid block name")
	}

	if !tslib.IsSpecialBlock(utils.GetTestChain(), "devcon1") {
		t.Error("Fails for valid block name with a number")
	}
}

func TestIsDateTimeString(t *testing.T) {
	if !IsDateTimeString("2021-09-28") {
		t.Error("Fails for date without time")
	}

	if !IsDateTimeString("2021-09-28T16") {
		t.Error("Fails for date without minutes")
	}

	if !IsDateTimeString("2021-09-28T16:42") {
		t.Error("Fails for date without seconds")
	}

	if !IsDateTimeString("2021-09-28T16:42:15") {
		t.Error("Fails for date with time")
	}

	if !IsDateTimeString("2021-09-28T16:42:15UTC") {
		t.Error("Fails for date with time and timezone")
	}

	if isBeforeFirstBlock(utils.GetTestChain(), "2015-07-30T15:26:15") {
		t.Error("Fails for exact first block date")
	}

	if IsDateTimeString("hello") {
		t.Error("Passes for invalid date #1")
	}

	if IsDateTimeString("2020-08") {
		t.Error("Passes for invalid date #2")
	}
}

func TestIsRange(t *testing.T) {
	if r, _ := IsRange(utils.GetTestChain(), "100"); r {
		t.Error("Passes for non-range")
	}

	if r, _ := IsRange(utils.GetTestChain(), "-0100"); r {
		t.Error("Passes for malformed string (1)")
	}

	if r, _ := IsRange(utils.GetTestChain(), "100-"); r {
		t.Error("Passes for malformed string (2)")
	}

	if r, _ := IsRange(utils.GetTestChain(), "0-100:"); r {
		t.Error("Passes for malformed string (3)")
	}

	if r, _ := IsRange(utils.GetTestChain(), "0-100"); !r {
		t.Error("Fails for range without step")
	}

	if r, _ := IsRange(utils.GetTestChain(), "100-100000:20"); !r {
		t.Error("Fails for range with step")
	}

	if r, _ := IsRange(utils.GetTestChain(), "london-100"); !r {
		t.Error("Fails for special")
	}

	if r, _ := IsRange(utils.GetTestChain(), "100-2021-04-20"); !r {
		t.Error("Fails for number and a date")
	}
}

func TestIsRangeLatestAsStart(t *testing.T) {
	expected := "cannot start range with 'latest'"
	_, err := IsRange(utils.GetTestChain(), "latest-10")

	if err.Error() != expected {
		t.Errorf("Error mismatch: %s", err)
	}
}

func TestIsRangeEndGreaterThanStart(t *testing.T) {
	expected := "'stop' must be strictly larger than 'start'"
	_, err := IsRange(utils.GetTestChain(), "1000-10")

	if err == nil {
		t.Error("No error")
	}

	if err.Error() != expected {
		t.Errorf("Error mismatch: %s", err)
	}
}

func TestIsRangeModifierError(t *testing.T) {
	expected := "Input argument appears to be invalid. No such skip marker: biweekly"
	_, err := IsRange(utils.GetTestChain(), "0-1000:biweekly")

	if err.Error() != expected {
		t.Errorf("Error mismatch: %s", err)
	}
}

func TestIsRangeInvalidSpecialStart(t *testing.T) {
	valid, err := IsRange(utils.GetTestChain(), "notexisting-1000:10")

	if valid {
		t.Error("Invalid special passing")
	}

	if literalErr, ok := err.(*InvalidIdentifierLiteralError); !ok {
		t.Errorf("Wrong error returned: %s", literalErr)
	}
}

func TestIsRangeInvalidSpecialStop(t *testing.T) {
	valid, err := IsRange(utils.GetTestChain(), "1000-notexisting:10")

	if valid {
		t.Error("Invalid special passing")
	}

	if literalErr, ok := err.(*InvalidIdentifierLiteralError); !ok {
		t.Errorf("Wrong error returned: %s", literalErr)
	}
}

func TestValidateBlockIdentifiers(t *testing.T) {
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
			name: "correct block numbers",
			args: args{
				identifiers: []string{"10", "100", "1000", "2542852800"},
				validTypes:  ValidArgumentBlockNumber | ValidArgumentTimestamp,
				maxRanges:   1,
			},
			wantErr: false,
		},
		{
			name: "correct block hashes",
			args: args{
				identifiers: []string{
					"0xd3b9663a5f2367cb1ebeff5eab7d45cc24931678a1e96348291db13057ad438f",
					"0x98862798ac03f688cba584931e0fe42bf17f37a7c48f905c6fc4e9f2a0ec7cb4",
				},
				validTypes: ValidArgumentBlockHash,
				maxRanges:  1,
			},
			wantErr: false,
		},
		{
			name: "correct datetime",
			args: args{
				identifiers: []string{
					"2021-06-28",
					"2021-07-15T10:25:30",
				},
				validTypes: ValidArgumentDate,
				maxRanges:  1,
			},
			wantErr: false,
		},
		// TODO: Turn off go testing that requires ts.bin
		// {
		// 	name: "too early",
		// 	args: args{
		// 		identifiers: []string{
		// 			"2014-01-01",
		// 		},
		// 		validTypes: ValidArgumentDate,
		// 		maxRanges:  1,
		// 	},
		// 	wantErr: true,
		// },
		{
			name: "correct special",
			args: args{
				identifiers: []string{
					"london",
					"devcon1",
				},
				validTypes: ValidArgumentSpecialBlock,
				maxRanges:  1,
			},
			wantErr: false,
		},
		{
			name: "correct range",
			args: args{
				identifiers: []string{
					"10-1000:50",
				},
				validTypes: ValidArgumentRange,
				maxRanges:  1,
			},
			wantErr: false,
		},
		{
			name: "correct misc types",
			args: args{
				identifiers: []string{
					"london",
				},
				validTypes: ValidArgumentBlockHash | ValidArgumentSpecialBlock,
				maxRanges:  1,
			},
			wantErr: false,
		},
		{
			name: "correct misc identifiers",
			args: args{
				identifiers: []string{
					"london",
					"10-1000:weekly",
					"1000",
				},
				validTypes: ValidArgumentBlockNumber | ValidArgumentSpecialBlock | ValidArgumentRange,
				maxRanges:  1,
			},
			wantErr: false,
		},
		{
			name: "incorrect range number",
			args: args{
				identifiers: []string{
					"10-1000:50",
					"10-1000:50",
					"10-1000:50",
				},
				validTypes: ValidArgumentRange,
				maxRanges:  2,
			},
			wantErr: true,
		},
		{
			name: "incorrect misc identifiers",
			args: args{
				identifiers: []string{
					"london",
					"10-1000:weekly",
					"1000",
				},
				validTypes: ValidArgumentBlockNumber | ValidArgumentSpecialBlock,
				maxRanges:  1,
			},
			wantErr: true,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if err := ValidateIdentifiers(
				utils.GetTestChain(),
				tt.args.identifiers,
				tt.args.validTypes,
				tt.args.maxRanges,
				nil,
			); (err != nil) != tt.wantErr {
				t.Errorf("ValidateIdentifiers() error = %v, wantErr %v", err, tt.wantErr)
			}
		})
	}
}
