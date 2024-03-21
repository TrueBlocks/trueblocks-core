/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, blknum, Token } from '../types';

export function getTokens(
  parameters?: {
    addrs: address[],
    blocks?: blknum[],
    parts?: string[],
    byAcct?: boolean,
    changes?: boolean,
    noZero?: boolean,
    fmt?: string,
    chain: string,
    noHeader?: boolean,
    cache?: boolean,
    decache?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Token[]>(
    { endpoint: '/tokens', method: 'get', parameters, options },
  );
}
