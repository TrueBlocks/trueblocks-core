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
	Estuary
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
	ESTUARY_URL = "https://shuttle-4.estuary.tech/content/add"
	PINATA_URL  = "https://api.pinata.cloud/pinning/pinFileToIPFS"
)

func NewPinningService(chain string, which ServiceType) (Service, error) {
	pinataKey, pinataSecret, estuaryKey := config.GetPinningKeys(chain)

	switch which {
	case Local:
		return Service{}, nil
	case Estuary:
		return Service{
			PinUrl:     ESTUARY_URL,
			Apikey:     estuaryKey,
			ResultName: "cid",
			HeaderFunc: EstuaryHeaders,
		}, nil
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

func EstuaryHeaders(s *Service, contentType string) map[string]string {
	headers := make(map[string]string)
	headers["Content-Type"] = contentType
	headers["Accept"] = "application/json"
	headers["Authorization"] = "Bearer " + s.Apikey
	return headers
}

// TODO: should be configurable (see #2804)
// export EST_UPLOAD_HOST=https://shuttle-4.estuary.tech
// export ESTUARY_TOKEN=<MYTOKEN>
// export EST_SAMPLE_LARGE_FILE=/Users/jrush/Development/estuary/main.go
// curl -X POST $EST_UPLOAD_HOST/content/add -H "Authorization: Bearer $ESTUARY_TOKEN" -H "Accept: application/json" -H "Content-Type: multipart/form-data" -F "data=@$EST_SAMPLE_LARGE_FILE" | jq
// {
//   "cid": "bafkreidikz2w6trfvyukqw5a5jchl7xzkpbvvraqci6ivb2q3klhf5asrm",
//   "estuaryId": 35421618,
//   "providers": [
//     "/ip4/3.134.223.177/tcp/6745/p2p/12D3KooWN8vAoGd6eurUSidcpLYguQiGZwt4eVgDvbgaS7kiGTup"
//   ]
// }
