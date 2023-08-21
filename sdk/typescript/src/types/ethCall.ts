/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum } from '.';

export type EthCall = {
  blockNumber: blknum
  address: address
  signature: string
  encoding: string
  bytes: string
  compressedResult: string
  deployed: blknum
}
