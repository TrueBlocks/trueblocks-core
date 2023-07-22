// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// Package namesPkg handles the chifra names command. It  is a surprisingly useful tool. It allows one to associate textual names with Ethereum addresses. One may ask why this is necessary given that ENS exists. The answer is a single word: "privacy". ENS names are public. In many cases, users desire to keep personal addresses private. Try to do this on a website. Like chifra abis, this tool is useful from the command line but is primarily used in support of other tools, especially chifra export where naming addresses becomes the single best way to turn unintelligible blockchain data into understandable information. The various options allow you to search and filter the results. The tags option is used primarily by the TrueBlocks explorer. You may use the TrueBlocks explorer to manage (add, edit, delete) address-name associations. 
package namesPkg
