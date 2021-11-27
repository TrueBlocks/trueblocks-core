// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"path"
	"sync"
)

type directoryScanner struct {
	rootDirectory    string
	dirScanWaitGroup sync.WaitGroup
	directoryChannel chan<- string

	walkSubdirectory bool
}

func (dirScanner *directoryScanner) waitForCompletion() {
	dirScanner.dirScanWaitGroup.Wait()
	// signal that we have no more directries available
	close(dirScanner.directoryChannel)
}

func (dirScanner *directoryScanner) scanInBackground() {

	dirScanner.dirScanWaitGroup.Add(1)

	go func() {
		defer dirScanner.dirScanWaitGroup.Done()

		dirScanner.directoryChannel <- dirScanner.rootDirectory

		if dirScanner.walkSubdirectory {

			f, err := os.Open(dirScanner.rootDirectory)

			if err != nil {
				fmt.Println("Error occured while opening directory", dirScanner.rootDirectory, err)
				return
			}

			//subDirectoriesInfo, err := ioutil.ReadDir(dirScanner.rootDirectory)
			subDirectoriesInfo, err := f.Readdir(-1)

			f.Close()

			if err != nil {
				fmt.Println("Error occured while reading directory", dirScanner.rootDirectory, err)
				return
			}
			for _, subDirectory := range subDirectoriesInfo {
				if subDirectory.IsDir() {
					subDirectoryName := subDirectory.Name()

					subDirectoryFullPath := path.Join(dirScanner.rootDirectory, subDirectoryName)

					dirScanner.directoryChannel <- subDirectoryFullPath
					walkSubdirectory(subDirectoryFullPath, dirScanner.directoryChannel)
				}
			}
		}
	}()
}

func walkSubdirectory(directoryPath string, directoriesChannel chan<- string) {

	subDirectoriesInfo, err := ioutil.ReadDir(directoryPath)

	if err != nil {
		fmt.Println("Error occured while reading directory", err)
	}

	for _, subDirectory := range subDirectoriesInfo {
		if subDirectory.IsDir() {
			subDirectoryPath := path.Join(directoryPath, subDirectory.Name())
			directoriesChannel <- subDirectoryPath
			walkSubdirectory(subDirectoryPath, directoriesChannel)
		}
	}
}
