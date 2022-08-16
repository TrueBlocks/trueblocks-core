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

func (s *Service) pinFileRemotely(filepath string) (string, error) {
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

	req, err := http.NewRequest(http.MethodPost, s.PinUrl, r)
	if err != nil {
		return "", err
	}

	if s.HeaderFunc != nil {
		headers := s.HeaderFunc(s, m.FormDataContentType())
		for key, value := range headers {
			req.Header.Add(key, value)
		}
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

	if out, castOk := dat["error"].(string); castOk {
		return "", fmt.Errorf(out)
	}

	if ipfsHash, castOk := dat[s.ResultName].(string); castOk {
		return ipfsHash, nil
	}

	return "", fmt.Errorf("Pin file to Pinata failure.")
}
