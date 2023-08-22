/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, blknum, Result, State } from '../types';

export function getState(
  parameters?: {
    addrs: address[],
    blocks?: blknum[],
    parts?: string[],
    changes?: boolean,
    noZero?: boolean,
    call?: string,
    articulate?: boolean,
    proxyFor?: address,
    chain: string,
    noHeader?: boolean,
    fmt?: string,
    verbose?: boolean,
    ether?: boolean,
    raw?: boolean,
    cache?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Result[] | State[]>(
    { endpoint: '/state', method: 'get', parameters, options },
  );
}
