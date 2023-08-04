/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { ChunkRecord, double, Manifest } from '../types';

export function getInit(
  parameters?: {
    all?: boolean,
    dryRun?: boolean,
    sleep?: double,
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
  return ApiCallers.fetch<ChunkRecord[] | Manifest[]>(
    { endpoint: '/init', method: 'get', parameters, options },
  );
}
