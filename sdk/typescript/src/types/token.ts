/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { address, blknum, datetime, float64, int256, timestamp, TokenType, txnum, uint64 } from '.';

export type Token = {
  address: address
  balance: int256
  balanceDec?: float64
  blockNumber: blknum
  date?: datetime
  decimals: uint64
  diff?: int256
  holder: address
  name: string
  priorBalance?: int256
  symbol: string
  timestamp: timestamp
  totalSupply: int256
  transactionIndex?: txnum
  type: TokenType
}
