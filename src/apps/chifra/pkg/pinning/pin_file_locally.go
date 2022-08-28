package pinning

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	shell "github.com/ipfs/go-ipfs-api"
)

func (p *Service) pinFileLocally(filepath string) (types.IpfsHash, error) {
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
	return types.IpfsHash(cid), nil
}
