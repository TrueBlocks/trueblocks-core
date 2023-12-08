/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { CacheItem, Chain } from '.';

export type Config = {
  cachePath?: string
  caches?: CacheItem[]
  chain?: string
  chainConfig?: string
  clientVersion?: string
  chainId?: string
  hasEsKey?: boolean
  hasPinKey?: boolean
  indexPath?: string
  isApi?: boolean
  isArchive?: boolean
  isTesting?: boolean
  isTracing?: boolean
  networkId?: string
  progress?: string
  rootConfig?: string
  rpcProvider?: string
  version?: string
  chains?: Chain[]
}
