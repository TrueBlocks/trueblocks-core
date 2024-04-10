// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// tokensPkg implements the chifra tokens command.
//
// Given the address of an ERC20 token contract, the chifra tokens tool reports token balances for one or
// more additional addresses. Alternatively, the tool can report the token balances for multiple ERC20
// tokens for a single addresses.
//
// In normal operation the **first item** in the address_list is assumed to be an ERC20 token
// contract whose balances are being queried, whereas the remainder of the list is assumed to be
// addresses on which to report.
//
// In --byAcct mode, **all addresses** in the address_list are assumed to be ERC20 token contracts,
// except the final one which is the account whose token balances are reported.
//
// You may optionally specify one or more blocks at which to report. If no block is specified, the
// latest block is assumed. You may also optionally specify which parts of the token data to extract.
package tokensPkg
