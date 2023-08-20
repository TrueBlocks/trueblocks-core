/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { blkrange, hash, uint64 } from '.';

export type ChunkIndex = {
  range: blkrange
  magic: string
  hash: hash
  nAddresses: uint64
  nAppearances: uint64
  size: uint64
}
