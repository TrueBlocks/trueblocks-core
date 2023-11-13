package scrapePkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
)

var ErrConfiguredButNotRunning = fmt.Errorf("listener is configured but not running")

type Message string

var ChunkWritten Message = "chunk written"
var StageUpdated Message = "stage updated"

// Notify may be used to tell other processes about progress.
func Notify(msg Notification) error {
	return nil
}

type Notification struct {
	Msg  Message       `json:"msg"`
	Meta *rpc.MetaData `json:"meta"`
	Data interface{}   `json:"data"`
}
