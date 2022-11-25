package cache

import (
	"testing"
)

// TODO: This can be expanded to all cache paths?
func Test_getPathByBlock(t *testing.T) {
	path := getPathByBlock(ItemBlock, 92590)
	if path != "blocks/00/00/92/000092590.bin" {
		t.Fatal("wrong result", path)
	}
}
