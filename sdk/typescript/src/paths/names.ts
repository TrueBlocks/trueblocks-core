/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { Name } from '../types';

export function getNames(
  parameters?: {
    terms: string[],
    expand?: boolean,
    matchCase?: boolean,
    all?: boolean,
    custom?: boolean,
    prefund?: boolean,
    addr?: boolean,
    tags?: boolean,
    regular?: boolean,
    dryRun?: boolean,
    create?: boolean,
    update?: boolean,
    delete?: boolean,
    undelete?: boolean,
    remove?: boolean,
    named?: boolean,
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
  return ApiCallers.fetch<Name[]>(
    { endpoint: '/names', method: 'get', parameters, options },
  );
}
