/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, int256, uint64, wei } from '.';

export type TokenBalance = {
  blockNumber: blknum
  date: string
  totalSupply: wei
  transactionIndex: blknum
  holder: address
  priorBalance: wei
  balance: wei
  diff: int256
  address: address
  name: string
  symbol: string
  decimals: uint64
  isContract: boolean
  isErc20: boolean
  isErc721: boolean
}
