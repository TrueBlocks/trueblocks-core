/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { address, blknum, bytes, datetime, float64, Function, gas, hash, Receipt, Statement, StorageSlot, timestamp, Trace, txnum, uint64, wei } from '.';

export type Transaction = {
  accessList: StorageSlot[]
  articulatedTx: Function
  blockHash: hash
  blockNumber: blknum
  chainId: string
  compressedTx?: string
  date?: datetime
  ether?: float64
  from: address
  gas: gas
  gasPrice: gas
  gasUsed: gas
  hasToken: boolean
  hash: hash
  input: bytes
  isError: boolean
  maxFeePerGas: gas
  maxPriorityFeePerGas: gas
  nonce: uint64
  receipt: Receipt
  statements?: Statement[]
  timestamp: timestamp
  to: address
  traces: Trace[]
  transactionIndex: txnum
  type: string
  value: wei
}
