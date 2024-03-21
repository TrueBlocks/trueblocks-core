/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, blknum, double, Slurp } from '../types';

export function getSlurp(
  parameters?: {
    addrs: address[],
    blocks?: blknum[],
    types?: string[],
    appearances?: boolean,
    articulate?: boolean,
    source?: 'etherscan*' | 'key',
    count?: boolean,
    sleep?: double,
    fmt?: string,
    chain: string,
    noHeader?: boolean,
    cache?: boolean,
    decache?: boolean,
    ether?: boolean,
    raw?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Slurp[]>(
    { endpoint: '/slurp', method: 'get', parameters, options },
  );
}
