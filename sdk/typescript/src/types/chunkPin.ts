/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { ipfshash } from '.';

export type ChunkPin = {
  version: string
  chain: string
  timestampHash: ipfshash
  manifestHash: ipfshash
  specHash: ipfshash
}
