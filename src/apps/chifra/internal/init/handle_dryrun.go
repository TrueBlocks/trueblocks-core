package initPkg

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/history"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

func (opts *InitOptions) HandleDryRun(rCtx output.RenderCtx) error {
	chain := opts.Globals.Chain

	remoteManifest, err := manifest.ReadManifest(chain, opts.PublisherAddr, manifest.TempContract)
	if err != nil {
		return err
	}
	historyFile := config.PathToRootConfig() + "unchained.txt"
	saved := history.FromHistory(historyFile, "headerVersion")
	defer func() {
		_ = history.ToHistory(historyFile, "headerVersion", saved)
	}()
	_ = history.ToHistory(historyFile, "headerVersion", remoteManifest.Version)
	logger.InfoTable("Existing version:", saved)
	logger.InfoTable("Remote version:", remoteManifest.Version)

	if remoteManifest.Chain != chain {
		msg := fmt.Sprintf("The chain value found in the downloaded manifest (%s) does not match the manifest on the command line (%s).", remoteManifest.Chain, chain)
		return errors.New(msg)
	}

	// Get the list of things we need to download
	_, nToDownload, nDeleted, err := opts.prepareDownloadList(chain, remoteManifest, []base.Blknum{})
	if err != nil {
		return err
	}

	spec := manifest.Specification()
	if opts.Globals.TestMode {
		nToDownload = base.Min(10, nToDownload)
		spec = "--testing-hash--"
	}

	// Tell the user what we're doing
	logger.InfoTable("Unchained Index:", config.GetUnchained().SmartContract)
	logger.InfoTable("PreferredPublisher:", opts.Publisher)
	logger.InfoTable("Specification:", spec)
	logger.InfoTable("Config Folder:", config.MustGetPathToChainConfig(chain))
	logger.InfoTable("Index Folder:", config.PathToIndex(chain))
	logger.InfoTable("Chunks in manifest:", fmt.Sprintf("%d", len(remoteManifest.Chunks)))
	logger.InfoTable("Files that would be deleted:", fmt.Sprintf("%d", nDeleted))
	logger.InfoTable("Files that would be downloaded:", fmt.Sprintf("%d", nToDownload))

	if nDeleted+nToDownload > 0 {
		logger.Warn("")
		logger.Warn("DryRun: The process would have modified both the manifest and the index. In that case, you would have")
		logger.Warn("DryRun: had to invalidate your monitor cache by removing it.")
		logger.Warn("")
		logger.Warn("DryRun: No files were modified.")
	}

	return nil
}
