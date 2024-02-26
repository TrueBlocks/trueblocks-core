/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, Log, LogFilter, topic, txId } from '../types';

export function getLogs(
  parameters?: {
    transactions: txId[],
    emitter?: address[],
    topic?: topic[],
    articulate?: boolean,
    fmt?: string,
    chain: string,
    noHeader?: boolean,
    cache?: boolean,
    decache?: boolean,
    raw?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Log[] | LogFilter[]>(
    { endpoint: '/logs', method: 'get', parameters, options },
  );
}

