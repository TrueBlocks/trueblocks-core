// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

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

func (ref *Version) IsBackLevel(test Version) bool {
	if test.Major < ref.Major {
		return true
	}
	if test.Minor < ref.Minor {
		return true
	}
	if test.Build < ref.Build {
		return true
	}
	return false
}
