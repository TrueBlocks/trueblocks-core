package pinning

import (
	cid "github.com/ipfs/go-cid"
)

func IsValid(needle string) bool {
	_, err := cid.Decode(needle)
	return err == nil
}
