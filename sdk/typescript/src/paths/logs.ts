/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { Log, LogFilter, txId } from '../types';

export function getLogs(
  parameters?: {
    transactions: txId[],
    articulate?: boolean,
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
  return ApiCallers.fetch<Log[] | LogFilter[]>(
    { endpoint: '/logs', method: 'get', parameters, options },
  );
}
