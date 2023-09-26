package pinning

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	shell "github.com/ipfs/go-ipfs-api"
)

type ServiceType int

const (
	NoType ServiceType = 1 << iota
	Pinata
	Local
)

type Service struct {
	Apikey     string
	Secret     string
	PinUrl     string
	ResultName string
	HeaderFunc func(s *Service, contentType string) map[string]string
}

func LocalDaemonRunning() bool {
	// TODO: should be configurable (see #2804)
	sh := shell.NewShell("localhost:5001")
	_, err := sh.Add(strings.NewReader("hello world!"))
	return err == nil
}

// TODO: should be configurable (see #2804)
const (
	PINATA_URL = "https://api.pinata.cloud/pinning/pinFileToIPFS"
)

func NewPinningService(chain string, which ServiceType) (Service, error) {
	pinataKey, pinataSecret := config.GetKey("pinata").ApiKey, config.GetKey("pinata").Secret
	switch which {
	case Local:
		return Service{}, nil
	case Pinata:
		return Service{
			PinUrl:     PINATA_URL,
			Apikey:     pinataKey,
			Secret:     pinataSecret,
			ResultName: "IpfsHash",
			HeaderFunc: PinataHeaders,
		}, nil
	default:
		return Service{}, fmt.Errorf("unknown service type %d", which)
	}
}

func PinataHeaders(s *Service, contentType string) map[string]string {
	headers := make(map[string]string)
	headers["Content-Type"] = contentType
	headers["pinata_secret_api_key"] = s.Secret
	headers["pinata_api_key"] = s.Apikey
	return headers
}
