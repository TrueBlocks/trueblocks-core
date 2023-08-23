/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { datetime, RawAppearance, timestamp, uint64 } from '.';

export type Bounds = {
  count: uint64
  firstApp: RawAppearance
  firstTs: timestamp
  firstDate: datetime
  latestApp: RawAppearance
  latestTs: timestamp
  latestDate: datetime
}
