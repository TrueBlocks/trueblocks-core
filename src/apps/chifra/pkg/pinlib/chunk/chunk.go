package chunk

import (
	"compress/gzip"
	"context"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"strings"
	"sync"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/sigintTrap"
	"github.com/panjf2000/ants/v2"
)

type ChunkType uint

const (
	BloomChunk ChunkType = iota
	IndexChunk
)

type jobResult struct {
	fileName string
	contents io.Reader
}

var gatewayUrl = "https://gateway.pinata.cloud/ipfs/"
var outputDir = "/tmp/pins/"

func fetchChunk(url string) (io.Reader, error) {
	response, err := http.Get(url)
	body := response.Body

	if response.StatusCode != 200 {
		return nil, fmt.Errorf("wrong status code: %d", response.StatusCode)
	}

	if err != nil {
		log.Println("\u274C [Error]", url)
		return body, err
	}

	log.Println("\u2705 [Success]", url)
	return body, nil
}

func GetChunkFromRemote(pins []manifest.PinDescriptor, chunkType ChunkType) {
	writeChannel := make(chan *jobResult, 20)
	ctx := context.Background()
	ctx, cancel := context.WithCancel(ctx)
	var wg sync.WaitGroup

	defer func() {
		cancel()
	}()

	pool, err := ants.NewPoolWithFunc(10, func(param interface{}) {
		url := gatewayUrl
		pin := param.(manifest.PinDescriptor)

		defer wg.Done()

		select {
		case <-ctx.Done():
			// Cancel
			ants.Reboot()
			return
		default:
			if chunkType == IndexChunk {
				url += pin.IndexHash
			} else {
				url += pin.BloomHash
			}

			reader, err := fetchChunk(url)

			if err == nil {
				writeChannel <- &jobResult{
					fileName: pin.FileName,
					contents: reader,
				}
			}

			if strings.Contains(gatewayUrl, "pinata") {
				time.Sleep(time.Second * 5) // Align with Pinata rate limit
			}
		}
	})

	defer pool.Release()

	if err != nil {
		panic(err)
	}

	go func() {
		for res := range writeChannel {
			err := saveFileContents(res)

			if err == sigintTrap.ErrInterrupted {
				log.Println("Finishing work...")
				cancel()
				break
			}
		}

		close(writeChannel)
	}()

	for _, pin := range pins {
		wg.Add(1)
		pool.Invoke(pin)
	}

	wg.Wait()
	fmt.Printf("running goroutines: %d\n", pool.Running())
	fmt.Println("pins done:", len(pins))
}

func saveFileContents(res *jobResult) error {
	// Postpone Ctrl-C
	cancelledChannel := sigintTrap.Enable()
	defer sigintTrap.Disable(cancelledChannel)

	log.Println("[Saving]", res.fileName)

	archive, err := gzip.NewReader(res.contents)

	if err != nil {
		log.Println("Error unpacking", res.fileName, ":", err)
		return err
	}

	defer archive.Close()

	// TODO: should be changed to finalized/file-name.bin for index chunks
	output, err := os.Create(outputDir + res.fileName + ".bloom")
	if err != nil {
		log.Println("Error creating file", res.fileName, ":", err)
		return err
	}
	defer output.Close()

	_, werr := io.Copy(output, archive)

	if werr != nil {
		log.Println("Error copying", res.fileName, ":", err)
		return err
	}

	if c := <-cancelledChannel; c {
		return sigintTrap.ErrInterrupted
	}

	return nil
}

func FilterDownloadedChunks(pins []manifest.PinDescriptor) []manifest.PinDescriptor {
	fileMap := make(map[string]bool)

	files, err := ioutil.ReadDir(outputDir)

	if err != nil {
		return pins
	}

	for _, file := range files {
		fileMap[file.Name()] = true
	}

	return exclude(fileMap, pins)
}

func exclude(what map[string]bool, from []manifest.PinDescriptor) []manifest.PinDescriptor {
	result := make([]manifest.PinDescriptor, 0, len(from))

	for _, pin := range from {
		if what[pin.FileName] {
			continue
		}

		result = append(result, pin)
	}

	return result
}
