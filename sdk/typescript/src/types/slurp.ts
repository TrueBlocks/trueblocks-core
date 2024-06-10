/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import { address, blknum, bytes, datetime, Function, gas, hash, timestamp, uint64, wei } from '.';

export type Slurp = {
  hash: hash
  blockHash: hash
  blockNumber: blknum
  transactionIndex: blknum
  nonce: uint64
  timestamp: timestamp
  date: datetime
  from: address
  to: address
  value: wei
  gas: gas
  gasPrice: gas
  input: bytes
  hasToken: boolean
  articulatedTx: Function
  compressedTx: string
  isError: boolean
  functionName: string
  methodId: string
  gasUsed: gas
  contractAddress: address
  cumulativeGasUsed: string
  txReceiptStatus: string
  ether: string
  withdrawalIndex: uint64
  validatorIndex: uint64
}

