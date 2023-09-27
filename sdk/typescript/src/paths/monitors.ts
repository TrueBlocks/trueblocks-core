/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, double, Monitor, MonitorClean, uint64 } from '../types';

export function getMonitors(
  parameters?: {
    addrs?: address[],
    delete?: boolean,
    undelete?: boolean,
    remove?: boolean,
    clean?: boolean,
    list?: boolean,
    watch?: boolean,
    watchlist?: string,
    commands?: string,
    batchSize?: uint64,
    sleep?: double,
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
  return ApiCallers.fetch<Monitor[] | MonitorClean[]>(
    { endpoint: '/monitors', method: 'get', parameters, options },
  );
}
