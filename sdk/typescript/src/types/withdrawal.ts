/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { address, blknum, datetime, float64, timestamp, uint64, wei } from '.';

export type Withdrawal = {
  address: address
  amount: wei
  blockNumber: blknum
  date?: datetime
  ether?: float64
  index: uint64
  timestamp: timestamp
  validatorIndex: uint64
}
