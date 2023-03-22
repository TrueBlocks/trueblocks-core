package cache

import (
	"testing"
)

// TODO(cache): Just noting that eventually this will be expanded to all cache paths.
func Test_getPathByBlock(t *testing.T) {
	path := getPathByBlock(ItemBlock, 92590)
	if path != "blocks/00/00/92/000092590.bin" {
		t.Fatal("wrong result", path)
	}
}

func TestCacheLayout(t *testing.T) {
	// TODO: turn these back on if possible
	// indexPath := config.GetPathToIndex(utils.GetTestChain())
	// cachePath := config.GetPathToCache(utils.GetTestChain())

	// tests := []struct {
	// 	on        bool
	// 	name      string
	// 	cacheType CacheType
	// 	param     string
	// 	expected  CachePath12
	// 	path      string
	// 	wantErr   bool
	// }{
	// 	{
	// 		on:    true,
	// 		name:  "index chunk path",
	// 		param: "0010000000-0010200000",
	// 		expected: CachePath12{
	// 			Type:      Index_Final,
	// 			RootPath:  indexPath,
	// 			Subdir:    "finalized/",
	// 			Extension: ".bin",
	// 		},
	// 		path:    "finalized/0010000000-0010200000.bin",
	// 		wantErr: false,
	// 	},
	// 	{
	// 		on:    true,
	// 		name:  "Bloom filter path",
	// 		param: "0010000000-0010200000",
	// 		expected: CachePath12{
	// 			Type:      Index_Bloom,
	// 			RootPath:  indexPath,
	// 			Subdir:    "blooms/",
	// 			Extension: ".bloom",
	// 		},
	// 		path:    "blooms/0010000000-0010200000.bloom",
	// 		wantErr: false,
	// 	},
	// 	{
	// 		on:    false,
	// 		name:  "Block cache path",
	// 		param: "001001001",
	// 		expected: CachePath12{
	// 			Type:      Cache_Block,
	// 			RootPath:  cachePath,
	// 			Subdir:    "blocks/",
	// 			Extension: ".bin",
	// 		},
	// 		path:    "blocks/00/10/01/001001001.bin",
	// 		wantErr: false,
	// 	},
	// 	{
	// 		on:    false,
	// 		name:  "Transaction cache path",
	// 		param: "1001001.20",
	// 		expected: CachePath12{
	// 			Type:      Cache_Tx,
	// 			RootPath:  cachePath,
	// 			Subdir:    "txs/",
	// 			Extension: ".bin",
	// 		},
	// 		path:    "txs/00/10/01/001001001-00020.bin",
	// 		wantErr: false,
	// 	},
	// 	// TraceFn:      $CACHE_PATH/traces/00/10/01/001001001-00020-10.bin
	// 	// NeighborFn:   $CACHE_PATH/neighbors/00/10/01/001001001-00020.bin
	// 	// ReconFn:      $CACHE_PATH/recons/c011/a724/00e58ecd99ee497cf89e3775d4bd732f/000000012.00013.bin
	// }

	// for _, tt := range tests {
	// 	if !tt.on {
	// 		continue
	// 	}

	// 	t.Run(tt.name, func(t *testing.T) {
	// 		cachePath := NewCachePath(utils.GetTestChain(), tt.expected.Type)
	// 		if cachePath.Extension != tt.expected.Extension {
	// 			t.Error("Wrong extension", cachePath.Extension)
	// 		}
	// 		if cachePath.Subdir != tt.expected.Subdir {
	// 			t.Error("Wrong subdir", cachePath.Subdir)
	// 		}
	// 		p := cachePath.GetFullPath(tt.param)
	// 		if p != filepath.Join(tt.expected.RootPath, tt.path) {
	// 			t.Error("Wrong full path", p)
	// 		}
	// 	})
	// }
}
