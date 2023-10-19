package pinning

import (
	"encoding/json"
	"fmt"
	"net/http"
	"os"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var listPins = "https://api.pinata.cloud/data/pinList?status=%s&includesCount=true&pageOffset=%d&pageLimit=%d"
var deletePin = "https://api.pinata.cloud/pinning/unpin/%s"

func getPins(chain, status string, first, cnt int) (int, []Pin) {
	url := fmt.Sprintf(listPins, status, first, cnt)
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

var fetchCount = 1000

// ListPins pins a file remotely to the pinning service
func ListPins(chain string) ([]string, error) {
	// return []string{}, DeletePins()
	count, _ := getPins(chain, "pinned", 0, 1)
	for i := 0; i < count; i += fetchCount {
		count, pins := getPins(chain, "pinned", i, fetchCount)
		for _, pin := range pins {
			fmt.Println(pin.IpfsPinHash, pin.DatePinned, pin.Metadata.Name)
		}
		if count < fetchCount {
			break
		}
		logger.Info("Sleeping...", count, i, fetchCount)
		time.Sleep(time.Second)
	}

	os.Exit(0)
	return []string{}, fmt.Errorf("not implemented")
}

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

// func main() {
// 	req, _ := http.NewRequest("DELETE", url, nil)
// 	req.Header.Add("accept", "application/json")
// 	res, _ := http.DefaultClient.Do(req)
// 	defer res.Body.Close()
// 	body, _ := io.ReadAll(res.Body)
// 	fmt.Println(string(body))
// }
