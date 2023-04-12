// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package progress

type Event uint

const (
	BeforeStart Event = iota
	Start
	Update
	Finished
	Error
	AllDone
	Cancelled
)

type ProgressMsg struct {
	Event   Event
	Message string
	Payload interface{}
}

func MakeChan() chan *ProgressMsg {
	return make(chan *ProgressMsg, 100)
}
