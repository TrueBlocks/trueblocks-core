package pinning

import (
	"strings"

	shell "github.com/ipfs/go-ipfs-api"
)

type Service struct {
	Apikey     string
	Secret     string
	PinUrl     string
	ResultName string
	HeaderFunc func(s *Service, contentType string) map[string]string
}

func LocalDaemonRunning() bool {
	sh := shell.NewShell("localhost:5001")
	_, err := sh.Add(strings.NewReader("hello world!"))
	return err == nil
}
