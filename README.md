quicklz implementation
======================

Because why not?

Compiling
---------
gcc -Wall  -O2  -std=c99     -c main.c -o main.o
gcc -Wall  -O2  -std=c99     -c quicklz.c -o quicklz.o
g++  -o ./ quicklz.o   -s

Quicklz
-------
http://www.quicklz.com/

Example usage
-------------

`quicklz -h`

outputs

<pre>
Quicklz implementation version 1:

usage: quicklz [options]:

   -f file - input file that will be compress or decompress. This can be stdin.
   -h      - show this help message.
   -o file - output file. This can be stdout.
   -d      - will decompress the file given via stdin or via -f option.

Examples:.

   cat file.qlz | quicklz -d > file
   cat file | quicklz > file.qlz
   quicklz -i file -o file.qlz
   quicklz -d -i file.qlz -o file
</pre>



