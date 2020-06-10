package main

import (
    "bufio"
    "bytes"
    "encoding/hex"
    "fmt"
    "github.com/ethereum/go-ethereum/crypto"
    "github.com/mholt/archiver/v3"
    "github.com/panjf2000/ants/v2"
    "os"
    "runtime"
    "sync"
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

    // Unpack archieve if not unpacked before
    test, err := os.Open("function_names.csv")
    if err != nil {
        archiver.Unarchive("data.tar.gz", "./")
    }
    test.Close()

    // Load files
    fNames, _ := os.Open("function_names.csv")
    fSigs, _ := os.Open("function_signtures.csv")
    defer fNames.Close()
    defer fSigs.Close()

    signatures := []string{""}

    // Load all signatures from file into signatures array
    sSigs := bufio.NewScanner(fSigs)
    sSigs.Split(bufio.ScanLines)
    for sSigs.Scan() {
        signatures = append(signatures, sSigs.Text())
    }

    // Create thread pool with number of concurrent threads equal to 'runtime.NumCPU()'
    var wait sync.WaitGroup
    p, _ := ants.NewPoolWithFunc(runtime.NumCPU(), func(canonical interface{}) {
        // Calculate 4-byte form
        res := crypto.Keccak256([]byte(canonical.(string)))[:4]
        // Go through queries and compare
        for i := 0; i < len(search); i++ {
            cur := search[i]
            if bytes.Equal(res, cur) {
                fmt.Printf("Found: 0x%s\t%s\n", hex.EncodeToString(res), canonical)
            } else {
                // \033[2K clears line, so output is human-readable
                fmt.Fprintf(os.Stderr, "\033[2KScanning: %s\r", canonical)
            }
        }
        wait.Done()
    })

    // Wait till all threads finished at the end of program
    defer p.Release()
    defer wait.Wait()

    sNames := bufio.NewScanner(fNames)
    sNames.Split(bufio.ScanLines)

    // Brute force names and signatures pushing them to pool
    for sNames.Scan() {
        for _, sig := range signatures {
            wait.Add(1)
            _ = p.Invoke(string(fmt.Sprintf("%s(%s)", sNames.Text(), sig)))
        }
    }
}
