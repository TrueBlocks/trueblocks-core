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
	Jwt        string
	ResultName string
	HeaderFunc func(s *Service, contentType string) map[string]string
}

func NewPinningService(chain string, which ServiceType) (Service, error) {
	apiKey, secret, jwt := config.GetKey("pinata").ApiKey, config.GetKey("pinata").Secret, config.GetKey("pinata").Jwt
	switch which {
	case Local:
		return Service{}, nil
	case Pinata:
		return Service{
			Apikey:     apiKey,
			Secret:     secret,
			Jwt:        jwt,
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
	if s.Secret != "" {
		headers["pinata_secret_api_key"] = s.Secret
		headers["pinata_api_key"] = s.Apikey
	} else {
		headers["authorization"] = "Bearer " + s.Jwt
	}
	return headers
}
