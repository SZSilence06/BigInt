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

## Acknowledgement

This project uses these 3rd party libraries.

### BSL-1.0

<a href="https://github.com/philsquared/Catch">Catch</a>


