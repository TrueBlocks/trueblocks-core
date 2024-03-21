/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { ChunkRecord, double, Manifest, uint64 } from '../types';

export function getScrape(
  parameters?: {
    blockCnt?: uint64,
    sleep?: double,
    touch?: uint64,
    chain: string,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<ChunkRecord[] | Manifest[]>(
    { endpoint: '/scrape', method: 'get', parameters, options },
  );
}
