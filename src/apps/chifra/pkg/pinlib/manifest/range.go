/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
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
