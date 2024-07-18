/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { address, blknum, datetime, float, hash, int256, lognum, timestamp, txnum, uint64 } from '.';

export type Statement = {
  accountedFor: address
  amountIn?: int256
  amountNet?: int256
  amountOut?: int256
  assetAddr: address
  assetSymbol: string
  begBal: int256
  begBalDiff?: int256
  blockNumber: blknum
  correctingIn?: int256
  correctingOut?: int256
  correctingReason?: string
  date?: datetime
  decimals: uint64
  endBal: int256
  endBalCalc?: int256
  endBalDiff?: int256
  gasOut?: int256
  internalIn?: int256
  internalOut?: int256
  logIndex: lognum
  minerBaseRewardIn?: int256
  minerNephewRewardIn?: int256
  minerTxFeeIn?: int256
  minerUncleRewardIn?: int256
  prefundIn?: int256
  prevBal?: int256
  priceSource: string
  recipient: address
  reconciled?: boolean
  reconciliationType?: string
  selfDestructIn?: int256
  selfDestructOut?: int256
  sender: address
  spotPrice: float
  timestamp: timestamp
  totalIn?: int256
  totalOut?: int256
  totalOutLessGas?: int256
  transactionHash: hash
  transactionIndex: txnum
}
