package tslib

import (
	"context"
	"fmt"
	"io"
	"net/http"
	"net/url"
	"os"
	"path/filepath"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/sigintTrap"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
)

func EstablishTsFile(chain string) error {
	if chain == "non-tracing" { // A test-mode chain to test IsTracing
		return nil
	}

	tsPath := config.PathToTimestamps(chain)
	if file.FileExists(tsPath) {
		return nil
	}

	cid, err := manifest.ReadUnchainedIndex(chain, "ts", unchained.GetPreferredPublisher())
	if err != nil {
		return err
	}

	return downloadCidToBinary(chain, cid, tsPath)
}

// downloadCidToBinary downloads a CID to a binary file
func downloadCidToBinary(chain, cid, outputFn string) error {
	gatewayUrl := config.GetIpfsGateway(chain)

	url, err := url.Parse(gatewayUrl)
	if err != nil {
		return err
	}
	url.Path = filepath.Join(url.Path, cid)

	logger.InfoTable("Chain:", chain)
	logger.InfoTable("Gateway:", gatewayUrl)
	logger.InfoTable("URL:", url.String())
	logger.InfoTable("CID:", cid)
	logger.Info(fmt.Sprintf("%sDownloading database %s (%s). This may take a moment...%s", colors.Yellow, "database", cid, colors.Off))

	header, err := http.Head(url.String())
	if err != nil {
		return err
	}
	if header.StatusCode != 200 {
		return fmt.Errorf("CID not found: %d status", header.StatusCode)
	}

	if header.ContentLength <= file.FileSize(outputFn) {
		// The file on disc is larger than the one we will download which means it has more
		// timestamps in it, so we don't download it.
		return nil
	}

	//Get the response bytes from the url
	response, err := http.Get(url.String())
	if err != nil {
		return err
	}
	defer response.Body.Close()
	if response.StatusCode != 200 {
		return fmt.Errorf("CID not found: %d status", header.StatusCode)
	}

	logger.Info(colors.Yellow + "Download complete. Saving timestamps..." + colors.Off)

	userHitsCtrlC := false
	go func() {
		for {
			if file.FileSize(outputFn) >= header.ContentLength {
				break
			}
			pct := int(float64(file.FileSize(outputFn)) / float64(header.ContentLength) * 100)
			msg := colors.Yellow + fmt.Sprintf("Downloading timestamps. Please wait... %d%%", pct) + colors.Off
			if userHitsCtrlC {
				msg = colors.Yellow + fmt.Sprintf("Finishing work. please wait... %d%%                                 ", pct) + colors.Off
			}
			logger.Progress(true, msg)
			time.Sleep(500 * time.Microsecond)
		}
	}()

	trapChannel := sigintTrap.Enable(context.Background(), func() {}, func() {
		userHitsCtrlC = true
	})
	defer sigintTrap.Disable(trapChannel)

	ff, err := os.Create(outputFn)
	if err != nil {
		return err
	}
	defer ff.Close()

	_, err = io.Copy(ff, response.Body)
	return err
}
