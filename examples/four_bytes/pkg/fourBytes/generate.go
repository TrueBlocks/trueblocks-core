package fourbytes

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"runtime"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/ethereum/go-ethereum/crypto"
	ants "github.com/panjf2000/ants/v2"
)

var mu sync.Mutex

// chunks is a map where keys are first four bytes of a signature
// and values are contents of chunk file
var chunks map[string]mapRecord

type mapRecord struct {
	sigRecords []SignatureRecord
	strRecords []StringRecord
}

type collectWorkerArgs struct {
	Str      string
	Encoded  []byte
	DirName  string
	FileName string
}

var collectWg sync.WaitGroup

// collectWorker puts signatures and their string representation under correct
// keys in `chunks` map
func collectWorker(i interface{}) {
	defer collectWg.Done()

	args := i.(*collectWorkerArgs)
	var offset uint32 = 0
	key := filepath.Join(args.DirName, args.FileName)

	// lock mutex for writing
	mu.Lock()
	record := chunks[key]

	// we need string length and valid offset
	strLen := len(args.Str)
	prevRecs := len(record.sigRecords)
	if prevRecs > 0 {
		offset = record.sigRecords[prevRecs-1].Offset + record.sigRecords[prevRecs-1].Len
	}
	// now we update the record
	record.strRecords = append(record.strRecords, StringRecord{Value: args.Str, Len: strLen})
	record.sigRecords = append(record.sigRecords, SignatureRecord{
		Signature: base.BytesToHash(args.Encoded),
		Offset:    offset,
		Len:       uint32(strLen),
	})
	chunks[key] = record
	mu.Unlock()
}

type writePoolArgs struct {
	outDir string
	key    string
	value  mapRecord
}

var writeWg sync.WaitGroup

// writeWorker writes chunk files
func writeWorker(i interface{}) {
	defer writeWg.Done()

	args := i.(*writePoolArgs)
	out, err := os.OpenFile(filepath.Join(args.outDir, args.key), os.O_RDWR|os.O_CREATE, 0666)
	if err != nil {
		panic(err)
	}
	defer out.Close()

	if err = write(out, args.value.sigRecords, args.value.strRecords); err != nil {
		panic(err)
	}
}

// GenerateFromFiles reads input files and creates four bytes database. It then sends it to writeWorkers to save
// chunk files.
func GenerateFromFiles(outDir string) (err error) {
	sigsFn := filepath.Join(config.PathToRootConfig(), "abis/known-000/uniq_sigs.tab")
	sigsFile, err := os.OpenFile(sigsFn, os.O_RDONLY, 0)
	if err != nil {
		return
	}
	funcsFn := filepath.Join(config.PathToRootConfig(), "abis/known-000/uniq_funcs.tab")
	funcsFile, err := os.OpenFile(funcsFn, os.O_RDONLY, 0)
	if err != nil {
		return
	}
	eventsFn := filepath.Join(config.PathToRootConfig(), "abis/known-000/uniq_events.tab")
	eventsFile, err := os.OpenFile(eventsFn, os.O_RDONLY, 0)
	if err != nil {
		return
	}
	defer func() {
		sigsFile.Close()
		funcsFile.Close()
		eventsFile.Close()
	}()

	// Some stats
	mapItems := 0
	writtenFiles := 0

	chunks = make(map[string]mapRecord, 100000)

	// This pool will put data into `chunks` map
	collectPool, err := ants.NewPoolWithFunc(runtime.GOMAXPROCS(0), collectWorker)
	if err != nil {
		return err
	}
	// This pool will save chunk files
	writePool, err := ants.NewPoolWithFunc(runtime.GOMAXPROCS(0), writeWorker)
	if err != nil {
		return err
	}

	paramsScanner := bufio.NewScanner(sigsFile)

	for paramsScanner.Scan() {
		params := paramsScanner.Text()

		if _, err = funcsFile.Seek(0, io.SeekStart); err != nil {
			return err
		}
		if _, err = eventsFile.Seek(0, io.SeekStart); err != nil {
			return err
		}

		// We create MultiReader to make reading from both functions and events files
		// easier
		multiReader := io.MultiReader(funcsFile, eventsFile)
		nameScanner := bufio.NewScanner(multiReader)

		for nameScanner.Scan() {
			name := nameScanner.Text()
			if err = nameScanner.Err(); err != nil {
				return err
			}
			// Build signature and generate keccak256
			str := fmt.Sprintf("%s(%s)", name, params)
			encoded := crypto.Keccak256([]byte(str))

			fileName := fmt.Sprintf("%x", encoded[:2])
			dirName := fileName[:2]

			err := file.EstablishFolder(filepath.Join(outDir, dirName))
			if err != nil {
				return err
			}

			mapItems++
			if mapItems%1000000 == 0 {
				fmt.Printf("%d items... ", mapItems)
			}

			collectWg.Add(1)

			collectPool.Invoke(&collectWorkerArgs{
				Str:      str,
				Encoded:  encoded,
				DirName:  dirName,
				FileName: fileName,
			})
		}
	}

	fmt.Println("\nWaiting for collect workers to finish")

	collectWg.Wait()
	collectPool.Release()

	fmt.Printf("Writing %d files (%d map items)\n", len(chunks), mapItems)

	for key, value := range chunks {
		writeWg.Add(1)

		writePool.Invoke(&writePoolArgs{
			outDir: outDir,
			key:    key,
			value:  value,
		})

		writtenFiles++
		if writtenFiles%1000 == 0 {
			fmt.Printf("%d... ", writtenFiles)
		}
	}

	fmt.Println("\nWaiting for write to finish")
	writeWg.Wait()
	fmt.Println()
	writePool.Release()

	fmt.Println("Veryfing...")
	Verify(&chunks, outDir)

	return nil
}
