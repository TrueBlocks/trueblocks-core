/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import { address, blknum, datetime, timestamp, uint64, wei } from '.';

export type State = {
  blockNumber: blknum
  timestamp: timestamp
  date: datetime
  address: address
  accountType: string
  balance: wei
  code: string
  deployed: blknum
  nonce: uint64
  proxy: address
}

