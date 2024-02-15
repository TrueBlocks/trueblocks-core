// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// Package exportPkg handles the chifra export command. It The  tools provides a major part of the functionality of the TrueBlocks system. Using the index of appearances created with chifra scrape and the list of transaction identifiers created with chifra list,  completes the actual extraction of an address's transactional history from the node. You may use topics, fourbyte values at the start of a transaction's input data, and/or a log's source address or emitter to filter the results. You may also choose which portions of the Ethereum data structures (--transactions, --logs, --traces, etc.) as you wish. By default, the results of the extraction are delivered to your console, however, you may export the results to any database (with a little bit of work). The format of the data, its content and its destination are up to you. 
package exportPkg

