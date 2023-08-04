/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, double, Monitor, MonitorClean } from '../types';

export function getMonitors(
  parameters?: {
    addrs?: address[],
    clean?: boolean,
    delete?: boolean,
    undelete?: boolean,
    remove?: boolean,
    list?: boolean,
    watch?: boolean,
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
  return ApiCallers.fetch<Monitor[] | MonitorClean[]>(
    { endpoint: '/monitors', method: 'get', parameters, options },
  );
}
