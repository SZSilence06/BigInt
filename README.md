# BigInt
Library for arbitrary length integer and floating number manipulation. Support multiple bases. 

Still under development. not completed yet.

## Usage

This is a header-only library. To use this package:
```C++
#include <BigInt.h>

using namespace szsilence06;
```

## Features

### Construct from integers or strings

```C++
BigInt a = -126;
a.toString();    //"-126"
BigInt b = "00002594";
b.toString();    //"2594"
BigInt c = "0123abcd";
c.isValid();    //"false"
BigInt16 hex = 256;
hex.toString();  //"100"
BigInt16 hex2 = "-0007cd";
hex2.toString();   //"-7CD"
```

### operands

```C++
BigInt a = 120;
BigInt b = 25;
a + b;   //145
a - b;   //95
a * b;   //3000
a / b;   //4
a % b;   //20
a > b;   //true
```

### Conversion to integers

```C++
BigInt a = "-00273";
a.toInt32();   //-273
```

### Conversion to other bases

```C++
BigInt a = 1288343654;

BigInt16 hex = a.toBase<16>();
hex.toString();  //"4CCA9066"

BigInt2 binary = a.toBase<2>();
binary.toString();  //"1001100110010101001000001100110"

BigInt8 oct = a.toBase<8>();
oct.toString(); //"11462510146"
```

## ToDo

- [x] 10-base BigInt
- [x] Other bases BigInt
- [x] Operands for BigInt
- [x] Conversion between bases for BigInt
- [ ] 10-base BigReal
- [ ] Other bases BigReal
- [ ] Operands for BigReal
- [ ] Conversion between bases for BigReal
- [ ] Conversion between BigInt and BigReal


## Acknowledgement

This project uses these 3rd party libraries.

### BSL-1.0

<a href="https://github.com/philsquared/Catch">Catch</a>


