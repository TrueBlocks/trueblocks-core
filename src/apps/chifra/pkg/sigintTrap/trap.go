// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package sigintTrap

import (
	"context"
	"errors"
	"fmt"
	"os"
	"os/signal"
	"sync"
)

var ErrInterrupted = errors.New("interrupted")
var SigintMessageOnce sync.Once

func DisplaySigintMessage() {
	fmt.Println("\nFinishing work...")
}

// Enable enables the trap, by blocking control-C. It returns
// a channel that will get a value when user presses ctrl-C.
func Enable(ctx context.Context, cancel context.CancelFunc) chan os.Signal {
	signals := make(chan os.Signal, 1)
	signal.Notify(signals, os.Interrupt)

	go func() {
		for {
			select {
			case <-signals:
				SigintMessageOnce.Do(DisplaySigintMessage)
				cancel()
				return
			case <-ctx.Done():
				return
			}
		}
	}()

	return signals
}

func Disable(channel chan os.Signal) {
	signal.Stop(channel)
}
