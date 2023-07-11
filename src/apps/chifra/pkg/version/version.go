// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package version

import (
	"strconv"
	"strings"
)

type Version struct {
	Major  int64  `json:"major"`
	Minor  int64  `json:"minor"`
	Build  int64  `json:"build"`
	Aspect string `json:"aspect"`
}

func NewVersion(str string) (vers Version, err error) {
	if str[:3] == "GHC" {
		// First, remove the part that cannot be parsed with ParseInt
		str = str[len("GHC-TrueBlocks//"):]
	}
	str = strings.Replace(strings.Replace(str, "-", ".", -1), "v", "", -1)
	parts := strings.Split(str, ".")
	if len(parts) > 0 {
		if vers.Major, err = strconv.ParseInt(parts[0], 10, 32); err != nil {
			return vers, err
		}
	}
	if len(parts) > 1 {
		if vers.Minor, err = strconv.ParseInt(parts[1], 10, 32); err != nil {
			return vers, err
		}
	}
	if len(parts) > 2 {
		if vers.Build, err = strconv.ParseInt(parts[2], 10, 32); err != nil {
			return vers, err
		}
	}
	if len(parts) > 3 {
		vers.Aspect = parts[3]
	}

	return vers, nil
}

func (ref *Version) IsEarlierThan(test Version) bool {
	if ref.Major < test.Major {
		return true
	}
	if ref.Minor < test.Minor {
		return true
	}
	if ref.Build < test.Build {
		return true
	}
	return false
}

// Uint64 returns version as a single uint64
func (ref *Version) Uint64() uint64 {
	return uint64((ref.Major * 1000000) + (ref.Minor * 1000) + ref.Build)
}
