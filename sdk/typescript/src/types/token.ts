/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import { address, blknum, datetime, int256, timestamp, TokenType, uint64 } from '.';

export type Token = {
  blockNumber: blknum
  transactionIndex?: blknum
  timestamp: timestamp
  date: datetime
  totalSupply: int256
  address: address
  holder: address
  priorBalance?: int256
  balance: int256
  diff?: int256
  name: string
  symbol: string
  decimals: uint64
  type: TokenType
}

