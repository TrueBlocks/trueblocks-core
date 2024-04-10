// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// The chifra status tool reports on the state (and size) of the various TrueBlocks local binary caches. TrueBlocks produces nine difference caches: abis, blocks, monitors, names, objs, recons, slurps, traces, transactions. In general practice, these caches may take up a few GB of hard drive space, however, for very popular smart contract the size of the caches may grow rather large. Keep an eye on it. The chifra status program allows you to manage the various TrueBlocks caches. You may list all of the caches, some of the cache, or even individual caches either in terse or full detail. The cache of interest is specified with the modes option. TrueBlocks maintains caches for the index of address appearances, named addresses, abi files, as well as other data including blockchain data, and address monitors.
package statusPkg
