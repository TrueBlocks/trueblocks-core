package index

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

type walkerFunc func(walker *CacheWalker, path string, first bool) (bool, error)

type CacheWalker struct {
	chain      string
	testMode   bool
	maxTests   int
	visitFunc1 walkerFunc
}

func NewCacheWalker(chain string, testMode bool, maxTests int, visitFunc walkerFunc) CacheWalker {
	return CacheWalker{
		chain:      chain,
		testMode:   testMode,
		maxTests:   maxTests,
		visitFunc1: visitFunc,
	}
}

func (walker *CacheWalker) MaxTests() int {
	return walker.maxTests
}

func (walker *CacheWalker) WalkBloomFilters(blockNums []uint64) error {
	filenameChan := make(chan walk.CacheFileInfo)

	var nRoutines int = 1
	go walk.WalkCacheFolder(context.Background(), walker.chain, walk.Index_Bloom, nil, filenameChan)

	cnt := 0
	for result := range filenameChan {
		switch result.Type {
		case walk.Index_Bloom:
			if walker.shouldDisplay(result, cnt, blockNums) {
				ok, err := walker.visitFunc1(walker, result.Path, cnt == 0)
				if err != nil {
					return err
				}
				if ok {
					cnt++
				} else {
					return nil
				}
			}
		case walk.Cache_NotACache:
			nRoutines--
			if nRoutines == 0 {
				close(filenameChan)
				continue
			}
		default:
			logger.Fatal("You may only traverse the bloom folder")
		}
	}

	return nil
}

// TODO: This should accept unresolved block ranges, not lists of block numbers
// This routine accepts the 'resolved' block numbers. If, instead, it it received the unresolved block ranges
// from the command line, it would be much more efficient. Using resolved block numbers means we have to
// provide a block range that is fine-grained enough to hit on every file inside the range. For example, if
// there are 100 files in the range 100000-200000, we need to create block numbers that cover every
// eventuallity. If on of the files has a two block range, we need to generate 50,000 block numbers. If we
// used the range on the command line instead we'd only have to intersect one range.

func (walker *CacheWalker) shouldDisplay(result walk.CacheFileInfo, cnt int, blockNums []uint64) bool {
	if !walk.IsCacheType(result.Path, result.Type, true /* checkExt */) {
		return false
	}

	if walker.testMode && cnt > walker.maxTests {
		return false
	}

	if len(blockNums) == 0 {
		return true
	}

	hit := false
	for _, bn := range blockNums {
		h := result.Range.IntersectsB(bn)
		hit = hit || h
		if hit {
			break
		}
	}

	return hit
}
