/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { blkrange, hash, uint64 } from '.';

export type ChunkBloom = {
  range: blkrange
  magic: string
  hash: hash
  nBlooms: uint64
  nInserted: uint64
  size: uint64
  byteWidth: uint64
}
