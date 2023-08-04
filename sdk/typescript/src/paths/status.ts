/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { CacheItem, Chain, Config, uint64 } from '../types';

export function getStatus(
  parameters?: {
    modes?: string[],
    firstRecord?: uint64,
    maxRecords?: uint64,
    chain: string,
    noHeader?: boolean,
    fmt?: string,
    verbose?: boolean,
    logLevel?: number,
    wei?: boolean,
    ether?: boolean,
    dollars?: boolean,
    raw?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<CacheItem[] | Chain[] | Config[]>(
    { endpoint: '/status', method: 'get', parameters, options },
  );
}
