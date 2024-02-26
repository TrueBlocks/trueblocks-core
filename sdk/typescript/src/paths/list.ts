/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, Appearance, AppearanceCount, blknum, Bounds, Monitor, uint64 } from '../types';

export function getList(
  parameters?: {
    addrs: address[],
    count?: boolean,
    noZero?: boolean,
    bounds?: boolean,
    unripe?: boolean,
    silent?: boolean,
    firstRecord?: uint64,
    maxRecords?: uint64,
    reversed?: boolean,
    firstBlock?: blknum,
    lastBlock?: blknum,
    fmt?: string,
    chain: string,
    noHeader?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Appearance[] | AppearanceCount[] | Bounds[] | Monitor[]>(
    { endpoint: '/list', method: 'get', parameters, options },
  );
}

