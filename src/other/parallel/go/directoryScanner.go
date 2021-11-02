/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
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
