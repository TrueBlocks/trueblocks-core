package chunksPkg

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"

func (opts *ChunksOptions) HandleRepairManifest(blockNums []uint64) error {
	logger.Log(logger.Error, "HandleRepairManifest is not implemented.")
	return nil
}

// TODO: BOGUS - PINNING TO PINATA AND WRITING MANIFEST FILE SHOULD BE ATOMIC AND PROTECTED FROM CANCEL
func (opts *ChunksOptions) repairManifest(ctx *WalkContext, path string, first bool) (bool, error) {
	// man, err := manifest.ReadManifest(opts.Globals.Chain, manifest.FromCache)
	// if err != nil {
	// 	return false, err
	// }

	// reports := []types.ReportRepair{}
	// for _, chunk := range man.Chunks {
	// 	rng, _ := cache.RangeFromFilename(chunk.Range + ".")
	// 	if rng.IntersectsB(blockNums[0]) {
	// 		report := types.ReportRepair{
	// 			Status:    "Repaired Bloom",
	// 			Range:     config.GetPathToIndex(opts.Globals.Chain) + "blooms/" + chunk.Range + ".bloom",
	// 			BloomHash: string(chunk.BloomHash),
	// 		}
	// 		reports = append(reports, report)
	// 		report = types.ReportRepair{
	// 			Status:    "Repaired Index",
	// 			Range:     config.GetPathToIndex(opts.Globals.Chain) + "finalized/" + chunk.Range + ".bin",
	// 			IndexHash: string(chunk.IndexHash),
	// 		}
	// 		reports = append(reports, report)
	// 	}
	// }

	// return true, globals.RenderSlice(&opts.Globals, reports)
	return true, nil
}
