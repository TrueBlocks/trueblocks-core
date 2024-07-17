package main

import (
	"os"
	"strconv"
)

// getOptions returns the number of addresses, the maximum amount of wei, and
// the frequency. If present, the values are taken from the command line.
// Note: there is no error checking, so make sure the values are correct.
func getOptions() (int, float64, string) {
	var err error
	nAddrs := 15
	if len(os.Args) > 1 {
		if nAddrs, err = strconv.Atoi(os.Args[1]); err != nil {
			nAddrs = 15
		}
	}

	maxAmt := 500000.0
	if len(os.Args) > 2 {
		if maxAmt, err = strconv.ParseFloat(os.Args[2], 64); err != nil {
			maxAmt = 500000.0
		}
	}

	freq := "annually"
	if len(os.Args) > 3 {
		if freq = os.Args[3]; len(freq) == 0 {
			freq = "annually"
		}
	}

	return nAddrs, maxAmt, freq
}
