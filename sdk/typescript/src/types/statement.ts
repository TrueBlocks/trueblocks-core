/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, datetime, float64, hash, int256, timestamp, uint64 } from '.';

export type Statement = {
  blockNumber: blknum
  transactionIndex: blknum
  logIndex: blknum
  transactionHash: hash
  timestamp: timestamp
  date: datetime
  assetAddr: address
  assetSymbol: string
  decimals: uint64
  spotPrice: float64
  priceSource: string
  accountedFor: address
  sender: address
  recipient: address
  begBal: int256
  amountNet: int256
  endBal: int256
  reconciliationType: string
  reconciled: boolean
  totalIn: int256
  amountIn?: int256
  internalIn?: int256
  selfDestructIn?: int256
  minerBaseRewardIn?: int256
  minerNephewRewardIn?: int256
  minerTxFeeIn?: int256
  minerUncleRewardIn?: int256
  correctingIn?: int256
  prefundIn?: int256
  totalOut: int256
  amountOut?: int256
  internalOut?: int256
  correctingOut?: int256
  selfDestructOut?: int256
  gasOut?: int256
  totalOutLessGas: int256
  prevBal?: int256
  begBalDiff?: int256
  endBalDiff?: int256
  endBalCalc?: int256
  correctingReason?: string
}
