package types

import (
	"io"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

type ReconType uint64

const (
	Invalid ReconType = 1 << iota
	Genesis
	DiffDiff
	SameSame
	DiffSame
	SameDiff
	ShouldNotHappen
	First
	Last
)

func (r ReconType) String() string {
	l := func(r ReconType, s string) string {
		if r&First != 0 {
			s = "first-" + s
			s = strings.Replace(s, "first-same", "first", 1)
		}
		if r&Last != 0 {
			s = s + "-last"
			s = strings.Replace(s, "same-last", "last", 1)
		}
		return s
	}

	rr := r &^ (First | Last)
	switch rr {
	case Genesis:
		return "genesis-diff"
	case DiffDiff:
		return l(r, "diff-diff")
	case SameSame:
		return l(r, "same-same")
	case DiffSame:
		return l(r, "diff-same")
	case SameDiff:
		return l(r, "same-diff")
	default:
		return "invalid"
	}
}

func (r *ReconType) MarshalCache(writer io.Writer) error {
	return cache.WriteValue(writer, int(*r))
}

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

func (r *TrialBalType) MarshalCache(writer io.Writer) error {
	return cache.WriteValue(writer, int(*r))
}
