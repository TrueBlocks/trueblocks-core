/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { blkrange, int64, ipfshash } from '.';

export type ChunkRecord = {
  range: blkrange
  bloomHash: ipfshash
  indexHash: ipfshash
  bloomSize: int64
  indexSize: int64
}
