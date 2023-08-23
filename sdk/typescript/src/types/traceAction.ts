/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, bytes, gas, wei } from '.';

export type TraceAction = {
  selfDestructed?: address
  balance?: wei
  callType: string
  from: address
  gas: gas
  init?: string
  input?: bytes
  refundAddress?: address
  rewardType?: string
  to: address
  value: wei
  address?: address
  author?: address
}
