// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package sigintTrap

import (
	"context"
	"os"
	"os/signal"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
)

var TrapMessage = colors.Yellow + "Ctrl+C. Finishing..." + colors.Off
var sigintMessageOnce sync.Once

type CleanupFunction func()

// Enable enables the trap, by blocking control-C. It returns
// a channel that will get a value when user presses ctrl-C.
func Enable(ctx context.Context, cancel context.CancelFunc, cleanUp CleanupFunction) chan os.Signal {
	signals := make(chan os.Signal, 1)
	signal.Notify(signals, os.Interrupt)

	go func() {
		for {
			select {
			case <-signals:
				sigintMessageOnce.Do(cleanUp)
				if cancel != nil {
					cancel()
				}
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
