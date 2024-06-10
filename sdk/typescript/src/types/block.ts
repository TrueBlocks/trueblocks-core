/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import { address, blknum, datetime, gas, hash, timestamp, Transaction, uint64, wei, Withdrawal } from '.';

export type Block = {
  author: address
  gasLimit: gas
  gasUsed: gas
  hash: hash
  blockNumber: blknum
  parentHash: hash
  receiptsRoot: hash
  sha3Uncles: hash
  size: uint64
  stateRoot: hash
  totalDifficulty: wei
  miner: address
  difficulty: uint64
  extraData: string
  logsBloom: string
  mixHash: string
  nonce: string
  timestamp: timestamp
  date: datetime
  baseFeePerGas: wei
  transactions: Transaction[]
  transactionsRoot: hash
  uncles?: hash[]
  withdrawals?: Withdrawal[]
}

