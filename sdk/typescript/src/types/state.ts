/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { address, blknum, datetime, float64, StatePart, timestamp, uint64, wei } from '.';

export type State = {
  accountType: string
  address: address
  balance: wei
  blockNumber: blknum
  code: string
  date?: datetime
  deployed: blknum
  ether?: float64
  nonce: uint64
  parts: StatePart
  proxy: address
  timestamp: timestamp
}
