/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { ipfshash } from '.';

export type ChunkPinReport = {
  version: string
  chain: string
  schemas: ipfshash
  tsHash: ipfshash
  manifestHash: ipfshash
}
