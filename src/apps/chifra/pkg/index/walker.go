package index

import (
	"log"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
)

type walkerFunc func(walker *IndexWalker, path string, first bool) (bool, error)

type IndexWalker struct {
	chain      string
	testMode   bool
	maxTests   int
	visitFunc1 walkerFunc
}

func NewIndexWalker(chain string, testMode bool, maxTests int, visitFunc walkerFunc) IndexWalker {
	return IndexWalker{
		chain:      chain,
		testMode:   testMode,
		maxTests:   maxTests,
		visitFunc1: visitFunc,
	}
}

func (walker *IndexWalker) MaxTests() int {
	return walker.maxTests
}

func (walker *IndexWalker) WalkBloomFilters(blockNums []uint64) error {
	filenameChan := make(chan paths.IndexFileInfo)

	var nRoutines int = 1
	go paths.WalkIndexFolder(walker.chain, paths.Index_Bloom, filenameChan)

	cnt := 0
	for result := range filenameChan {
		switch result.Type {
		case paths.Index_Bloom:
			skip := (walker.testMode && cnt > walker.maxTests) || !strings.HasSuffix(result.Path, ".bloom")
			if !skip && shouldDisplay(result, blockNums) {
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
		case paths.None:
			nRoutines--
			if nRoutines == 0 {
				close(filenameChan)
				continue
			}
		default:
			log.Fatal("You may only traverse the bloom folder")
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
func shouldDisplay(result paths.IndexFileInfo, blockNums []uint64) bool {
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
