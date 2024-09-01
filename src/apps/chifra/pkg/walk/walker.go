package walk

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type walkerFunc func(walker *CacheWalker, path string, first bool) (bool, error)
type ForEveryFunc func(path string, vP any) (bool, error)

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

func ForEveryFileInFolder(path string, forEvery ForEveryFunc, vP any) error {
	visitFunc := func(walker *CacheWalker, path string, first bool) (bool, error) {
		return forEvery(path, vP)
	}
	walker := NewCacheWalker("", false, int(base.NOPOSI), visitFunc)
	return walker.WalkRegularFolder(path)
}

func (walker *CacheWalker) WalkRegularFolder(path string) error {
	filenameChan := make(chan CacheFileInfo)

	var nRoutines int = 1
	go WalkFolder(context.Background(), path, nil, filenameChan)

	cnt := 0
	for result := range filenameChan {
		switch result.Type {
		case Regular:
			ok, err := walker.visitFunc1(walker, result.Path, cnt == 0)
			if err != nil {
				return err
			}
			if ok {
				cnt++
			} else {
				return nil
			}
		case Cache_NotACache:
			nRoutines--
			if nRoutines == 0 {
				close(filenameChan)
				continue
			}
		default:
			logger.Fatal("should not happen ==> in WalkRegularFolder")
		}
	}

	return nil
}

func (walker *CacheWalker) WalkBloomFilters(blockNums []base.Blknum) error {
	filenameChan := make(chan CacheFileInfo)

	// TODO: changing this will probably create data races because we append to slices and/or modify maps
	// in the visitFunc. We need to make sure that we don't modify the same data structure in two different
	// goroutines.
	var nRoutines int = 1
	go WalkCacheFolder(context.Background(), walker.chain, Index_Bloom, nil, filenameChan)

	cnt := 0
	for result := range filenameChan {
		switch result.Type {
		case Index_Bloom:
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
		case Cache_NotACache:
			nRoutines--
			if nRoutines == 0 {
				close(filenameChan)
				continue
			}
		default:
			logger.Fatal("should not happen ==> you may only traverse the bloom folder")
		}
	}

	return nil
}

// TODO: This should accept unresolved block ranges, not lists of block numbers
// This routine accepts the 'resolved' block numbers. If, instead, it received the unresolved block ranges
// from the command line, it would be much more efficient. Using resolved block numbers means we have to
// provide a block range that is fine-grained enough to hit on every file inside the range. For example, if
// there are 100 files in the range 100000-200000, we need to create block numbers that cover every
// eventuallity. If on of the files has a two block range, we need to generate 50,000 block numbers. If we
// used the range on the command line instead we'd only have to intersect one range.

func (walker *CacheWalker) shouldDisplay(result CacheFileInfo, cnt int, blockNums []base.Blknum) bool {
	if !IsCacheType(result.Path, result.Type, true /* checkExt */) {
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
		h := result.FileRange.IntersectsB(bn)
		hit = hit || h
		if hit {
			break
		}
	}

	return hit
}
