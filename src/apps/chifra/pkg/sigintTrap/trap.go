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
