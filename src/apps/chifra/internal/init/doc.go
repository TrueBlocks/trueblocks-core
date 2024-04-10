// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// When invoked, chifra init reads a value from a smart contract called **The Unchained Index** (0x0c316b7042b419d07d343f2f4f5bd54ff731183d). This value (manifestHashMap) is an IPFS hash pointing to a pinned file (called the Manifest) that contains a large collection of other IPFS hashes. These other hashes point to each of the Bloom filter and Index Chunk. TrueBlocks periodically publishes the Manifest's hash to the smart contract. This makes the index available for our software to use and impossible for us to withhold. Both of these aspects of the manifest are by design. If you stop chifra init before it finishes, it will pick up again where it left off the next time you run it. Certain parts of the system (chifra list and chifra export for example) if you have not previously run chifra init or chifra scrape. You will be warned by the system until it's satisfied. If you run chifra init and allow it to complete, the next time you run chifra scrape, it will start where init finished. This means that only the blooms will be stored on your hard drive. Subsequent scraping will produce both chunks and blooms, although you can, if you wish delete chunks that are not being used. You may periodically run chifra init if you prefer not to scrape.
package initPkg
