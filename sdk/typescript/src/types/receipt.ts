/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, gas, hash, Log, uint32, wei } from '.';

export type Receipt = {
  blockHash?: hash
  blockNumber: blknum
  contractAddress?: address
  cumulativeGasUsed?: wei
  from?: address
  gasUsed: gas
  effectiveGasPrice?: gas
  isError?: boolean
  logs: Log[]
  logsBloom?: string
  status: uint32
  to?: address
  transactionHash: hash
  transactionIndex: blknum
}
