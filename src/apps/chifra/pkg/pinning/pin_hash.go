package pinning

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"time"
)

const (
	PIN_HASH_URL = "https://api.pinata.cloud/pinning/pinByHash"
)

func (p *PinningService) PinHash(hash string) (bool, error) {
	if hash == "" {
		return false, fmt.Errorf("HASH: %s is invalid.", hash)
	}

	jsonValue, _ := json.Marshal(map[string]string{"hashToPin": hash})

	req, err := http.NewRequest(http.MethodPost, PIN_HASH_URL, bytes.NewBuffer(jsonValue))
	req.Header.Set("Content-Type", "application/json")
	req.Header.Add("pinata_secret_api_key", p.Secret)
	req.Header.Add("pinata_api_key", p.Apikey)

	client := &http.Client{
		Timeout: 30 * time.Second,
	}
	resp, err := client.Do(req)
	if err != nil {
		return false, err
	}
	defer resp.Body.Close()

	data, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return false, err
	}

	var dat map[string]interface{}
	if err := json.Unmarshal(data, &dat); err != nil {
		return false, err
	}

	if h, ok := dat["ipfsHash"].(string); ok {
		return h == hash, nil
	}

	return false, fmt.Errorf("Pin hash to Pinata failure.")
}
