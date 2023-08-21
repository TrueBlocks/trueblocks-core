/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { int64, uint64 } from '.';

export type CacheItem = {
  type: string
  items: any[]
  lastCached?: string
  nFiles: uint64
  nFolders: uint64
  path: string
  sizeInBytes: int64
}
