package pinning

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"io/ioutil"
	"mime/multipart"
	"net/http"
	"os"
	fp "path/filepath"
	"strings"
	"time"

	shell "github.com/ipfs/go-ipfs-api"
)

const (
	PIN_FILE_URL = "https://api.pinata.cloud/pinning/pinFileToIPFS"
	PIN_HASH_URL = "https://api.pinata.cloud/pinning/pinByHash"
)

type Service struct {
	Local  bool
	Apikey string
	Secret string
}

func (p *Service) PinFile(filepath string) (string, error) {
	file, err := os.Open(filepath)
	if err != nil {
		return "", err
	}
	defer file.Close()

	if p.Local {
		sh := shell.NewShell("localhost:5001")
		cid, err := sh.Add(file)
		if err != nil {
			return "", err
		}
		return cid, nil
	}

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

	req, err := http.NewRequest(http.MethodPost, PIN_FILE_URL, r)
	req.Header.Add("Content-Type", m.FormDataContentType())
	req.Header.Add("pinata_secret_api_key", p.Secret)
	req.Header.Add("pinata_api_key", p.Apikey)

	client := &http.Client{
		Timeout: 30 * time.Second,
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

	if out, err := dat["error"].(string); err {
		return "", fmt.Errorf(out)
	}
	if hash, ok := dat["IpfsHash"].(string); ok {
		return hash, nil
	}

	return "", fmt.Errorf("Pin file to Pinata failure.")
}

func (p *Service) PinHash(hash string) (bool, error) {
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

func LocalDaemonRunning() bool {
	sh := shell.NewShell("localhost:5001")
	_, err := sh.Add(strings.NewReader("hello world!"))
	return err == nil
}
