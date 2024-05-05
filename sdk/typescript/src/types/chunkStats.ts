/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { blkrange, datetime, float64, uint64 } from '.';

export type ChunkStats = {
  range: blkrange
  rangeEnd: datetime
  nAddrs: uint64
  nApps: uint64
  nBlocks: uint64
  nBlooms: uint64
  recWid: uint64
  bloomSz: uint64
  chunkSz: uint64
  addrsPerBlock: float64
  appsPerBlock: float64
  appsPerAddr: float64
  ratio: float64
}
