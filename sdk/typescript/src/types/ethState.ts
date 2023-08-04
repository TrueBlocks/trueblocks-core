/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, bytes, uint64, wei } from '.';

export type EthState = {
  blockNumber: blknum
  address: address
  proxy: address
  balance: wei
  nonce: uint64
  code: bytes
  deployed: blknum
  accttype: string
}
