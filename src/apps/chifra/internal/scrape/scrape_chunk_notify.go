package scrapePkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/notify"
)

func (opts *ScrapeOptions) NotifyChunkWritten(chunk index.Chunk, chunkPath string) (err error) {
	_ = chunk
	if !opts.Notify {
		return nil
	}

	// If --notify is on, it's properly configured and IPFS is running
	var cidString string
	if config.IpfsRunning() { // probablyh redundant
		if cidString, err = index.ChunkCid(chunkPath); err != nil {
			return err
		}
	}

	// Generate range from path, as chunks sometimes don't have Range set
	chunkRange := base.RangeFromFilename(index.ToIndexPath(chunkPath))
	return Notify(notify.Notification[[]notify.NotificationPayloadChunkWritten]{
		Msg:  notify.MessageChunkWritten,
		Meta: nil,
		Payload: []notify.NotificationPayloadChunkWritten{
			{
				Cid:    cidString,
				Range:  chunkRange.String(),
				Author: config.GetRootConfig().Settings.Notify.Author,
			},
		},
	})
}
