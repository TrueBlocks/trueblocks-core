package types

import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

type PostType uint64

const (
	Invalid PostType = 1 << iota
	Genesis
	DiffDiff
	SameSame
	DiffSame
	SameDiff
	ShouldNotHappen
)

func (r PostType) String() string {
	switch r {
	case Genesis:
		return "genesis-diff"
	case DiffDiff:
		return "diff-diff"
	case SameSame:
		return "same-same"
	case DiffSame:
		return "diff-same"
	case SameDiff:
		return "same-diff"
	default:
		return "invalid"
	}
}

func (r *PostType) MarshalCache(writer io.Writer) error {
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
