/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import { address, blknum, bytes, datetime, Function, hash, timestamp, topic, uint64 } from '.';

export type Log = {
  blockNumber: blknum
  transactionIndex: uint64
  logIndex: uint64
  timestamp?: timestamp
  date: datetime
  address: address
  topics?: topic[]
  data?: bytes
  transactionHash: hash
  blockHash: hash
  articulatedLog?: Function
  compressedLog?: string
}

