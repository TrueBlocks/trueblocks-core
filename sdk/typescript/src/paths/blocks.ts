/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, Appearance, blknum, Block, BlockCount, Log, LogFilter, topic, Trace, TraceAction, TraceResult, uint64 } from '../types';

export function getBlocks(
  parameters?: {
    blocks: blknum[],
    hashes?: boolean,
    uncles?: boolean,
    traces?: boolean,
    uniq?: boolean,
    flow?: 'from' | 'to' | 'reward',
    logs?: boolean,
    emitter?: address[],
    topic?: topic[],
    articulate?: boolean,
    bigRange?: uint64,
    count?: boolean,
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
  return ApiCallers.fetch<Appearance[] | Block[] | BlockCount[] | Log[] | LogFilter[] | Trace[] | TraceAction[] | TraceResult[]>(
    { endpoint: '/blocks', method: 'get', parameters, options },
  );
}
