#!/usr/bin/env bash

make -j 12 

# This script is only used internally for testing.

# decache missing
# sdkFuzzer tokens
# sdkFuzzer slurp

# json parsing fails
sdkFuzzer export
# sdkFuzzer chunks

# reports when should not
# sdkFuzzer monitors

# passes fuzzer
# sdkFuzzer blocks
# sdkFuzzer names
# sdkFuzzer transactions
# sdkFuzzer receipts
# sdkFuzzer logs
# sdkFuzzer traces
# sdkFuzzer state
# sdkFuzzer abis
# sdkFuzzer when
# sdkFuzzer list
# sdkFuzzer config
# sdkFuzzer status

# Do these do anything?
# sdkFuzzer daemon
# sdkFuzzer scrape
# sdkFuzzer init
# sdkFuzzer explore

# //---------------------------------------------------------------------------------------------------
# export        ==> LocationsFromAddrAppsAndCacheType       ==> [flags] <address> [address...] [topics...] [fourbytes...]
# walk.Cache_Statements   ==> address,tx
# walk.Cache_Traces       ==> tx
# walk.Cache_Transactions ==> tx
# walk.Cache_Receipts     ==> block
# walk.Cache_Withdrawals  ==> block

# //---------------------------------------------------------------------------------------------------
# logs          ==> LocationsFromTransactionIds             ==> [flags] <tx_id> [tx_id...]
# walk.Cache_Logs         ==> block

# receipts      ==> LocationsFromTransactionIds             ==> [flags] <tx_id> [tx_id...]
# walk.Cache_Receipts     ==> block

# traces        ==> LocationsFromTransactionIds             ==> [flags] <tx_id> [tx_id...]
# walk.Cache_Traces       ==> tx

# transactions  ==> LocationsFromTransactionIds             ==> [flags] <tx_id> [tx_id...]
# walk.Cache_Transactions  ==> tx

# //---------------------------------------------------------------------------------------------------
# blocks        ==> LocationsFromBlockIds                   ==> [flags] <block> [block...]
# walk.Cache_Block          ==> block

# when          ==> LocationsFromBlockIds                   ==> [flags] < block | date > [ block... | date... ]
# walk.Cache_Block          ==> block

# //---------------------------------------------------------------------------------------------------
# state         ==> LocationsFromAddressEncodingAndBlockIds ==> [flags] <address> [address...] [block...]
# walk.Cache_Results        ==> address,block,fourbyte
# walk.Cache_State          ==> address,block

# tokens        ==>                                         ==> [flags] <address> <address> [address...] [block...]

# //---------------------------------------------------------------------------------------------------
# NOT FOUND
# monitors,acctExport,,,,visible|docs,command,,,Manage monitors,[flags] <address> [address...]

# abis,grabABI,,,,visible|docs,command,,,Manage Abi files,[flags] <address> [address...]

# ethslurp,,,,visible|docs,command,,,Slurp Api services,[flags] <address> [address...] [block...]
#   ../src/dev_tools/goMaker/templates/classDefinitions/slurp.toml:    cache_type = "cacheable"
#   ../src/dev_tools/goMaker/templates/classDefinitions/slurp.toml:    cache_by = "address,tx"
#   ../src/dev_tools/goMaker/templates/classDefinitions/slurp.toml:    cache_as = "group"


# ../src/dev_tools/goMaker/templates/classDefinitions/status.toml:    doc_descr = "status-related data about the TrueBlocks system including the server and local binary caches"
# ../src/dev_tools/goMaker/templates/classDefinitions/status.toml:    contains = "cacheitem, chain"
