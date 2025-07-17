package abi

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"io/fs"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
	"github.com/ethereum/go-ethereum/accounts/abi"
)

func PathToAbisCache(chain, fileName string) string {
	return filepath.Join(walk.GetRootPathFromCacheType(chain, walk.Cache_Abis), fileName)
}

func PathToKnownAbis(fileName string) string {
	return filepath.Join(config.PathToRootConfig(), "abis", fileName)
}

// LoadAbi tries to load ABI from any source (local file, cache, download from 3rd party)
func LoadAbi(conn *rpc.Connection, address base.Address, abiMap *SelectorSyncMap) error {
	err := conn.IsContractAtLatest(address)
	if err != nil {
		if errors.Is(err, rpc.ErrNotAContract) && !utils.IsFuzzing() {
			logger.Progress(logger.IsTerminal(), fmt.Sprintf("Skipping EOA %s", colors.Cyan+address.Hex()+colors.Off))
		}
		return err
	}

	if err = loadAbiFromAddress(conn, address, abiMap); err != nil {
		if !os.IsNotExist(err) && err != io.EOF {
			return fmt.Errorf("while reading %s ABI file: %w", address, err)
		}

		return abiMap.downloadAbi(conn, address)
	}

	return nil
}

// Where to find know ABI files
// var knownAbiSubdirectories = []string{
// 	"known-000", "known-005", "known-010", "known-015",
// }

func fromJson(reader io.Reader, abiMap *SelectorSyncMap) (err error) {
	// Compute encodings, signatures and parse file
	loadedAbi, err := abi.JSON(reader)
	if err != nil {
		return
	}

	for _, method := range loadedAbi.Methods {
		function := types.FunctionFromAbiMethod(&method)
		abiMap.SetValue(function.Encoding, function)
	}

	for _, ethEvent := range loadedAbi.Events {
		event := types.FunctionFromAbiEvent(&ethEvent)
		abiMap.SetValue(event.Encoding, event)
	}

	return
}

// loadAbiFromKnownFile loads data from _known_ ABI file, which has encodings and
// signatures.
func loadAbiFromKnownFile(filePath string, abiMap *SelectorSyncMap) (err error) {
	f, err := os.OpenFile(filePath, os.O_RDONLY, 0)
	if err != nil {
		return
	}

	// We still need abi.Method and abi.Event, so will just use fromJson
	return fromJson(f, abiMap)
}

// readCString reads cString structure from reader. It has different signature than
// the rest of `read*` functions in this package, to ease reading values into
// other structs' fields
func readCString(reader *bufio.Reader, str *cString) (err error) {
	err = binary.Read(reader, binary.LittleEndian, &str.size)
	if err != nil {
		return
	}

	str.content = make([]byte, str.size)
	err = binary.Read(reader, binary.LittleEndian, str.content)
	if err != nil {
		return
	}
	return
}

func readCacheHeader(reader *bufio.Reader, target *cacheHeader) (err error) {
	err = binary.Read(reader, binary.LittleEndian, &target.deleted)
	if err != nil {
		return
	}

	err = binary.Read(reader, binary.LittleEndian, &target.schema)
	if err != nil {
		return
	}

	err = binary.Read(reader, binary.LittleEndian, &target.showing)
	if err != nil {
		return
	}

	err = readCString(reader, &target.className)
	if err != nil {
		return err
	}

	return
}

func readAddress(reader *bufio.Reader, target *base.Address) (err error) {
	str := &cString{}
	err = readCString(reader, str)
	if err != nil {
		return
	}
	addr := base.HexToAddress(string(str.content))
	*target = addr
	return
}

type arrayItem interface {
	~string |
		base.Hash |
		base.Address |
		types.Parameter |
		types.Function
}

func writeArray[Item arrayItem](
	writer *bufio.Writer,
	items []Item,
	writeFn func(*bufio.Writer, *Item) (err error),
) (err error) {
	itemCount := uint64(len(items))
	err = binary.Write(writer, binary.LittleEndian, itemCount)
	if err != nil {
		return
	}

	for _, item := range items {
		err = writeFn(writer, &item)
		if err != nil {
			return
		}
	}
	return
}

// readFromArray converts binary array into slice of type Item
func readFromArray[Item arrayItem](
	reader *bufio.Reader,
	target *[]Item,
	readValue func(reader *bufio.Reader) (*Item, error),
) (err error) {
	// first, read item count
	var itemCount uint64 = 0
	err = binary.Read(reader, binary.LittleEndian, &itemCount)
	if err != nil {
		return
	}

	// make target large enough
	*target = make([]Item, 0, itemCount)

	// TODO: Just noting. If we knew the records in the array were fixed with (I think we
	// TODO: may be able to know that), we can read and write the entire chunk of memory
	// TODO: in one write (or read). It will almost certainly be faster. I don't think we do
	// TODO: this in C++ code, but I always wanted to.
	// read items
	for i := 0; uint64(i) < itemCount; i++ {
		item, readErr := readValue(reader)
		if readErr != nil {
			err = readErr
			return
		}

		*target = append(*target, *item)
	}

	return
}

// readAbis reads ABI cache (known.bin)
func readAbis(reader *bufio.Reader) (result []types.Function, err error) {
	header := &cacheHeader{}
	if err = readCacheHeader(reader, header); err != nil {
		return
	}
	if err = validateHeader(header); err != nil {
		return
	}

	// This address is always empty
	var address base.Address
	if err = readAddress(reader, &address); err != nil {
		return
	}
	err = readFromArray(reader, &result, readFunction)

	return
}

func readParameter(reader *bufio.Reader) (param *types.Parameter, err error) {
	param = &types.Parameter{}
	header := &cacheHeader{}
	err = readCacheHeader(reader, header)
	if err != nil {
		return
	}
	err = validateHeader(header)
	if err != nil {
		return
	}

	err = readString(reader, &param.ParameterType)
	if err != nil {
		return
	}

	err = readString(reader, &param.Name)
	if err != nil {
		return
	}

	err = readString(reader, &param.StrDefault)
	if err != nil {
		return
	}

	var jsonValue string
	if err = readString(reader, &jsonValue); err != nil {
		return
	}
	if err = json.Unmarshal([]byte(jsonValue), &param.Value); err != nil {
		return
	}

	err = binary.Read(reader, binary.LittleEndian, &param.Indexed)
	if err != nil {
		return
	}

	err = readString(reader, &param.InternalType)
	if err != nil {
		return
	}

	err = readFromArray(reader, &param.Components, readParameter)
	if err != nil {
		return
	}

	unused1 := false
	err = binary.Read(reader, binary.LittleEndian, &unused1)
	if err != nil {
		return
	}

	unused2 := uint64(0)
	err = binary.Read(reader, binary.LittleEndian, &unused2)
	if err != nil {
		return
	}

	return
}

// TODO: I don't think we want to hard code this version value here. We want to read it programmatically
// TODO: from auto-generated code. There is a string called version.LibraryVersion that we can use
// TODO: to calculate this value. We can add a function to the version package.
var minimumCacheVersion = uint64(41000)

func validateHeader(header *cacheHeader) error {
	if header.schema < minimumCacheVersion {
		return errors.New("invalid schema")
	}
	return nil
}

type cString struct {
	size    uint64
	content []byte
}
type cacheHeader struct {
	deleted   uint64
	schema    uint64
	showing   uint64
	className cString
}

func readString(reader *bufio.Reader, target *string) (err error) {
	str := &cString{}
	err = readCString(reader, str)
	if err != nil {
		return
	}
	s := string(str.content)
	*target = s
	return
}

func readFunction(reader *bufio.Reader) (function *types.Function, err error) {
	function = &types.Function{}
	header := &cacheHeader{}
	err = readCacheHeader(reader, header)
	if err != nil {
		return
	}
	err = validateHeader(header)
	if err != nil {
		return
	}

	err = readString(reader, &function.Name)
	if err != nil {
		return
	}

	err = readString(reader, &function.FunctionType)
	if err != nil {
		return
	}

	var unused string
	err = readString(reader, &unused)
	if err != nil {
		return
	}

	err = binary.Read(reader, binary.LittleEndian, &function.Anonymous)
	if err != nil {
		return
	}

	err = binary.Read(reader, binary.LittleEndian, &function.Constant)
	if err != nil {
		return
	}

	err = readString(reader, &function.StateMutability)
	if err != nil {
		return
	}

	err = readString(reader, &function.Signature)
	if err != nil {
		return
	}

	err = readString(reader, &function.Encoding)
	if err != nil {
		return
	}

	err = readFromArray(reader, &function.Inputs, readParameter)
	if err != nil {
		return
	}

	err = readFromArray(reader, &function.Outputs, readParameter)
	if err != nil {
		return
	}

	return
}

// getAbis reads all ABIs stored in the cache
func getAbis(chain string) ([]types.Function, error) {
	fullPath := PathToAbisCache(chain, "known.bin")
	if f, err := os.OpenFile(fullPath, os.O_RDONLY, 0); err != nil {
		return nil, err

	} else {
		defer f.Close()
		bufReader := bufio.NewReader(f)
		abis, err := readAbis(bufReader)
		if err != nil && !os.IsNotExist(err) {
			// Remove the file (if it exists). We will re-try next time
			os.Remove(fullPath)
		}
		return abis, err
	}
}

// loadCache loads binary cache of known ABIs
func loadCache(chain string, abiMap *SelectorSyncMap) (loaded bool) {
	functions, cacheErr := getAbis(chain)
	// We can ignore cache error
	if cacheErr != nil {
		return
	}

	for _, function := range functions {
		function.Normalize()
		abiMap.SetValue(function.Encoding, &function)
	}

	return true
}

func writeString(writer *bufio.Writer, str *string) (err error) {
	err = binary.Write(writer, binary.LittleEndian, uint64(len(*str)))
	if err != nil {
		return
	}
	err = binary.Write(writer, binary.LittleEndian, []byte(*str))
	if err != nil {
		return
	}
	return
}

func writeDefaultHeader(writer *bufio.Writer, className string) (err error) {
	err = binary.Write(writer, binary.LittleEndian, uint64(0))
	if err != nil {
		return
	}
	err = binary.Write(writer, binary.LittleEndian, uint64(41000))
	if err != nil {
		return
	}
	err = binary.Write(writer, binary.LittleEndian, uint64(1))
	if err != nil {
		return
	}

	err = writeString(writer, &className)
	if err != nil {
		return
	}
	return
}

func writeFunction(writer *bufio.Writer, function *types.Function) (err error) {
	err = writeDefaultHeader(writer, "CFunction")
	if err != nil {
		return
	}

	err = writeString(writer, &function.Name)
	if err != nil {
		return
	}

	err = writeString(writer, &function.FunctionType)
	if err != nil {
		return
	}

	var unused string
	err = writeString(writer, &unused)
	if err != nil {
		return
	}

	err = binary.Write(writer, binary.LittleEndian, &function.Anonymous)
	if err != nil {
		return
	}

	err = binary.Write(writer, binary.LittleEndian, &function.Constant)
	if err != nil {
		return
	}

	err = writeString(writer, &function.StateMutability)
	if err != nil {
		return
	}

	err = writeString(writer, &function.Signature)
	if err != nil {
		return
	}

	err = writeString(writer, &function.Encoding)
	if err != nil {
		return
	}

	err = writeArray(writer, function.Inputs, writeParameter)
	if err != nil {
		return
	}

	err = writeArray(writer, function.Outputs, writeParameter)
	if err != nil {
		return
	}

	return
}

func writeParameter(writer *bufio.Writer, param *types.Parameter) (err error) {
	err = writeDefaultHeader(writer, "CParameter")
	if err != nil {
		return
	}

	err = writeString(writer, &param.ParameterType)
	if err != nil {
		return
	}

	err = writeString(writer, &param.Name)
	if err != nil {
		return
	}

	err = writeString(writer, &param.StrDefault)
	if err != nil {
		return
	}

	jsonValue, err := json.Marshal(&param.Value)
	if err != nil {
		return
	}
	strValue := string(jsonValue)
	if err = writeString(writer, &strValue); err != nil {
		return
	}

	err = binary.Write(writer, binary.LittleEndian, &param.Indexed)
	if err != nil {
		return
	}

	err = writeString(writer, &param.InternalType)
	if err != nil {
		return
	}

	err = writeArray(writer, param.Components, writeParameter)
	if err != nil {
		return
	}

	unused1 := false
	err = binary.Write(writer, binary.LittleEndian, &unused1)
	if err != nil {
		return
	}

	unused2 := uint64(0)
	err = binary.Write(writer, binary.LittleEndian, &unused2)
	if err != nil {
		return
	}

	return
}

func writeAddress(writer *bufio.Writer, address *base.Address) (err error) {
	value := address.Hex()[:2] + strings.ToLower(address.Hex()[2:])
	if value == "0x0000000000000000000000000000000000000000" {
		value = "0x0"
		return writeString(writer, &value)
	}
	return writeString(writer, &value)
}

// writeAbis writes ABI cache (known.bin)
func writeAbis(writer *bufio.Writer, abis []types.Function) (err error) {
	err = writeDefaultHeader(writer, "CAbi")
	if err != nil {
		return
	}

	// This address is always empty
	address := base.Address{}
	if err = writeAddress(writer, &address); err != nil {
		return
	}
	err = writeArray(writer, abis, writeFunction)
	return
}

// setAbis writes ABIs to the cache
func setAbis(chain string, abis []types.Function) (err error) {
	buf := bytes.Buffer{}
	writer := bufio.NewWriter(&buf)
	if err = writeAbis(writer, abis); err != nil {
		return err
	}
	if err = writer.Flush(); err != nil {
		return err
	}

	fullPath := PathToAbisCache(chain, "known.bin")
	var f *os.File
	if file.FileExists(fullPath) {
		// If file doesn't exist, we don't need a lock
		f, err = os.OpenFile(fullPath, os.O_RDWR|os.O_CREATE, 0666)
		if err != nil {
			return
		}
		defer f.Close()
	} else {
		f, err = file.WaitOnLock(fullPath, file.DefaultOpenFlags)
		if err != nil {
			return
		}
		defer f.Close()
		if err = file.Empty(f); err != nil {
			return
		}
	}

	reader := bytes.NewReader(buf.Bytes())
	_, err = io.Copy(f, reader)
	return
}

// LoadKnownAbis loads known ABI files into abiMap, refreshing binary cache if needed
func (abiMap *SelectorSyncMap) LoadKnownAbis(chain string) (err error) {
	isUpToDate := func(chain string) (bool, error) {
		testFn := PathToAbisCache(chain, "known.bin")
		testDir := PathToKnownAbis("")
		if cacheFile, err := os.Stat(testFn); os.IsNotExist(err) {
			return false, nil

		} else if err != nil {
			return false, err

		} else {
			if newestFile, err := file.GetNewestInDirectory(testDir); err != nil {
				return false, err
			} else {
				return cacheFile.ModTime().Unix() >= newestFile.ModTime().Unix(), nil
			}
		}
	}

	useCache, err := isUpToDate(chain)
	if err != nil {
		return
	}

	if useCache {
		if loaded := loadCache(chain, abiMap); loaded {
			return
		}
	}

	paths, err := getKnownAbiPaths()
	if err != nil {
		return
	}

	for _, filePath := range paths {
		loadErr := loadAbiFromKnownFile(filePath, abiMap)
		if loadErr != nil {
			return fmt.Errorf("%s: %w", filePath, loadErr)
		}
	}

	toCache := abiMap.Values()
	return setAbis(chain, toCache)
}

func getKnownAbiPaths() (filePaths []string, err error) {
	knownDirPath := PathToKnownAbis("")
	err = filepath.WalkDir(knownDirPath, func(path string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}

		if d.IsDir() {
			return nil
		}
		if filepath.Ext(d.Name()) != ".json" {
			return nil
		}
		info, err := d.Info()
		if err != nil {
			return err
		}
		if info.Size() == 0 {
			return nil
		}

		filePaths = append(filePaths, path)
		return nil
	})
	return
}

// loadAbiFromAddress loads ABI from local file or cache
func loadAbiFromAddress(conn *rpc.Connection, address base.Address, abiMap *SelectorSyncMap) error {
	var err error
	localFileName := address.Hex() + ".json"
	localFile, err := os.OpenFile(localFileName, os.O_RDONLY, 0)
	if os.IsNotExist(err) {
		chain := conn.Chain
		loadedAbis, err := getAbi(chain, address)
		if err != nil {
			return err
		}

		for _, function := range loadedAbis {
			function.Normalize()
			abiMap.SetValue(function.Encoding, &function)
		}

		return nil
	}
	if err != nil {
		// There was different error, we may want to report it
		return err
	}
	defer localFile.Close()

	// Local file found
	if err = fromJson(localFile, abiMap); err != nil {
		return err
	}
	// File is correct, cache it
	if err = insertAbiNative(conn, address, localFile); err != nil {
		return err
	}

	return err
}

// insertAbiNative caches ABI using the native TrueBlocks caching system
func insertAbiNative(conn *rpc.Connection, address base.Address, inputReader io.Reader) error {
	chain := conn.Chain
	return insertAbi(chain, address, inputReader)
}

// getAbi returns single ABI per address. ABI-per-address are stored as JSON, not binary.
func getAbi(chain string, address base.Address) (simpleAbis []types.Function, err error) {
	fullPath := PathToAbisCache(chain, address.Hex()+".json")
	f, err := os.OpenFile(fullPath, os.O_RDONLY, 0)
	if err != nil {
		return
	}
	defer f.Close()

	ethAbi, err := abi.JSON(f)
	if err != nil {
		return
	}

	functions := make([]types.Function, 0, len(ethAbi.Methods))
	for _, method := range ethAbi.Methods {
		functions = append(functions, *types.FunctionFromAbiMethod(&method))
	}

	events := make([]types.Function, 0, len(ethAbi.Events))
	for _, event := range ethAbi.Events {
		events = append(events, *types.FunctionFromAbiEvent(&event))
	}

	simpleAbis = append(functions, events...)
	return
}

// insertAbi copies file (e.g. opened local file) into cache - DEPRECATED: Use insertAbiNative for new code
func insertAbi(chain string, address base.Address, inputReader io.Reader) error {
	fullPath := PathToAbisCache(chain, address.Hex()+".json")
	if file, err := os.OpenFile(fullPath, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0666); err != nil {
		return err
	} else {
		defer file.Close()
		if _, err = io.Copy(file, inputReader); err != nil {
			return err
		}
		return nil
	}
}

