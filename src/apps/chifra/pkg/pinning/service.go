package pinning

import (
	"strings"

	shell "github.com/ipfs/go-ipfs-api"
)

type PinningService struct {
	Local  bool
	Apikey string
	Secret string
}

func (s *PinningService) Headers(contentType string) map[string]string {
	headers := make(map[string]string)
	headers["Content-Type"] = contentType
	headers["pinata_secret_api_key"] = s.Secret
	headers["pinata_api_key"] = s.Apikey
	return headers
}

func LocalDaemonRunning() bool {
	sh := shell.NewShell("localhost:5001")
	_, err := sh.Add(strings.NewReader("hello world!"))
	return err == nil
}
