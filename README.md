
Cyclic Redundancy Check
=======================

This repository contains the code I wrote as part of the first lab assignment of the Computer Networks Course, Spring 2014, IITM. We are to generate a set of 128 bit string (refer ```generate.c```) which is stored in a file (```input.txt```). This file is read using ```apply_crc.c``` which tries the following for each bit string:
 
 - Generate 1 bit errors on 10 different trials
 - Generate consecutive 2 bit errors on 10 different trials
 - Generate burst errors of length < 30 on 10 different trials
 - Generate odd number of bit flips on 10 different trials
 
 and checks whether the corrupted bit string fails on CRC-check. Refer ```output.txt``` for the output of this check.
 
 
 Commands
 -
 
 First run,
 
  ```
  ./generate <input_file> <no_of_strings >
```

 to generate the bit strings. To check correctness of CRC, run:
 
   ```
  ./ec <input_file> <output_file>
```
 

 
