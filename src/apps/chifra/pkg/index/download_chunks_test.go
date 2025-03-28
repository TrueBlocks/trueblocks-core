// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package index

// func Test_exclude(t *testing.T) {
// 	onDisc := map[string]bool{
// 		"013337527-013340418": true,
// 		"013340419-013343305": true,
// 		"013346064-013348861": true,
// 		"013348862-013351760": true,
// 	}

// 	inManifest := []types.ChunkRecord{
// 		{
// 			Range: "013337527-013340418",
// 		},
// 		{
// 			Range: "013340419-013343305",
// 		},
// 		{
// 			Range: "013346064-013348861",
// 		},
// 		{
// 			Range: "013348862-013351760",
// 		},
// 		{
// 			Range: "013387069-013389874",
// 		},
// 		{
// 			Range: "013389875-013392800",
// 		},
// 	}

// 	result := exclude(utils.GetTestChain(), c ache.Index_Final, onDisc, inManifest, nil)
// 	if len(result) != 2 {
// 		t.Errorf("Wrong length: %d", len(result))
// 	}

// 	if result[0].Range != "013387069-013389874" &&
// 		result[1].Range != "013389875-013392800" {
// 		t.Errorf("Bad values: '%s' and '%s'", result[0].Range, result[1].Range)
// 	}
// }
