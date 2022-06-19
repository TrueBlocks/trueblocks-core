// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

import (
	"errors"
	"strconv"
	"strings"
)

var ErrMalformedManifest = errors.New("malformed manifest")
var rangePartLength = 9
var rangeLength = (rangePartLength * 2) + 1 // 1 for separator

type ManifestRange [2]uint64

// UnmarshalJSON populates a ManifestRange from a []byte
func (mr *ManifestRange) UnmarshalJSON(data []byte) error {
	unquoted, err := strconv.Unquote(string(data))
	if err != nil {
		return err
	}

	newManifestRange, err := StringToManifestRange(unquoted)
	if err != nil {
		return err
	}

	*mr = newManifestRange

	return nil
}

// StringToManifestRange parses a string found in JSON manifest
// and returns ManifestRange
func StringToManifestRange(source string) (ManifestRange, error) {
	mr := [2]uint64{0, 0}

	if len(source) == 0 {
		return mr, nil
	}
	if !(len(source) == rangeLength && source[rangePartLength] == '-') {
		return mr, ErrMalformedManifest
	}

	parts := strings.Split(source, "-")

	start, err := strconv.ParseUint(parts[0], 10, 64)
	if err != nil {
		return mr, err
	}

	end, err := strconv.ParseUint(parts[1], 10, 64)
	if err != nil {
		return mr, err
	}

	mr[0] = start
	mr[1] = end

	return mr, nil
}
