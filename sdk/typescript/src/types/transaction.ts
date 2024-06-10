/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import { address, blknum, bytes, datetime, Function, gas, hash, Receipt, Statement, timestamp, Trace, uint64, wei } from '.';

export type Transaction = {
  chainId: string
  blockNumber: blknum
  transactionIndex: blknum
  timestamp: timestamp
  date: datetime
  hash: hash
  blockHash: hash
  from: address
  to: address
  nonce: uint64
  value: wei
  gas: gas
  gasPrice: gas
  maxFeePerGas: gas
  maxPriorityFeePerGas: gas
  input: bytes
  isError: boolean
  hasToken: boolean
  receipt: Receipt
  traces: Trace[]
  articulatedTx: Function
  compressedTx: string
  statements: Statement[]
  gasUsed: gas
  type: string
}

