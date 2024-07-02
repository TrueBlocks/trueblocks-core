/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { address, blknum, bytes, datetime, float64, Function, gas, hash, timestamp, txnum, uint64, wei } from '.';

export type Slurp = {
  address: address
  amount: wei
  articulatedTx: Function
  blockHash: hash
  blockNumber: blknum
  compressedTx?: string
  contractAddress: address
  cumulativeGasUsed: string
  date?: datetime
  ether?: float64
  from: address
  functionName: string
  gas: gas
  gasPrice: gas
  gasUsed: gas
  hasToken: boolean
  hash: hash
  input: bytes
  isError: boolean
  methodId: string
  nonce: uint64
  timestamp: timestamp
  to: address
  transactionIndex: txnum
  txReceiptStatus: string
  validatorIndex: uint64
  value: wei
  withdrawalIndex: uint64
}
