package pinning

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
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

func NewPinningService(chain string, which ServiceType) (Service, error) {
	pinataKey, pinataSecret := config.GetKey("pinata").ApiKey, config.GetKey("pinata").Secret
	switch which {
	case Local:
		return Service{}, nil
	case Pinata:
		return Service{
			PinUrl:     config.PINATA_URL,
			Apikey:     pinataKey,
			Secret:     pinataSecret,
			ResultName: "IpfsHash",
			HeaderFunc: pinataHeaders,
		}, nil
	default:
		return Service{}, fmt.Errorf("unknown service type %d", which)
	}
}

func pinataHeaders(s *Service, contentType string) map[string]string {
	headers := make(map[string]string)
	headers["Content-Type"] = contentType
	headers["pinata_secret_api_key"] = s.Secret
	headers["pinata_api_key"] = s.Apikey
	return headers
}
