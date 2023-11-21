package notify

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"

type Message string

type Notification[Payload NotificationPayload] struct {
	Msg     Message       `json:"msg"`
	Meta    *rpc.MetaData `json:"meta"`
	Payload Payload       `json:"payload"`
}

type NotificationPayload interface {
	[]NotificationPayloadAppearance |
		string
}
