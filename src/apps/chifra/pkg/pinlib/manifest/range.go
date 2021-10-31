package manifest

import (
	"strconv"
	"strings"
)

type ManifestRange [2]uint64

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
