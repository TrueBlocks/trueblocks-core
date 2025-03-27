package pinning

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

type ServiceType int

const (
	NoType ServiceType = iota
	Pinata
	Local
)

type Service struct {
	Apikey     string
	Secret     string
	Jwt        string
	HeaderFunc func(s *Service, contentType string) map[string]string
}

func NewService(chain string, serviceType ServiceType) (Service, error) {
	_ = chain
	apiKey, secret, jwt := config.GetKey("pinata").ApiKey, config.GetKey("pinata").Secret, config.GetKey("pinata").Jwt
	switch serviceType {
	case Local:
		return Service{}, nil
	case Pinata:
		return Service{
			Apikey:     apiKey,
			Secret:     secret,
			Jwt:        jwt,
			HeaderFunc: pinataHeaders,
		}, nil
	default:
		return Service{}, fmt.Errorf("unknown pinning service type %d", serviceType)
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
