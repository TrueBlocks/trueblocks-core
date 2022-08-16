package pinning

import (
	"encoding/json"
	"fmt"
	"io"
	"io/ioutil"
	"mime/multipart"
	"net/http"
	"os"
	fp "path/filepath"
	"time"
)

const (
	PIN_FILE_URL = "https://api.pinata.cloud/pinning/pinFileToIPFS"
)

func (p *PinningService) pinFileRemotely(filepath string) (string, error) {
	file, err := os.Open(filepath)
	if err != nil {
		return "", err
	}
	defer file.Close()

	r, w := io.Pipe()
	m := multipart.NewWriter(w)

	go func() {
		defer w.Close()
		defer m.Close()

		part, err := m.CreateFormFile("file", fp.Base(file.Name()))
		if err != nil {
			return
		}

		if _, err = io.Copy(part, file); err != nil {
			return
		}
	}()

	client := &http.Client{
		Timeout: 30 * time.Second,
	}

	req, err := http.NewRequest(http.MethodPost, PIN_FILE_URL, r)
	if err != nil {
		return "", err
	}

	for key, value := range p.Headers(m.FormDataContentType()) {
		req.Header.Add(key, value)
	}

	resp, err := client.Do(req)
	if err != nil {
		return "", err
	}
	defer resp.Body.Close()

	data, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return "", err
	}

	var dat map[string]interface{}
	if err := json.Unmarshal(data, &dat); err != nil {
		return "", err
	}

	if out, pass := dat["error"].(string); pass {
		return "", fmt.Errorf(out)
	}

	if hash, ok := dat["IpfsHash"].(string); ok {
		return hash, nil
	}
	return "", fmt.Errorf("Pin file to Pinata failure.")
}
