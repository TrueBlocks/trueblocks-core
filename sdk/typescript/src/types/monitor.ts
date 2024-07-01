/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, uint64 } from '.';

export type Monitor = {
  nAppearances: blknum
  lastExport: blknum
  firstAppearance: blknum
  latestAppearance: blknum
  lastVisitedBlock: blknum
  sizeInBytes?: uint64
  nApps: blknum
  firstApp: blknum
  latestApp: blknum
  address: address
  decimals: uint64
  isContract: boolean
  isCustom: boolean
  isErc20?: boolean
  isErc721?: boolean
  isPrefund?: boolean
  name: string
  source: string
  symbol: string
  tags: string
  deleted: boolean
}
