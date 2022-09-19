package tslib

import (
	"errors"
	"io"
	"net/http"
	"net/url"
	"os"
	"path"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
)

func EstablishTsFile(chain string) error {
	if chain == "non-tracing" { // A test-mode chain to test IsTracing
		return nil
	}

	tsPath := config.GetPathToIndex(chain) + "ts.bin"
	if file.FileExists(tsPath) {
		return nil
	}

	cid, err := manifest.ReadUnchainIndex(chain, "ts")
	if err != nil {
		return err
	}

	return downloadCidToBinary(chain, cid, tsPath)
}

// downloadCidToBinary downloads a timestamp database file
func downloadCidToBinary(chain, cid, fileName string) error {
	gatewayUrl := config.GetIpfsGateway(chain)

	url, err := url.Parse(gatewayUrl)
	if err != nil {
		return err
	}
	url.Path = path.Join(url.Path, cid)

	logger.Log(logger.InfoC, "Chain:", chain)
	logger.Log(logger.InfoC, "Gateway:", gatewayUrl)
	logger.Log(logger.InfoC, "CID:", cid)
	logger.Log(logger.InfoC, "URL:", url.String())
	msg := colors.Yellow + "Downloading timestamp file. This may take a moment, please wait..." + colors.Off
	logger.Log(logger.Warning, msg)

	//Get the response bytes from the url
	response, err := http.Get(url.String())
	if err != nil {
		return err
	}
	defer response.Body.Close()

	if response.StatusCode != 200 {
		return errors.New("received non-200 response code")
	}

	ff, err := os.Create(fileName)
	if err != nil {
		return err
	}
	defer ff.Close()

	_, err = io.Copy(ff, response.Body)
	if err != nil {
		return err
	}

	return nil
}
