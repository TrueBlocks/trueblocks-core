/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
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
