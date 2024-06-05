package notify

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/uniq"
)

const (
	MessageChunkWritten Message = "chunkWritten"
	MessageStageUpdated Message = "stageUpdated"
	MessageAppearance   Message = "appearance"
)

type NotificationPayloadAppearance struct {
	Address string `json:"address"`
	// We use string for block number to ensure it's never
	// too big
	BlockNumber      string `json:"blockNumber"`
	TransactionIndex uint32 `json:"txid"`
}

func (n *NotificationPayloadAppearance) FromString(s string) (err error) {
	var bn uint32
	_, err = fmt.Sscanf(
		s,
		uniq.AppearanceFmt,
		&n.Address,
		&bn,
		&n.TransactionIndex,
	)

	n.BlockNumber = fmt.Sprint(bn)
	return
}

func NewChunkWrittenNotification(meta *types.MetaData, chunk string) *Notification[string] {
	return &Notification[string]{
		Msg:     MessageChunkWritten,
		Meta:    meta,
		Payload: chunk,
	}
}

func NewStageUpdatedNotification(meta *types.MetaData, chunkRange string) *Notification[string] {
	return &Notification[string]{
		Msg:     MessageChunkWritten,
		Meta:    meta,
		Payload: chunkRange,
	}
}

func NewAppearanceNotification(meta *types.MetaData, appearances []NotificationPayloadAppearance) *Notification[[]NotificationPayloadAppearance] {
	return &Notification[[]NotificationPayloadAppearance]{
		Msg:     MessageChunkWritten,
		Meta:    meta,
		Payload: appearances,
	}
}

type NotificationPayloadChunkWritten struct {
	Cid    string `json:"cid"`
	Range  string `json:"range"`
	Author string `json:"author"`
}
