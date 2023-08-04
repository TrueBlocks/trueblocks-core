/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { Abi, address, blknum, Function } from '.';

export type EthCall = {
  blockNumber: blknum
  address: address
  signature: string
  encoding: string
  bytes: string
  abi_spec: Abi
  callResult: Function
  compressedResult: string
  deployed: blknum
}
