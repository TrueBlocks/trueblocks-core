package types

import (
	"fmt"
	"strconv"
	"strings"
)

type TrialBalType uint64

const (
	TrialBalEth      TrialBalType = 1
	TrialBalTraceEth TrialBalType = 2
	TrialBalToken    TrialBalType = 3
	TrialBalNft      TrialBalType = 4
)

func (r TrialBalType) String() string {
	return map[TrialBalType]string{
		TrialBalEth:      "eth",
		TrialBalTraceEth: "trace-eth",
		TrialBalToken:    "token",
		TrialBalNft:      "token-nft",
	}[r]
}

// UnmarshalText implements the encoding.TextUnmarshaler interface.
func (r *TrialBalType) UnmarshalText(text []byte) error {
	s := string(text)
	switch s {
	case "eth":
		*r = TrialBalEth
	case "trace-eth":
		*r = TrialBalTraceEth
	case "token":
		*r = TrialBalToken
	case "token-nft":
		*r = TrialBalNft
	default:
		// If the input is numeric, try parsing it as a number.
		n, err := strconv.ParseUint(s, 10, 64)
		if err != nil {
			return fmt.Errorf("unknown TrialBalType %q", s)
		}
		*r = TrialBalType(n)
	}
	return nil
}

// UnmarshalJSON implements the json.Unmarshaler interface.
func (r *TrialBalType) UnmarshalJSON(data []byte) error {
	str := strings.Trim(strings.TrimSpace(string(data)), "\"")
	if len(str) == 0 {
		return nil
	}
	return r.UnmarshalText([]byte(str))
}
