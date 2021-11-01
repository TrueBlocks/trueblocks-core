/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
package sigintTrap

import (
	"errors"
	"os"
	"os/signal"
)

var ErrInterrupted = errors.New("interrupted")

// Enable enables the trap, by blocking control-C. It returns
// a channel that will get a value when user presses ctrl-C.
func Enable() chan bool {
	cancelledChannel := make(chan bool, 1)
	signals := make(chan os.Signal)
	signal.Notify(signals, os.Interrupt)
	go func() {
		<-signals
		cancelledChannel <- true
	}()

	return cancelledChannel
}

// Disable stops blocking control-C
func Disable(channel chan bool) {
	signal.Reset(os.Interrupt)
	close(channel)
}
