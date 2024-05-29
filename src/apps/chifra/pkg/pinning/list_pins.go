package pinning

import (
	"encoding/json"
	"fmt"
	"net/http"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/debug"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var listPins = "https://api.pinata.cloud/data/pinList?status=%s&includesCount=true&pageOffset=%d&pageLimit=%d"

func getPins(chain, status string, first, cnt int) (int, []Pin) {
	url := fmt.Sprintf(listPins, status, first, cnt)

	debug.DebugCurlStr(url)
	if req, err := http.NewRequest("GET", url, nil); err != nil {
		return 0, []Pin{}
	} else {
		s, _ := NewService(chain, Pinata)
		if s.HeaderFunc != nil {
			headers := s.HeaderFunc(&s, "application/json")
			for key, value := range headers {
				req.Header.Add(key, value)
			}
		}
		if res, err := http.DefaultClient.Do(req); err != nil {
			return 0, []Pin{}
		} else {
			defer res.Body.Close()
			data := PinSet{}
			decoder := json.NewDecoder(res.Body)
			if err = decoder.Decode(&data); err != nil {
				return 0, []Pin{}
			}
			return data.Count, data.Rows
		}
	}
}

// ListPins pins a file remotely to the pinning service
func ListPins(chain, status string, showProgress, countOnly bool, perPage int, dur time.Duration) ([]string, error) {
	count, _ := getPins(chain, status, 0, 1)
	ret := make([]string, 0, count)
	if countOnly {
		logger.Info("There are", count, "pins.")

	} else {
		testing := false
		if perPage < 0 {
			perPage = -1 * perPage
			testing = true
		}

		bar := logger.NewBar(logger.BarOptions{
			Enabled: showProgress,
			Total:   int64(count)/int64(perPage) + 1,
		})

		for i := 0; i < count; i += perPage {
			count, pins := getPins(chain, status, i, perPage)
			bar.Prefix = fmt.Sprintf("Listing %d '%s' items in %d pages...", count, status, count/perPage+1)
			for _, pin := range pins {
				ret = append(ret, fmt.Sprintf("%s\t%s\t%s\t%d\t%s", pin.IpfsPinHash, pin.DatePinned, pin.Metadata.Name, pin.Size, status))
			}
			if testing || count < perPage {
				break
			}
			bar.Tick()
			time.Sleep(dur)
		}
		bar.Finish(true /* newLine */)
	}
	return ret, nil
}

// Pin data type carries information about remote pins
type Pin struct {
	Id           string        `json:"id"`
	IpfsPinHash  base.IpfsHash `json:"ipfs_pin_hash"`
	Size         int           `json:"size"`
	UserId       string        `json:"user_id"`
	DatePinned   string        `json:"date_pinned"`
	DateUnpinned string        `json:"date_unpinned"`
	Metadata     struct {
		Name      string      `json:"name"`
		Keyvalues interface{} `json:"keyvalues"`
	} `json:"metadata"`
	Regions []struct {
		RegionId                string `json:"regionId"`
		CurrentReplicationCount int    `json:"currentReplicationCount"`
		DesiredReplicationCount int    `json:"desiredReplicationCount"`
	} `json:"regions"`
	MimeType      string `json:"mime_type"`
	NumberOfFiles int    `json:"number_of_files"`
}

type PinSet struct {
	Count int   `json:"count"`
	Rows  []Pin `json:"rows"`
}
