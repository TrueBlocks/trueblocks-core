package notify

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/uniq"
)

const (
	MessageChunkWritten Message = "chunkWritten"
	MessageStageUpdated Message = "stageUpdated"
	MessageAppearance   Message = "asppearance"
)

type NotificationPayloadAppearance struct {
	Address string `json:"address"`
	// We use string for block number to ensure it's never
	// too big
	BlockNumber      string `json:"blockNumber"`
	TransactionIndex uint32
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

func NewChunkWrittenNotification(meta *rpc.MetaData, chunk string) *Notification[string] {
	return &Notification[string]{
		Msg:     MessageChunkWritten,
		Meta:    meta,
		Payload: chunk,
	}
}

func NewStageUpdatedNotification(meta *rpc.MetaData, chunkRange string) *Notification[string] {
	return &Notification[string]{
		Msg:     MessageChunkWritten,
		Meta:    meta,
		Payload: chunkRange,
	}
}

func NewAppearanceNotification(meta *rpc.MetaData, appearances []NotificationPayloadAppearance) *Notification[[]NotificationPayloadAppearance] {
	return &Notification[[]NotificationPayloadAppearance]{
		Msg:     MessageChunkWritten,
		Meta:    meta,
		Payload: appearances,
	}
}
