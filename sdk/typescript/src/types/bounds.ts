/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { RawAppearance, timestamp, uint64 } from '.';

export type Bounds = {
  count: uint64
  firstApp: RawAppearance
  firstTs: timestamp
  latestApp: RawAppearance
  latestTs: timestamp
}
