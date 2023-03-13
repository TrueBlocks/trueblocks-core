package abi

import (
	"archive/zip"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	"os/exec"
	"path"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type generatorSource struct {
	Name        string
	Url         string
	SourceDir   string
	SourceFile  string
	InstallDeps installDepsCmd
}

type installDepsCmd struct {
	Exec string
	Args []string
}

var defaultSources = []generatorSource{
	{
		Name:       "beaconproxy",
		Url:        "https://github.com/OpenZeppelin/openzeppelin-contracts",
		SourceDir:  "contracts/proxy/beacon",
		SourceFile: "BeaconProxy.sol",
	},
	// {
	// 	Name:       "gitcoin_bulkcheckout",
	// 	Url:        "https://github.com/gitcoinco/BulkTransactions",
	// 	SourceDir:  "contracts",
	// 	SourceFile: "BulkCheckout.sol",
	// 	InstallDeps: installDepsCmd{
	// 		Exec: "npm",
	// 		Args: []string{"i", "-D"},
	// 	},
	// },
}

var outputDir = path.Join(os.TempDir(), "abicachegen")
var cacheOutput = path.Join(outputDir, "cache")

type pathList struct {
	ZipArchive       string
	UnpackedZip      string
	SolcInputDir     string
	SolcOutputDir    string
	SolcCombinedFile string
	CacheFile        string
}

// getPaths builds paths that will be used to download, unzip, compile and read
// the source files.
func getPaths(source *generatorSource) *pathList {
	name := source.Name
	unpackedZipDirName := name + ".dir"
	solcOutputDir := path.Join(outputDir, name+"_combined")
	combinedFile := strings.Replace(source.SourceFile, ".sol", ".json", 1)
	return &pathList{
		ZipArchive:       path.Join(outputDir, name+".zip"),
		UnpackedZip:      path.Join(outputDir, unpackedZipDirName),
		SolcInputDir:     path.Join(outputDir, unpackedZipDirName, source.SourceDir),
		SolcOutputDir:    solcOutputDir,
		SolcCombinedFile: path.Join(solcOutputDir, source.SourceFile, combinedFile),
		CacheFile:        path.Join(cacheOutput, name+".json"),
	}
}

// cleanAllInputs removes all created files but cache (output) file
func cleanAllInputs(pl *pathList) error {
	paths := []string{
		pl.ZipArchive,
		pl.UnpackedZip,
		// pl.SolcInput,
		pl.SolcOutputDir,
		pl.SolcCombinedFile,
	}
	for _, inputPath := range paths {
		err := os.RemoveAll(inputPath)
		if err != nil {
			return err
		}
	}
	return nil
}

// Fetch fetches source files from GitHub
func Fetch(source *generatorSource, pl *pathList, forceDownload bool) (err error) {
	if !forceDownload && file.FileExists(pl.ZipArchive) {
		return nil
	}

	if !strings.Contains(source.Url, "github.com") {
		return fmt.Errorf("unsupported provider: %s", source.Url)
	}

	url := strings.Replace(source.Url, "github.com", "api.github.com/repos", 1)
	response, err := http.Get(url + "/zipball")
	if err != nil {
		return
	}
	defer response.Body.Close()

	if response.StatusCode != 200 {
		return fmt.Errorf("cannot fetch source %s: %d", url, response.StatusCode)
	}

	outputFile, err := os.OpenFile(pl.ZipArchive, os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0666)
	if err != nil {
		return
	}

	logger.Info("Downloading", source.Name, "to", pl.ZipArchive)

	if _, err = io.Copy(outputFile, response.Body); err != nil {
		return
	}

	return
}

func runUnzip(pl *pathList) error {
	// read root dir
	zipFile, err := os.OpenFile(pl.ZipArchive, os.O_RDONLY, 0)
	if err != nil {
		return err
	}
	defer zipFile.Close()
	info, err := zipFile.Stat()
	if err != nil {
		return err
	}
	zipReader, err := zip.NewReader(zipFile, info.Size())
	if err != nil {
		return err
	}
	rootDirName := zipReader.File[0].Name

	// unzip
	unzip := exec.Command("unzip", "-qu", pl.ZipArchive)
	unzip.Dir = outputDir
	unzip.Stderr = os.Stderr
	unzip.Stdout = os.Stdout
	if err = unzip.Run(); err != nil {
		return err
	}

	// move the output, so whe know where it is
	err = os.RemoveAll(pl.UnpackedZip)
	if err != nil && !os.IsNotExist(err) {
		return err
	}
	return os.Rename(path.Join(outputDir, rootDirName), pl.UnpackedZip)
}

func runSolc(pl *pathList, source *generatorSource) (err error) {
	if source.InstallDeps.Exec != "" {
		// Sometimes we need to install dependencies to be able to compile .sol files
		installDeps := exec.Command(source.InstallDeps.Exec, source.InstallDeps.Args...)
		installDeps.Dir = pl.UnpackedZip
		err = installDeps.Run()
		if err != nil {
			logger.Error("install deps error:", err)
		}
		// We continue, because error doesn't mean we can't build
	}
	forge := exec.Command("forge", "build", "--skip", "test", "-c", pl.SolcInputDir, "-o", pl.SolcOutputDir)
	forge.Dir = pl.UnpackedZip
	forge.Stderr = os.Stderr
	forge.Stdout = os.Stdout
	return forge.Run()
}

// buildCacheFile produces a file in the same format as found in abis/known-0xy/file.json
func buildCacheFile(pl *pathList) (err error) {
	inputFile, err := os.OpenFile(pl.SolcCombinedFile, os.O_RDONLY, 0)
	if err != nil {
		return
	}
	defer inputFile.Close()
	outputFile, err := os.OpenFile(
		pl.CacheFile,
		os.O_WRONLY|os.O_CREATE|os.O_TRUNC,
		0666,
	)
	if err != nil {
		return
	}
	defer outputFile.Close()
	funcs, err := GenerateFromCombinedAst(inputFile)
	if err != nil {
		return
	}
	b, err := json.MarshalIndent(funcs, "", " ")
	if err != nil {
		return
	}
	if _, err = outputFile.Write(b); err != nil {
		return
	}
	return
}

// isPresent checks if file is already present
func isPresent(filePath string) bool {
	_, err := os.Stat(filePath)
	return !os.IsNotExist(err)
}

func RegenerateAll() (err error) {
	if err = file.EstablishFolder(outputDir); err != nil && !os.IsExist(err) {
		return
	}
	if err = file.EstablishFolder(cacheOutput); err != nil && !os.IsExist(err) {
		return
	}
	for _, source := range defaultSources {
		pl := getPaths(&source)
		if isPresent(pl.CacheFile) {
			logger.Warn("Cache file for", source.Name, "already present, skipping")
			continue
		}
		logger.Info("Downloading")
		err = Fetch(&source, pl, false)
		if err != nil {
			return err
		}
		logger.Info("Unpacking")
		if err = runUnzip(pl); err != nil {
			return err
		}
		logger.Info("Compiling")
		err = runSolc(pl, &source)
		if err != nil {
			return err
		}
		logger.Info("Building known ABI file")
		if err = buildCacheFile(pl); err != nil {
			return err
		}
		logger.Info("Built", source.Name)
		cleanAllInputs(pl)
	}
	return
}
