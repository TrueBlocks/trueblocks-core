package pinning

import (
	"fmt"
	"net/http"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var deletePin = "https://api.pinata.cloud/pinning/unpin/%s"

// unpinPin unpins a pin
func unpinPin(chain string, i int, hash base.IpfsHash) error {
	url := fmt.Sprintf(deletePin, hash.String())
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
				logger.Info("Deleted pin", i, hash.String())
			}
			return nil
		}
	}
}

func DeletePins() error {
	lines := file.AsciiFileToLines("unpins.txt")
	for i, line := range lines {
		if err := unpinPin("sepolia", i, base.IpfsHash(line)); err != nil {
			return err
		}
		time.Sleep(time.Second)
	}
	return nil
}
