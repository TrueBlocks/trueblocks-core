package sigintTrap

import (
	"errors"
	"os"
	"os/signal"
)

var ErrInterrupted = errors.New("interrupted")

func Enable() chan bool {
	cancelledChannel := make(chan bool)
	signals := make(chan os.Signal)
	signal.Notify(signals, os.Interrupt)
	go func() {
		<-signals
		cancelledChannel <- true
	}()

	return cancelledChannel
}

func Disable(channel chan bool) {
	signal.Reset(os.Interrupt)
	close(channel)
}
