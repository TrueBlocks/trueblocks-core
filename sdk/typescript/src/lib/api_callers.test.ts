import * as Callers from './api_callers';
import { expect, beforeEach, describe, it } from 'vitest';

const baseUrl = `${process.env.CORE_URL}`;
beforeEach(() => {
  Callers.setBaseUrl(baseUrl);
  //  Callers.setFetch(global.fetch);
});

describe('makeUrl', () => {
  it('returns correct URL when no params', () => {
    const expected = `${baseUrl}/hello`;
    const result = Callers.makeUrl('/hello');
    expect(result).toBe(expected);
  });
  it('returns correct URL when params', () => {
    const expected = `${baseUrl}/hello?a=1&b=string&c=true&d=false`;
    const result = Callers.makeUrl('/hello', {
      a: 1,
      b: 'string',
      c: true,
      d: false,
    });
    expect(result).toBe(expected);
  });
});
