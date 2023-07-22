package pinning

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	shell "github.com/ipfs/go-ipfs-api"
)

func (s *Service) pinFileLocally(filepath string) (base.IpfsHash, error) {
	file, err := os.OpenFile(filepath, os.O_RDONLY, 0)
	if err != nil {
		return "", err
	}
	defer file.Close()

	// TODO: should be configurable (see #2804)
	sh := shell.NewShell("localhost:5001")
	cid, err := sh.Add(file, shell.Pin(true))
	if err != nil {
		return "", err
	}
	return base.IpfsHash(cid), nil
}
