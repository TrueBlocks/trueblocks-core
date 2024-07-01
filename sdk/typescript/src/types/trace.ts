/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { blknum, datetime, Function, hash, timestamp, TraceAction, TraceResult, txnum, uint64 } from '.';

export type Trace = {
  action: TraceAction
  articulatedTrace?: Function
  blockHash: hash
  blockNumber: blknum
  compressedTrace?: string
  date?: datetime
  error?: string
  result: TraceResult
  subtraces: uint64
  timestamp: timestamp
  traceAddress: uint64[]
  transactionHash: hash
  transactionIndex: txnum
  type?: string
}
