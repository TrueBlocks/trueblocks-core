package main

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

import (
	"log"
	"sync"

	scraper "github.com/TrueBlocks/trueblocks-core/src/go-apps/flame-scrape/scraper"
)

func main() {
	// Handle command line options
	err := scraper.ParseOptions()
	if err != nil {
		log.Println("Could not parse command line.")
		return
	}

<<<<<<< HEAD
=======
	// Start listening on web sockets
	scraper.RunWebsocketPool()

>>>>>>> acd73f267a2b86e8ddb51a418950cbbbfa9eb730
	var wg sync.WaitGroup
	wg.Add(2)
	go scraper.RunIndexScraper(wg)
	go scraper.RunMonitorScraper(wg)
	wg.Wait()
}
