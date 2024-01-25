package scrapePkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/notify"
)

func NotifyChunkWritten(chunk index.Chunk, chunkPath string) (err error) {
	var cidString string
	if ok, _ := NotifyConfigured(); ok {
		if cidString, err = index.ChunkCid(chunkPath); err != nil {
			return err
		}
	}
	return Notify(notify.Notification[[]notify.NotificationPayloadChunkWritten]{
		Msg:  notify.MessageChunkWritten,
		Meta: nil,
		Payload: []notify.NotificationPayloadChunkWritten{
			{
				Cid:    cidString,
				Range:  chunk.Range.String(),
				Author: config.GetRootConfig().Settings.Notify.Author,
			},
		},
	})
}
