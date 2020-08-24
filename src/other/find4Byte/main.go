package main

import (
	"bufio"
	"bytes"
	"encoding/hex"
	"fmt"
	"os"
	"runtime"
	"sync"

	"github.com/ethereum/go-ethereum/crypto"
	"github.com/panjf2000/ants/v2"
)

func main() {
	var search [][]byte
	for _, i := range os.Args[1:] {
		decoded, err := hex.DecodeString(i[2:])
		if err != nil {
			fmt.Printf("Couldn't parse argument %s\n", i)
		} else {
			search = append(search, decoded)
		}
	}

	// Create thread pool with number of concurrent threads equal to 'runtime.NumCPU()'
	var wait sync.WaitGroup
	checkOne, _ := ants.NewPoolWithFunc(runtime.NumCPU(), func(canonical interface{}) {
		// Calculate 4-byte form
		res := crypto.Keccak256([]byte(canonical.(string)))[:4]
		// Go through queries and compare
		for i := 0; i < len(search); i++ {
			cur := search[i]
			if bytes.Equal(res, cur) {
				fmt.Printf("                                                            \r")
				fmt.Printf("\"0x%s\",\"%s\"\n", hex.EncodeToString(res), canonical)
				os.Exit(0)
			}
			fmt.Fprintf(os.Stderr, "\033[2KScanning: %s\r", canonical)
		}
		wait.Done()
	})

	// Load files
	fTemp2, _ := os.Open("/Users/jrush/.quickBlocks/known_abis/uniq_funcs.tab")
	defer fTemp2.Close()
	sTemp2 := bufio.NewScanner(fTemp2)
	sTemp2.Split(bufio.ScanLines)

	fTemp1, _ := os.Open("/Users/jrush/.quickBlocks/known_abis/uniq_sigs.tab")
	defer fTemp1.Close()
	sTemp1 := bufio.NewScanner(fTemp1)
	sTemp1.Split(bufio.ScanLines)

	// temp1s := []string{""}
	// for sTemp1.Scan() {
	// 	temp1s = append(temp1s, sTemp1.Text())
	// }

	// // Brute force names and signatures pushing them to pool
	// for sTemp2.Scan() {
	// 	for _, temp1 := range temp1s {
	// 		wait.Add(1)
	// 		_ = checkOne.Invoke(string(fmt.Sprintf("%s(%s)", sTemp2.Text(), temp1)))
	// 	}
	// }

	temp2s := []string{""}
	for sTemp2.Scan() {
		temp2s = append(temp2s, sTemp2.Text())
	}

	for sTemp1.Scan() {
		for _, temp2 := range temp2s {
			wait.Add(1)
			_ = checkOne.Invoke(string(fmt.Sprintf("%s(%s)", temp2, sTemp1.Text())))
		}
	}
	// Wait till all threads finished at the end of program
	defer checkOne.Release()
	defer wait.Wait()
}
