// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// Package scrapePkg handles the chifra scrape command. It The  application creates TrueBlocks' chunked index of address appearances -- the fundamental data structure of the entire system. It also, optionally, pins each chunk of the index to IPFS.  is a long running process, therefore we advise you run it as a service or in terminal multiplexer such as tmux. You may start and stop  as needed, but doing so means the scraper will not be keeping up with the front of the blockchain. The next time it starts, it will have to catch up to the chain, a process that may take several hours depending on how long ago it was last run. See the section below and the "Papers" section of our website for more information on how the scraping process works and prerequisites for it proper operation. You may adjust the speed of the index creation with the --sleep and --block_cnt options. On some machines, or when running against some EVM node software, the scraper may overburden the hardware. Slowing things down will ensure proper operation. Finally, you may optionally --pin each new chunk to IPFS which naturally shards the database among all users. By default, pinning is against a locally running IPFS node, but the --remote option allows pinning to an IPFS pinning service such as Pinata or Estuary. 
package scrapePkg
