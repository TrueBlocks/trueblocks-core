// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package version

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type Version struct {
	Major  int64  `json:"major"`
	Minor  int64  `json:"minor"`
	Build  int64  `json:"build"`
	Aspect string `json:"aspect"`
}

func NewVersion(str string) Version {
	str = strings.Replace(str, "GHC-TrueBlocks//", "", -1)
	str = strings.Replace(strings.Replace(str, "-", ".", -1), "v", "", -1)
	parts := strings.Split(str, ".")

	var vers Version
	if len(parts) > 0 {
		vers.Major = utils.MustParseInt(parts[0])
	}
	if len(parts) > 1 {
		vers.Minor = utils.MustParseInt(parts[1])
	}
	if len(parts) > 2 {
		vers.Build = utils.MustParseInt(parts[2])
	}

	if len(parts) > 3 {
		vers.Aspect = parts[3]
	}

	return vers
}

// Uint64 returns version as a single uint64
func (ref *Version) Uint64() uint64 {
	return uint64((ref.Major * 1000000) + (ref.Minor * 1000) + ref.Build)
}

// String prints the version to a string
func (ref *Version) String() string {
	return fmt.Sprintf("v%d.%d.%d-%s", ref.Major, ref.Minor, ref.Build, ref.Aspect)
}

func VersionString() string {
	return strings.Replace(LibraryVersion, "GHC-TrueBlocks//", "v", -1)
}
