/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, bytes, gas } from '.';

export type TraceResult = {
  address?: address
  code?: bytes
  gasUsed: gas
  output: bytes
}
