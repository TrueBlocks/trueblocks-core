package pinning

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	shell "github.com/ipfs/go-ipfs-api"
)

func (p *Service) pinFileLocally(filepath string) (base.IpfsHash, error) {
	file, err := os.OpenFile(filepath, os.O_RDONLY, 0)
	if err != nil {
		return "", err
	}
	defer file.Close()

	sh := shell.NewShell("localhost:5001")
	cid, err := sh.Add(file, shell.Pin(true))
	if err != nil {
		return "", err
	}
	return base.IpfsHash(cid), nil
}
