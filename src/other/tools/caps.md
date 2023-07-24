## Global Options

|           |                                                                                                                              |
| --------- | ---------------------------------------------------------------------------------------------------------------------------- |
| All tools | --help, --chain, --fmt, --verbose, --no_header,<br>--version, --file, --output, --append, --log_level,<br>--no_color, --noop |

### Additional Global Options

| Tool         | --cache | --raw | --wei | --ether |     |
| ------------ | ------- | ----- | ----- | ------- | --- |
| export       | x       |       | x     | x       |     |
| blocks       | x       | x     |       |         |     |
| transactions | x       | x     | x     | x       |     |
| receipts     | x       | x     |       |         |     |
| logs         | x       | x     |       |         |     |
| traces       | x       | x     |       |         |     |
| when         | x       |       |       |         |     |
| state        | x       |       | x     | x       |     |
| tokens       | x       |       |       |         |     |
| slurp        | x       | x     |       |         |     |

### Global Options Disabled

| Tool    | --chain | --fmt | --verbose | --no_header | --file | --output | --append |
| ------- | ------- | ----- | --------- | ----------- | ------ | -------- | -------- |
| daemon  | x       | x     |           | x           | x      | x        | x        |
| scrape  |         | x     | x         | x           | x      | x        | x        |
| init    |         | x     | x         | x           | x      | x        | x        |
| config  |         | x     | x         | x           | x      | x        | x        |
| explore |         | x     | x         | x           |        | x        | x        |
| status  |         | x     |           | x           | x      | x        | x        |
| chunks  |         |       |           |             | x      | x        | x        |
