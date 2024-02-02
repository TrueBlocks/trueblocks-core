/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { int64, ipfshash } from '.';

export type IpfsPin = {
  cid: ipfshash
  datePinned: string
  status: string
  size: int64
  fileName: string
}

