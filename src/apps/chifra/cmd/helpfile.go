// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */

package cmd

// helpText is the help displayed with chifra --help
const helpText = `  Accounts:
    list          list every appearance of an address anywhere on the chain
    export        export full details of transactions for one or more addresses
    monitors      add, remove, clean, and list address monitors
    names         query addresses or names of well-known accounts
    abis          fetches the ABI for a smart contract
  Chain Data:
    blocks        retrieve one or more blocks from the chain or local cache
    transactions  retrieve one or more transactions from the chain or local cache
    receipts      retrieve receipts for the given transaction(s)
    logs          retrieve logs for the given transaction(s)
    traces        retrieve traces for the given transaction(s)
    when          find block(s) based on date, blockNum, timestamp, or 'special'
  Chain State:
    state         retrieve account balance(s) for one or more addresses at given block(s)
    tokens        retrieve token balance(s) for one or more addresses at given block(s)
  Admin:
    config        report on and edit the configuration of the TrueBlocks system
    status        report on the state of the internal binary caches
    daemon        initialize and control long-running processes such as the API and the scrapers
    scrape        scan the chain and update the TrueBlocks index of appearances
    chunks        manage, investigate, and display the Unchained Index
    init          initialize the TrueBlocks system by downloading the Unchained Index from IPFS
  Other:
    explore       open a local or remote explorer for one or more addresses, blocks, or transactions
    slurp         fetch data from Etherscan for any address
  Flags:
    -h, --help    display this help screen

  Use "chifra [command] --help" for more information about a command.
`
