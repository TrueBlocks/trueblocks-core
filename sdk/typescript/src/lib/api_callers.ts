export type ErrorResponse = {
  status: number,
  errors: string[],
};

export type SuccessResponse<DataType> = {
  data: DataType,
  meta: {
    unripe: number,
    ripe: number,
    staging: number,
    finalized: number,
    client: number
  },
};

export type AnyResponse<DataType> =
    | ErrorResponse
    | SuccessResponse<DataType>

let baseUrl = `${process.env.CORE_URL}`;
const fetchToUse = global.fetch;

export function setBaseUrl(url: string) {
  baseUrl = url;
}

export function getBaseUrl() {
  return baseUrl;
}

export function makeUrl(
  endpoint: string,
  parameters: Record<string, unknown> = {},
) {
  const search = new URLSearchParams(Object.entries(parameters)
    .map(([key, value]) => [key, String(value)]));
  const endpointUrl = new URL(endpoint, baseUrl);

  endpointUrl.search = search.toString();

  return endpointUrl.toString();
}

export function fetchBody({
  endpoint,
  method,
  parameters = {},
  options = {},
}: {
  endpoint: string,
  method: string,
  parameters?: Parameters<typeof makeUrl>[1],
  options?: RequestInit,
}) {
  const endpointUrl = makeUrl(endpoint, parameters);

  return fetchToUse(endpointUrl, {
    method,
    ...options,
  });
}

export function fetch<DataType>(args: Parameters<typeof fetchBody>[0]) {
  return fetchBody(args)
    .then((response) => response.json() as Promise<AnyResponse<DataType>>);
}
