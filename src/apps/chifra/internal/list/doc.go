// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// listPkg implements the chifra list command.
//
// chifra list takes one or more addresses, queries the index of appearances, and builds TrueBlocks
// monitors. A TrueBlocks monitor is a file that contains blockNumber.transactionIndex pairs (transaction
// identifiers) representing the history of the address.
//
// Because TrueBlocks only extracts data from the Ethereum node when it's requested, the first time
// you list an address it takes about a minute. Subsequent queries are much faster because TrueBlocks
// caches the results.
//
// Note that chifra list only queries the index, it does not extract the full transactional details.
// You may use chifra export for that.
package listPkg
