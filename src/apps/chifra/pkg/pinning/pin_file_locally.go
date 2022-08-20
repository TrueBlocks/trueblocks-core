package pinning

import (
	"os"

	shell "github.com/ipfs/go-ipfs-api"
)

func (p *Service) pinFileLocally(filepath string) (string, error) {
	file, err := os.Open(filepath)
	if err != nil {
		return "", err
	}
	defer file.Close()

	sh := shell.NewShell("localhost:5001")
	cid, err := sh.Add(file, shell.Pin(true))
	if err != nil {
		return "", err
	}
	return cid, nil
}
