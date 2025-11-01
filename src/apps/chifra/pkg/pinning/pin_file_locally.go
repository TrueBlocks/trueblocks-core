package pinning

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	shell "github.com/ipfs/go-ipfs-api"
)

// pinFileLocally pins the file locally to the local IPFS node
func (s *Service) pinFileLocally(chain, filepath string) (base.IpfsHash, error) {
	_ = chain
	file, err := os.OpenFile(filepath, os.O_RDONLY, 0)
	if err != nil {
		return "", err
	}
	defer file.Close()

	sh := shell.NewShell(config.GetPinning().LocalPinUrl)
	cid, err := sh.Add(file, shell.Pin(true))
	if err != nil {
		return "", err
	}
	return base.IpfsHash(cid), nil
}
