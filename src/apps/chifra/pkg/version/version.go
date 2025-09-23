// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package version

import (
	"fmt"
	"strconv"
	"strings"
)

type Version struct {
	Major  int64  `json:"major"`
	Minor  int64  `json:"minor"`
	Build  int64  `json:"build"`
	Aspect string `json:"aspect"`
}

func NewVersion(str string) Version {
	str = strings.ReplaceAll(str, "GHC-TrueBlocks//", "")
	str = strings.ReplaceAll(strings.ReplaceAll(str, "-", "."), "v", "")
	parts := strings.Split(str, ".")

	var vers Version
	if len(parts) > 0 {
		vers.Major, _ = strconv.ParseInt(parts[0], 0, 64)
	}
	if len(parts) > 1 {
		vers.Minor, _ = strconv.ParseInt(parts[1], 0, 64)
	}
	if len(parts) > 2 {
		vers.Build, _ = strconv.ParseInt(parts[2], 0, 64)
	}

	if len(parts) > 3 {
		vers.Aspect = parts[3]
	}

	return vers
}

// IsValidVersion returns true if the string is a full version string
func IsValidVersion(test string) bool {
	test = strings.ReplaceAll(test, "trueblocks-core@", "")
	if !strings.HasPrefix(test, "v") {
		return false
	}
	parts := strings.Split(test, "-")
	if len(parts) > 2 {
		return false
	}
	if len(parts) < 2 {
		parts = append(parts, "beta")
	}
	if len(parts[1]) == 0 {
		return false
	}
	parts[0] = strings.ReplaceAll(parts[0], "v", "")
	nums := strings.Split(parts[0], ".")
	if len(nums) != 3 {
		return false
	}
	for _, n := range nums {
		if len(n) == 0 {
			return false
		}
		t := strings.Trim(n, "0123456789")
		if len(t) > 0 {
			return false
		}
	}
	return true
}

// Uint64 returns version as a single uint64
func (ref *Version) Uint64() uint64 {
	return uint64((ref.Major * 1000000) + (ref.Minor * 1000) + ref.Build)
}

// String prints the version to a string
func (ref *Version) String() string {
	return strings.Trim(strings.Trim(fmt.Sprintf("v%d.%d.%d-%s", ref.Major, ref.Minor, ref.Build, ref.Aspect), "-"), " ")
}
