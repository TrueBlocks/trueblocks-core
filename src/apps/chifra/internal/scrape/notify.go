package scrapePkg

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"net/http"
	"syscall"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/uniq"
)

var ErrConfiguredButNotRunning = fmt.Errorf("listener is configured but not running")

type Message string

const (
	MessageChunkWritten Message = "chunkWritten"
	MessageStageUpdated Message = "stageUpdated"
	MessageAppearance   Message = "asppearance"
)

type Notification[Payload notificationPayload] struct {
	Msg     Message       `json:"msg"`
	Meta    *rpc.MetaData `json:"meta"`
	Payload Payload       `json:"payload"`
}

type notificationPayload interface {
	NotificationPayloadAppearance |
		string
}

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

func NewAppearanceNotification(meta *rpc.MetaData, appearance NotificationPayloadAppearance) *Notification[NotificationPayloadAppearance] {
	return &Notification[NotificationPayloadAppearance]{
		Msg:     MessageChunkWritten,
		Meta:    meta,
		Payload: appearance,
	}
}

// Notify may be used to tell other processes about progress.
func Notify[Payload notificationPayload](notification Notification[Payload]) error {
	endpoint := config.GetScrape(notification.Meta.Chain).NotifyUrl
	if endpoint == "" {
		return nil
	}
	return notify(endpoint, notification)
}

func notify(endpoint string, notification any) error {
	encoded, err := json.Marshal(notification)
	if err != nil {
		return fmt.Errorf("marshalling message: %w", err)
	}

	resp, err := http.Post(
		endpoint,
		"application/json",
		bytes.NewReader(encoded),
	)
	if err != nil {
		if errors.Is(err, syscall.ECONNREFUSED) {
			return ErrConfiguredButNotRunning
		}
		return fmt.Errorf("sending notification: %w", err)
	}
	if resp.StatusCode < 200 || resp.StatusCode > 299 {
		respBody, _ := io.ReadAll(resp.Body)
		resp.Body.Close()
		return fmt.Errorf("listener responded with %d: %s", resp.StatusCode, respBody)
	}
	return nil
}
