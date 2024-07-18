/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { blknum, datetime, timestamp, uint64 } from '.';

export type BlockCount = {
  addressCnt?: uint64
  blockNumber: blknum
  date?: datetime
  logsCnt?: uint64
  timestamp: timestamp
  tracesCnt?: uint64
  transactionsCnt: uint64
  unclesCnt?: uint64
  withdrawalsCnt?: uint64
}
