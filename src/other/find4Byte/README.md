# Running code

```
go run main.go 0xXXXXXXXX 0xYYYYYYYY ... 0xZZZZZZZZ
```
where 0xXXXXXXXX, 0xYYYYYYYY, 0xZZZZZZZZ - search requests (multiple at once)

I'd recommend to use 'go run main.go ... 2> /dev/null', i.e. redirect STDERR (or even comment 'fmt.Fprintf(os.Stderr...' line in code), because console output heavily slows down bruteforce.


# Documentation

After unzipping the data files found in this folder and using the same techniques as are used by the Solidity compiler to build 4-byte codes out of canonical function signatures, cross multiply a list of the 100,000 most commonly-used English words (provided in the file `function_names.csv`) with 680 argument lists (provided in the file `function_signatures.csv`) searching for the given 4-byte code by applying keccak256 to each resulting function signature.

The delivered code should be
  - written in 'Go',
  - should be able to parse a simple command line that includes one or more four byte codes (each starting with `0x`), and
  - deliver matches to the `standard output`, while delivering progress to `standard err`.

The search should be done in parallel and be as fast as possible. Use of Go channels is preferred.

Even if a given four-byte code is found, the program should continue searching as there may be multiple matches the canonical signature. We would like to find all matches.

C++ like pseudo code follows:

### Pseudo Code in C++ (the deliverable should be in 'Go')

    int main(int argc, char *argv[]) {

        if (argc != 2) return 0;
        search = argv[1];

        names = file containing 100,000 common English words

        sigs = file containing ~700 function signatures

        cout << "Scanning " << names.size() << " names ";
        cout << "and " << sigs.size() << " signatures (";
        cout << (names.size() * sigs.size()) << " total) for 4-byte code " << search << endl;

        for (every name : names) {
            for (every sig : sigs) {
                canonical = name + "(" + sig + ")";
                fourByte = '0x' + keccak256(canonical).substr(0, 8);
                if (fourByte == search) {
                    cout << "Found: " << fourByte << "\t" << canonical << endl;
                } else {
                    cerr << "Scanning << canonical << "\r";
                    cerr.flush();
                }
            }
        }
        return 1;
    }

