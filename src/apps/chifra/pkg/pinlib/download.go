package pinlib

import (
	"errors"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
)

// func fetch(gatewayUrl string) (*http.Response, error) {
// 	response, err := http.Get(gatewayUrl)

// 	if err != nil {
// 		return nil, err
// 	}

// 	return response, nil
// }

// func

func DownloadManifest(gatewayUrl string) (*manifest.Manifest, error) {
	response, err := http.Get(gatewayUrl)

	if err != nil {
		return nil, err
	}

	switch response.Header.Get("content-type") {
	case "text/tab-separated-values":
		return manifest.ReadTabManifest(response.Body)
	case "application/json":
		return manifest.ReadJSONManifest(response.Body)
	default:
		return nil, errors.New("unrecognized content type")
	}
}
