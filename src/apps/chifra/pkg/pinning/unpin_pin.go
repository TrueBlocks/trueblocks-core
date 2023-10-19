package pinning

import (
	"fmt"
	"net/http"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var unpinPin = "https://api.pinata.cloud/pinning/unpin/%s"

// unpinPin unpins a pin
func unpinOne(chain string, i int, hash base.IpfsHash) error {
	url := fmt.Sprintf(unpinPin, hash.String())
	if req, err := http.NewRequest("DELETE", url, nil); err != nil {
		return err
	} else {
		s, _ := NewService(chain, Pinata)
		if s.HeaderFunc != nil {
			headers := s.HeaderFunc(&s, "application/json")
			for key, value := range headers {
				req.Header.Add(key, value)
			}
		}
		if res, err := http.DefaultClient.Do(req); err != nil {
			return err
		} else {
			if res.StatusCode != 200 {
				logger.Error("Error deleting pin", hash, res.StatusCode)
			} else {
				logger.Info("Unpinned", i, hash.String())
			}
			return nil
		}
	}
}

func Unpin(chain string) error {
	lines := file.AsciiFileToLines("./unpins")
	for i, line := range lines {
		if IsValid(line) {
			if err := unpinOne(chain, i, base.IpfsHash(line)); err != nil {
				return err
			}
			time.Sleep(time.Second * 3)
		}
	}
	return nil
}
