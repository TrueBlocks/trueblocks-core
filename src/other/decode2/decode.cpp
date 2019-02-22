#include <iostream>
#include <vector>
#include <sstream>
//#include <gmp.h>
#include "etherlib.h"
#include "tests.h"

typedef bigint_t mpz_t;
#define mpz_init(i)
#define mpz_set_ui(b,v) b=v
#define mpz_pow_ui(v,b,p) v=modexp(b,p,uint64_t(10000000000))
#define mpz_cdiv_q_ui(v,n,d) v=n/d
#define mpz_cmp(a,b) ((a==b)?0:(a<b)?-1:1)
#define mpz_set(t,v) t=v
#define mpz_sub(v,f,s) v=f-s
#define mpz_neg(v,v1) v=-v1
#define mpz_set_str(r,s,n)

using namespace std;

extern void padTest(void);
extern void hexUtilTest(void);

bigint_t str_2_BigInt2(const string_q& hexIn) { //}, size_t maxSize) {
    bigint_t ret = str_2_BigInt(hexIn);
    if (true) { //maxSize == 256) {
        static const bigint_t max256Int = str_2_BigInt("115792089237316195423570985008687907853269984665640564039457584007913129639935");
        if (ret >= (max256Int / 2))
            ret = ret - max256Int - 1; // wrap if larger than max int256
    }
    return ret;
}

/*
 mpz_t uint;
 cout << uint << " " << hex << endl;
 mpz_init(uint);
 cout << uint << " " << hex << endl;
 uint = str_2_BigUint(hex);
 cout << uint << " " << hex << endl;

 mpz_t max;
 cout << max << endl;
 mpz_init(max);
 cout << max << endl;

 mpz_t base;
 cout << base << endl;
 mpz_init(base);
 cout << base << endl;
 mpz_set_ui(base, 2);
 cout << base << endl;

 mpz_pow_ui(max, base, bitSize);
 cout << max << " " << base << " " << bitSize << endl;

 mpz_t center;
 cout << center << endl;
 mpz_init(center);
 cout << center << endl;
 mpz_cdiv_q_ui(center, max, 2);
 cout << center << endl;

 int res = mpz_cmp(uint, center);
 cout << uint << " " << center << " " << res << endl;
 if(res < 0 || res == 0){
 mpz_set(integer, uint);
 } else {
 mpz_sub(integer, max, uint);
 mpz_neg(integer, integer);
 }
 */

string decode(string rawFunction, string abi);
string decodeParams(vector<string> parsedParams, vector<string> parsedABI, int &ABIPointer);

bool Hex32ToBool(string hex);
int Hex32ToInteger(string hex);
string Hex32ToBytes(string hex);

string toCleanFunctionSig(string functionStr);
string parseFunctionName(string str);
vector<string> parseParameterTypes(string str);
vector<string> parseABI(string abi);

std::vector<std::string> split(std::string str, char delimiter);
std::string trim(std::string const& str);

void decodeTest();

void Hex32ToIntegerTest(string hexInput, int expectedVal);


////////////////////////////////////////////////////////////////////

// Test var constants (break out soon)

const string successCode = "1 TEST PASSED: SUCCESS!!!";
const string failureCode = "0 FAILED!";

///////////////////////////////////////////////////////////////////

/*
 * ABIDecoder
 *
 */


string decode(string rawFunction, string abi){
    vector<string> decodedParams = parseParameterTypes(rawFunction);

    //Mini-test to tell if we're decoding our params correctly
    for(string s : parseABI(abi)){
        cout << s << "|||\n";
    }

    int ABIPointer = 0;
    string total = decodeParams(decodedParams, parseABI(abi), ABIPointer);

    return total;
}

/*
 * decodeParams(vector<string> parsedParams, vector<string> parsedABI, int ABIPointer)
 *
 * The idea of this function is to loop through each parameter we expect from the ones given,
 * and thus determine their type. For example, "int8" can tell us to convert the corresponding first ABI value as
 * a value, and "string" can tell us to look at the pointer, switch to the specified 32-byte value in the ABI
 * (which, is the bytelength of the string, as it is the 1st value for strings)
 *
 * ABIPointer should ALWAYS represent the beginning of the initial value/pointer list which corresponds with the parsed parameters.
 *
 * parsedABI is simply an array with the 32-byte hex values split.
 *
 * The function is recursive in order to allow for entering "new scopes" (i.e. multi-dimensional arrays) easily,
 * as looping through this can quickly become complex, and recursion is great for algorithms which face an
 * unknown depth search
 *
 * When recursion occurs, it is assumed that the parsedABI remains the same string[], whereas the ABIPointer should
 * point to the new set of initial ABI value/pointer list. Therefore, parsedParams and ABIPointer must remain IN SYNC.
 *
 * */

string_q Hex32ToString(const string_q& hexIn, size_t byteLength) {
    string ret;

    string hexParsed = hexIn.substr(0, byteLength * 2);
    for(size_t i = 0; i < hexParsed.length(); i += 2){
        string byte = hexParsed.substr(i,2);
        char c = (char)(int)strtol(byte.c_str(), NULL, 16);
        ret.push_back(c);
    }

    return ret;
};

string decodeParams(vector<string> parsedParams, vector<string> parsedABI, int &ABIPointer){

    string total = "";

    //Iterate through array of parameter types
    //ABIPointer assumed to point to the very first element in the "parameter scope"
    for(int p = 0; p < parsedParams.size(); p++){
        string param = parsedParams[p];

        if(param.find("uint") != -1 && param.find("[") == -1){
            //Convert integer at ABIPointer, i.e. the parameter 32-byte (which is a value)
            biguint_t integer;
            //mpz_init(integer);
            integer = str_2_BigUint(parsedABI[ABIPointer]);

            //add to total
            total += bnu_2_Str(integer);

            //move forward
            ABIPointer++;

        } else if(param.find("int") != -1 && param.find("[") == -1){

            //By default, let's assume it is a 256-bit integer
            //int bitSize = 256;

            //Convert integer at ABIPointer
            mpz_t integer;
            mpz_init(integer);
            integer = str_2_BigInt2(parsedABI[ABIPointer]); //, bitSize);

            //add to total
            total += bni_2_Str(integer);

            //move forward
            ABIPointer++;

        } else if(param.find("string") != -1 && param.find("[") == -1){

            //Find offset
            int stringOffset = Hex32ToInteger(parsedABI[ABIPointer]);
            //Temporary pointer we can use with the parsedABI array - allows us to see where the actual value starts
            ABIPointer = stringOffset / 32;

            //Get length at 1st 32-byte element pointer points to  TODO: Consider using str_2_BigInt2 instead
            int byteLength = Hex32ToInteger(parsedABI[ABIPointer]);
            //Using the byteLength, we can now determine how to parse the string on the 2nd
            string strparam = Hex32ToString(parsedABI[ABIPointer+1], byteLength);

            //Concatenate strings
            total += strparam;
            //Move forward 1, onto the next set of parameter values/pointers
            ABIPointer++;

        } else if(param.find("bytes") != -1 && param.find("[") == -1){
            //Convert bytes at ABIPointer
            string bytes = Hex32ToBytes(parsedABI[ABIPointer]);

            //add to total
            total += bytes;

            //move forward
            ABIPointer++;

        } else if(param.find("[") != -1 && param.find("]") == param.find("[") + 1) {
            /*
             * Since types with multiple values (e.g. dynamic values/strings/arrs)
             * will ONLY have an offset if there is more than one of them, in the params,
             * we need to check if there it is the only one or not; if it is not,              * skip over and set tempPointer to the regular ABIPointer's value,
             * as we skip right to the "actual values"
             *
             */

            int tempPointer;
            /*
             * This temporary pointer is intended to work as a pointer which STARTS initialized at the "real values"
             * of the array. Since we will need to pass a pointer which will point to a "new scope" (set of parameter
             * "value/pointer" 32-byte hex values), we use this as a value we can advance.
             *
             */
            if(parsedParams.size() != 1) {
                //Find offset pointing to "real values" of dynamic array
                int arrOffset = Hex32ToInteger(parsedABI[ABIPointer]);
                tempPointer = arrOffset / 32;
            } else {
                tempPointer = ABIPointer;
            }

            //Obtain number of elements from the first set of "real array" values
            int elementNum = Hex32ToInteger(parsedABI[tempPointer]);

            //Advance the tempPointer; this will thus point at the beginning of the "value/pointer" 32-byte hex values
            tempPointer++;

            /*
             * Generate a list of types the array has in scope; do we have ints, or even int[]s? Important to know, as we
             * want to loop this
             * This should thus generate an array corresponding with the LENGTH of the array (as we expect the same
             * number of parameters as the LENGTH) and the TYPES expected (thus will be duplicates~~~)
             */
            vector<string> arrParams;

            int firstBracePos = param.find('[');

            //Parse out everything surrounding the FIRST "[]", which should give us the next "sub"type
            string paramType = param.substr(0, firstBracePos);
            if(firstBracePos + 2 != param.size()) {
                paramType += param.substr(firstBracePos + 2);
            }

            //Loop for array parameters
            for(int i = 0; i < elementNum; i++) {
                arrParams.push_back(paramType);
            }

            /*
             * Pass the array's types as arrParams, regular parsedABI, and tempPointer POINTING to the set of values
             * corresponding to array's types as arrParams
             *
             * Since this is recursive, we expect it to simply "create its own scope" to parse through with a new total,
             * which will get passed back to us, which we surround in array braces
             *
             * */
            string arrparam = "[" + decodeParams(arrParams, parsedABI, tempPointer) + "]";

            //Add to the total
            total += arrparam;

            //Advance it forward to the next parameter "value/pointer" 32-byte hex value
            ABIPointer++;

        } else if(param.find("[") != -1 && param.find("]") != -1){
            /*
             * Temporary pointer we can use with the parsedABI array
             */
            //int tempPointer;
            /*
             * This temporary pointer is intended to work as a pointer which STARTS initialized at the "real values"
             * of the array. Since we will need to pass a pointer which will point to a "new scope" (set of parameter
             * "value/pointer" 32-byte hex values), we use this as a value we can advance.
             *
             */
            /*          if(parsedParams.size() != 1) {
             //Find offset pointing to "real values" of dynamic array
             int arrOffset = Hex32ToInteger(parsedABI[ABIPointer]);
             tempPointer = arrOffset / 32;
             } else {
             tempPointer = ABIPointer;
             }

             */

            //Number of elements
            /*
             * Since we have the number of elements within the passed type, lets grab the "first batch", i.e. everything before the first []
             * e.g. int[][3] --> int[]
             *
             * Therefore, int[][3] will assume that it is a int[3] which contains 3 amount of int[]s
             *
             * This does not quite correspond to how multi-dimensional arrays are outlined (should be the int[] which contains x amount of int[3]s),
             * which is likely where our current bug lies
             *
             */
            int elementNum = stoi(param.substr(param.find('[')+1, param.find(']')));

            /*
             * Generate a list of types the array has in scope; do we have ints, or even int[]s? Important to know, as we
             * want to loop this
             * This should thus generate an array corresponding with the LENGTH of the array (as we expect the same
             * number of parameters as the LENGTH) and the TYPES expected (thus will be duplicates~~~)
             *
             * Similarly as above, we assume everything before the LAST [ is the "next set" of containing types
             *
             */
            vector<string> arrParams;

            int firstLBracePos = param.find('[');
            int firstRBracePos = param.find(']');

            //Parse out everything surrounding the FIRST "[]", which should give us the next "sub"type
            string paramType = param.substr(0, firstLBracePos);
            if(firstRBracePos + 1 != param.size()) {
                paramType += param.substr(firstRBracePos + 1);
            }

            //Loop for array parameters
            for(int i = 0; i < elementNum; i++) {
                arrParams.push_back(paramType);
            }

            /*
             * Pass the array's types as arrParams, regular parsedABI, and tempPointer POINTING to the set of values
             * corresponding to array's types as arrParams
             *
             * Since this is recursive, we expect it to simply "create its own scope" to parse through with a new total,
             * which will get passed back to us, which we surround in array braces
             *
             * Following from above, we make the assumption that tempPointer points to the int[] piece of the int[][3],
             * which is likely NOT quite the case
             *
             * */
            string arrparam = "[" + decodeParams(arrParams, parsedABI, ABIPointer) + "]";

            //Add to the total
            total += arrparam;

            //Advance it forward to the next parameter "value/pointer" 32-byte hex value
            //ABIPointer++;


        }

        if(parsedParams.size() - 1 != p){
            total += ", ";
        }

    }

    //Close string
    return total;

}





/*
 * ABIUtil
 *
 */


string toCleanFunctionSig(string functionStr){

    string cleanSig;
    string functionName = parseFunctionName(functionStr);
    vector<string> parameterTypes = parseParameterTypes(functionStr);
    //Build the first piece of the clean signature
    cleanSig = functionName + "(" + parameterTypes[0];
    //If we have more than 1 parameter, lets cycle through them and add commas...
    if(parameterTypes.size() >= 2){
        for(int i = 1; i < parameterTypes.size(); i++){
            cleanSig += "," + parameterTypes[i];
        }
    }
    cleanSig += ")";

    return cleanSig;

}

string parseFunctionName(string str){

    string functionName;

    int indexEnd = str.find("(");
    int indexStart = 0;
    //If our function name has the function keyword specified...
    if(str.find("function") != string::npos){
        indexStart = str.find("function") + 8;
    }
    functionName = str.substr(indexStart, (indexEnd - indexStart));

    return functionName;

}

vector<string> parseParameterTypes(string str){

    vector<string> parameterTypes;

    int indexStart = str.find("(") + 1;
    int indexEnd = str.find(")");
    parameterTypes = split(str.substr(indexStart, (indexEnd - indexStart)), ',');

    for(int i = 0; i < parameterTypes.size(); i++){
        //Trim the extra whitespace
        string paramType = trim(parameterTypes[i],' ');
        //Remove all but the type, if we have a var explicitly named
        if(paramType.find(" ") != -1){
            paramType = paramType.substr(0, paramType.find(" "));
        }
        parameterTypes[i] = paramType;

    }
    return parameterTypes;

}

vector<string> parseABI(string abi){

    abi = trim(abi,' ');
    if(abi.find("0x") == 0) { abi = abi.substr(2, abi.size() - 2); }

    int num = abi.size() / 64;
    vector<string> parsedABI;

    for(int i = 0; i < num; i++){
        string abiByte32 = abi.substr(0, 64);
        parsedABI.push_back(abiByte32);

        abi = abi.substr(64);
    }
    return parsedABI;

}


/*
 * ABIHexUtil
 */

bool Hex32ToBool(string hex){
    int boolVal = Hex32ToInteger(hex);
    return boolVal == 1 ? true : false;
}

int Hex32ToInteger(string hex){
    return std::stoul(hex, nullptr, 16);
}

string Hex32ToBytes(string hex){
    return hex.find("0x") == 0 ? hex : "0x" + hex;
}


/* BIGINT UTIL */





/* BYTE PADDING FUNCTIONS */



/*
 * ABIDecoder
 */

/*
 *  General utility functions
 *
 *
 *
 */


std::vector<std::string> split(std::string str, char delimiter)
{
    std::istringstream ss(str);
    std::string token;

    std::vector<std::string> vectorOut;
    while(std::getline(ss, token, delimiter)) {
        vectorOut.push_back(token);
        //std::cout << token << '\n';
    }
    return vectorOut;
}


std::string trim(std::string const& str)
{
    if(str.empty())
        return str;

    std::size_t firstScan = str.find_first_not_of(' ');
    std::size_t first     = firstScan == std::string::npos ? str.length() : firstScan;
    std::size_t last      = str.find_last_not_of(' ');
    return str.substr(first, last-first+1);
}



/** BAD TESTS **/

void tempTest(){

    vector<string> testabi1 = parseABI("0x0000000000000000000000000000000000000000000000000000000000000040cb93e7ddea88eb37f5419784b399cf13f7df44079d05905006044dd14bb898110000000000000000000000000000000000000000000000000000000000000003000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c0007cc5734453f8d7bbacd4b3a8e753250dc4a432aaa5be5b048c59e0b5ac5fc00120aa407bdbff1d93ea98dafc5f1da56b589b427167ec414bccbe0cfdfd573");

    vector<string> testabi2 = parseABI("0x0000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000000b68656c6c6f20776f726c64000000000000000000000000000000000000000000");

    vector<string> testparam1 = parseParameterTypes("function baz(bytes[] a, bytes32 b)");
    vector<string> testparam2 = parseParameterTypes("function baz(uint128[2][3][2], uint)");

    for(int i = 0; i < testabi1.size(); i++){
        cout << "|" << testabi1[i] << "|" << endl;

    }

    for(int i = 0; i < testparam1.size(); i++){
        cout << "|" << testparam1[i] << "|" << endl;
    }

    for(int i = 0; i < testabi2.size(); i++){
        cout << "|" << testabi2[i] << "|" << endl;

    }

    for(int i = 0; i < testparam2.size(); i++){
        cout << "|" << testparam2[i] << "|" << endl;
    }
}


void Hex32ToIntegerTest(string hexInput, int expectedVal){

    cout << "=============================================================" << endl;
    cout << "Testing Hex32 to Integer" << endl;
    cout << "Hex32 Input: " << hexInput << endl;
    cout << "EXPECTING: " << expectedVal << endl;

    int res = Hex32ToInteger(hexInput);
    cout << "\n" << res << "\n" << endl;
    string testRes;
    res == expectedVal ? testRes = successCode : testRes = failureCode;
    cout << "\n     " << testRes << endl;
    cout << "=============================================================\n\n" << endl;

}


/////////////////////////////////////////////



void decodeTest(){
    vector<vector<string>> testCases;
    vector<string> test1 = {"function baz(int8)", "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe", "-2"};
    vector<string> test2 = {"function baz(int80)", "0x0000000000000000000000000000000000000000000000000000b29c26f344fe", "196383738119422"};
    vector<string> test3 = {"function baz(uint32)", "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe", "4294967294"};
    vector<string> test4 = {"function baz(string)", "0x0000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000000b68656c6c6f20776f726c64000000000000000000000000000000000000000000", "hello world"};
    vector<string> test5 = {"function baz(bytes[] a, bytes32 b)", "0x0000000000000000000000000000000000000000000000000000000000000040cb93e7ddea88eb37f5419784b399cf13f7df44079d05905006044dd14bb898110000000000000000000000000000000000000000000000000000000000000003000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c0007cc5734453f8d7bbacd4b3a8e753250dc4a432aaa5be5b048c59e0b5ac5fc00120aa407bdbff1d93ea98dafc5f1da56b589b427167ec414bccbe0cfdfd573", "[0x000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c, 0x0007cc5734453f8d7bbacd4b3a8e753250dc4a432aaa5be5b048c59e0b5ac5fc, 0x00120aa407bdbff1d93ea98dafc5f1da56b589b427167ec414bccbe0cfdfd573], 0xcb93e7ddea88eb37f5419784b399cf13f7df44079d05905006044dd14bb89811"};
    vector<string> test6 = {"function baz(int[3])", "0x000000000000000000000000000000000000000000000000000000000000002afffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffdfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffb", "[42, -3, -5]"};
    vector<string> test7 = {"function baz(uint128[2][3], uint)", "0x000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000050000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000a", "[[1, 2, 3], [4, 5, 6]], 10"};
    vector<string> test8 = {"function baz(uint128[2][3][2], uint)", "0x000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000050000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000050000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000a", "[[[1, 2], [3, 4], [5, 6]], [[1, 2], [3, 4], [5, 6]]], 10"};
    vector<string> test9 = {"function baz(uint[3][], uint)", "0x0000000000000000000000000000000000000000000000000000000000000040000000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000050000000000000000000000000000000000000000000000000000000000000006", "[[1, 2], [3, 4], [5, 6]], 10"};
    vector<string> test10 = {"function baz(uint[][3],uint)", "0x000000000000000000000000000000000000000000000000000000000000008000000000000000000000000000000000000000000000000000000000000000e00000000000000000000000000000000000000000000000000000000000000140000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000030000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000050000000000000000000000000000000000000000000000000000000000000006", "[[1, 2], [3, 4], [5, 6]], 10"};
    vector<string> test11 = {"function baz(uint256[] a,uint[] b,uint256[] c)", "0x000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000c0000000000000000000000000000000000000000000000000000000000000012000000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000500000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000015af1d78b58c400000000000000000000000000000000000000000000000000015af1d78b58c4000000000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000001bc16d674ec800000000000000000000000000000000000000000000000000001bc16d674ec80000", "[6, 5], [25000000000000000000, 25000000000000000000], [2000000000000000000, 2000000000000000000]"};
    testCases.push_back(test1);
    testCases.push_back(test2);
    testCases.push_back(test3);
    testCases.push_back(test4);
    testCases.push_back(test5);
    testCases.push_back(test6);
    testCases.push_back(test7);
    testCases.push_back(test8);
    testCases.push_back(test9);
    testCases.push_back(test10);
    testCases.push_back(test11);

    for(vector<string> test : testCases){
        cout << "=============================================================" << endl;
        cout << "FUNCTION INPUT: " << test[0] << endl;
        cout << "ABI: " << test[1] << endl;
        cout << "EXPECTING: " << test[2] << endl;

        string res = decode(test[0], test[1]);
        //if(res == nullptr) { res = "null"; } else if(res == "") { res = "empty value"; }
        if(res == "") { res = "empty value"; }
        cout << "\n" << res << "\n" << endl;
        string testRes;
        res == test[2] ? testRes = successCode : testRes = failureCode;
        cout << "\n     " << testRes << endl;
        cout << "=============================================================\n\n" << endl;
    }
}

//-----------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    padTest();
    hexUtilTest();
//    decodeTest();
    return 0;
}
