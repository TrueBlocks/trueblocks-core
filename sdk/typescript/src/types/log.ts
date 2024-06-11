/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { address, blknum, bytes, datetime, Function, hash, lognum, timestamp, topic, txnum } from '.';

export type Log = {
  address: address
  articulatedLog?: Function
  blockHash: hash
  blockNumber: blknum
  compressedLog?: string
  data?: bytes
  date?: datetime
  logIndex: lognum
  timestamp?: timestamp
  topics?: topic[]
  transactionHash: hash
  transactionIndex: txnum
}
