-- Project Overview -- 

SOURCE CODE PATH: pa4/src

This project was developed for a Computer Architecture class. Given a description of
a circuit in a text file, returns the truth table for that circuit. This code and implementation is completely my own.


Supports the following directives: 


  INPUT n i1 ···in
    Declares n input variables. This directive must always occur first in a circuit description.
  OUTPUT n o1 ···on
    Declares n output variables. This directive must always occur second in a circuit description.
  NOT i o
    Represents a not gate in logic design. Computes o = i.
  AND i1 i2 o
    Represents an and gate in logic design. Computes o = i1i2.
  OR i1 i2 o
    Represents an or gate in logic design. Computes o = i1 + i2.
  NAND i1 i2 o
    Represents a nand gate in logic design. Computes o = i1i2
  NOR i1 i2 o
    Represents a nor gate in logic design. Computes o = i1 + i2
  XOR i1 i2 o
    Represents an xor gate in logic design. Computes o = i1 ⊕i2, where ⊕indicates exclusive or.
  DECODER n i1 ···ino0 ···o2n−1
    Represents an n : 2ndecoder gate in logic design. The first argument gives the number of
    inputs, n. The next n parameters are the inputs, followed by 2nparameters indicating the
    outputs.
    The inputs are interpreted as an n-bit binary number s in the range 0, ···, 2n−1, where i1
    is the most significant bit and inis the least significant bit. The output oswill be 1 and all
    others will be 0.

-- EXAMPLE -- 


Given:

INPUT 3 a b c
OUTPUT 1 d
AND a b   x
AND c b   d

Returns:

0 0 0 | 0
0 0 1 | 0
0 1 0 | 0
0 1 1 | 0
1 0 0 | 0
1 0 1 | 0
1 1 0 | 0
1 1 1 | 1


