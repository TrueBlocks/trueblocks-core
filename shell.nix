with import <nixpkgs> {};
mkShell {
    buildInputs = [
      go
      git
      cmake
      python3
      curl.dev
      clang-tools
      jq
    ];
}
