// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package version

import (
	"reflect"
	"testing"
)

func TestVersion(t *testing.T) {
	early := Version{
		Major:  0,
		Minor:  10,
		Build:  0,
		Aspect: "",
	}
	late := Version{
		Major:  0,
		Minor:  20,
		Build:  0,
		Aspect: "",
	}

	if early.Uint64() >= late.Uint64() {
		t.Error("early is not earlier than late")
	}

	if late.Uint64() < early.Uint64() {
		t.Error("late is earlier than early")
	}
}

func TestNewVersion(t *testing.T) {
	type args struct {
		str string
	}
	tests := []struct {
		name     string
		args     args
		wantVers Version
	}{
		{
			name: "library version string",
			args: args{
				str: "GHC-TrueBlocks//0.76.0-beta",
			},
			wantVers: Version{
				Major:  0,
				Minor:  76,
				Build:  0,
				Aspect: "beta",
			},
		},
		{
			name: "version 1.0.0",
			args: args{
				str: "v1.0.0-release",
			},
			wantVers: Version{
				Major:  1,
				Minor:  0,
				Build:  0,
				Aspect: "release",
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			gotVers := NewVersion(tt.args.str)
			if !reflect.DeepEqual(gotVers, tt.wantVers) {
				t.Errorf("NewVersion() = %v, want %v", gotVers, tt.wantVers)
			}
		})
	}
}

func TestIsValidVersion(t *testing.T) {
	tests := []struct {
		test  string
		valid bool
	}{
		{
			test:  "1.0.0-release",
			valid: false,
		},
		{
			test:  "v1.0-release",
			valid: false,
		},
		{
			test:  "va.0.0-release",
			valid: false,
		},
		{
			test:  "v1.0.0-",
			valid: false,
		},
		{
			test:  "v1..0-beta",
			valid: false},
		{
			test:  "v1.0.0-release",
			valid: true,
		},
		{
			test:  "trueblocks-core@v0.40.0", // special case
			valid: true,
		},
	}

	for _, tt := range tests {
		res := IsValidVersion(tt.test)
		if tt.valid != res {
			t.Errorf("IsValidVersion(%s) = %v, want %v", tt.test, res, tt.valid)
		}
	}
}
