/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, datetime, double, hash, timestamp, uint64, wei } from '.';

export type Transfer = {
  blockNumber: blknum
  transactionIndex: blknum
  logIndex: blknum
  transactionHash: hash
  timestamp: timestamp
  date: datetime
  sender: address
  recipient: address
  assetAddr: address
  assetSymbol: string
  decimals: uint64
  amount: wei
  spotPrice: double
  priceSource: string
  encoding: string
}
