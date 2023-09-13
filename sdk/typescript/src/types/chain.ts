/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { uint64 } from '.';

export type Chain = {
  chain: string
  chainId: uint64
  symbol: string
  rpcProvider: string
  remoteExplorer: string
  localExplorer: string
  ipfsGateway: string
}
