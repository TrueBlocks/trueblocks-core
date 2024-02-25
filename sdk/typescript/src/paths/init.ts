/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { blknum, ChunkRecord, double, Manifest } from '../types';

export function getInit(
  parameters?: {
    all?: boolean,
    dryRun?: boolean,
    firstBlock?: blknum,
    sleep?: double,
    chain: string,
    noHeader?: boolean,
    fmt?: string,
    verbose?: boolean,
    ether?: boolean,
    raw?: boolean,
    cache?: boolean,
    decache?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<ChunkRecord[] | Manifest[]>(
    { endpoint: '/init', method: 'get', parameters, options },
  );
}

