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
    clean?: boolean,
    regular?: boolean,
    dryRun?: boolean,
    autoname?: string,
    create?: boolean,
    update?: boolean,
    delete?: boolean,
    undelete?: boolean,
    remove?: boolean,
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
  return ApiCallers.fetch<Name[]>(
    { endpoint: '/names', method: 'get', parameters, options },
  );
}
