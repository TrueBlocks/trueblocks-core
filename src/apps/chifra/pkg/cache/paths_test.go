package cache

import (
	"testing"
)

// TODO(cache): Just noting that eventually this will be expanded to all cache paths.
func Test_getPathByBlock(t *testing.T) {
	path := getPathByBlock(Cache_Blocks, 92590)
	if path != "blocks/00/00/92/000092590.bin" {
		t.Fatal("wrong result", path)
	}
}
