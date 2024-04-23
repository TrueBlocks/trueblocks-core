// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// slurpPkg implements the chifra slurp command.
//
// chifra slurp is the first tool we built in the Ethereum space. It even has its [own website](http://ethslurp.com).
//
// While it's useful, it has two shortcomings. First, it is fully centralized, pulling its data from
// [http://etherscan.io](http://etherscan.io). Second, is that it does not report every transaction
// for a given account. This is actually a shortcoming with API providers. It's too complicated to explain
// here, but see our blog.
//
// While chifra slurp has its shortcomings, it does provides some nice features. You may use it to pull
// any transaction initiated by an EOA for example or to explore mining rewards. Visit the above
// referenced website for more information.
//
// Currently supported API providers:
// - [TrueBlocks Key](https://key.trueblocks.io)
// - Etherscan
// - Covalent
// - Alchemy
package slurpPkg
