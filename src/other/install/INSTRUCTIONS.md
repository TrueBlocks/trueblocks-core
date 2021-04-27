#### Make a backup copy of `~/Library/Application Support/TrueBlocks/`, then

```bash
    rm -fR "~/Library/Application Support/TrueBlocks"
```

#### From your development folder clone the master branch:

```bash
    git clone https://github.com/TrueBlocks/trueblocks-explorer
```

#### In a new terminal window:

```bash
    cd trueblocks-explorer/api
    yarn install
    yarn start             # should start the API
```

#### In the original terminal window:

```bash
    cd trueblocks-explorer
    yarn install
    yarn start             # should start the explorer, will report a problem
```

#### In another new terminal window:<br><small><i>(assumes c++ and go build environments)</i></small>

```bash
    git clone --branch develop https://github.com/TrueBlocks/trueblocks-core
    cd trueblocks-core
    mkdir build && cd build
    cmake ../src
    make
```

#### The explorer application should now work.
