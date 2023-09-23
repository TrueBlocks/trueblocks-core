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

	if !early.IsEarlierThan(late) {
		t.Error("early is not earlier than late")
	}

	if late.IsEarlierThan(early) {
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
