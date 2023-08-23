/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { CacheItem, Chain, timestamp } from '.';

export type Config = {
  clientVersion: string
  clientIds: string
  trueblocksVersion: string
  rpcProvider: string
  configPath: string
  cachePath: string
  indexPath: string
  host: string
  isTesting: boolean
  isApi: boolean
  isScraping: boolean
  isArchive: boolean
  isTracing: boolean
  hasEskey: boolean
  hasPinkey: boolean
  ts: timestamp
  chains: Chain[]
  caches: CacheItem[]
}
