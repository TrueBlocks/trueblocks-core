/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { Block, NamedBlock, Timestamp, TimestampCount } from '../types';

export function getWhen(
  parameters?: {
    blocks?: string[],
    list?: boolean,
    timestamps?: boolean,
    count?: boolean,
    repair?: boolean,
    check?: boolean,
    update?: boolean,
    deep?: boolean,
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
  return ApiCallers.fetch<Block[] | NamedBlock[] | Timestamp[] | TimestampCount[]>(
    { endpoint: '/when', method: 'get', parameters, options },
  );
}
