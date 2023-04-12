// IPFS Client code https://pkg.go.dev/github.com/ipfs/go-ipfs-http-client for pinning directly

package pinning

import (
	"encoding/json"
	"fmt"
	"io"
	"mime/multipart"
	"net/http"
	"os"
	fp "path/filepath"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func (s *Service) pinFileRemotely(filepath string) (base.IpfsHash, error) {
	if s.PinUrl == "" {
		return "", fmt.Errorf("empty remote pinning URL")
	}

	if s.HeaderFunc == nil {
		return "", fmt.Errorf("header function is nil")
	}

	file, err := os.OpenFile(filepath, os.O_RDONLY, 0)
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

	data, err := io.ReadAll(resp.Body)
	if err != nil {
		return "", err
	}

	var dat = struct {
		IpfsHash string
		PinSize  int64
		Date     string
	}{}

	if err := json.Unmarshal(data, &dat); err != nil {
		return "", err
	}
	return base.IpfsHash(dat.IpfsHash), nil
}
