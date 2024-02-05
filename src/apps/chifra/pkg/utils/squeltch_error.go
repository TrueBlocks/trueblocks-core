package utils

import "fmt"

// SqueltchError squelches errors if we are in test mode and testing concurrent code. Because
// ordering of errors is non-deterministc, we only want to report the existance, not the contents.
func SqueltchError(testMode bool, err error) error {
	if testMode {
		return fmt.Errorf("there was an error during testing")
	}
	return err
}
