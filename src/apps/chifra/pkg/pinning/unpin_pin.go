package pinning

import (
	"fmt"
	"net/http"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/debug"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var unpinPin = "https://api.pinata.cloud/pinning/unpin/%s"

// unpinOne unpins a pin
func unpinOne(chain string, i, total int, hash base.IpfsHash) error {
	url := fmt.Sprintf(unpinPin, hash.String())

	debug.DebugCurlStr(url)
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
				logger.Info("Unpinned", i, "of", total, hash.String())
			}
			return nil
		}
	}
}

func Unpin(chain string, count bool, sleep float64) error {
	lines := file.AsciiFileToLines("./unpins")
	if count {
		logger.Info("There are", len(lines), "pins to unpin.")
	} else {
		for i, line := range lines {
			if IsValid(line) {
				if err := unpinOne(chain, i, len(lines), base.IpfsHash(line)); err != nil {
					return err
				}
				if sleep > 0 {
					ms := time.Duration(sleep*1000) * time.Millisecond
					// logger.Info(fmt.Sprintf("Sleeping for %g seconds", sleep))
					time.Sleep(ms)
				}
			}
		}
	}
	return nil
}
