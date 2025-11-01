package notify

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"

// TODO: THIS IS PART OF THE NOTIFY CODE TO BE USED FOR MONITORING
type Message string

type Notification[T NotificationPayload] struct {
	Msg     Message         `json:"msg"`
	Meta    *types.MetaData `json:"meta"`
	Payload T               `json:"payload"`
}

type NotificationPayload interface {
	[]NotificationPayloadAppearance |
		[]NotificationPayloadChunkWritten |
		NotificationPayloadChunkWritten |
		string
}
