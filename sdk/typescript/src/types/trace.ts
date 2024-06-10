/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import { blknum, datetime, Function, hash, timestamp, TraceAction, TraceResult, uint64 } from '.';

export type Trace = {
  blockHash: hash
  blockNumber: blknum
  subtraces: uint64
  traceAddress: uint64[]
  transactionHash: hash
  transactionIndex: blknum
  type?: string
  error?: string
  action: TraceAction
  result: TraceResult
  articulatedTrace?: Function
  compressedTrace?: string
  timestamp: timestamp
  date: datetime
}

