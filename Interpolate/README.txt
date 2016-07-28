A template function that tries to combine propertis of C++'s iostream and C's printf.
Ostream object is used beneath to make Interpolate work.

Notes:

Compiler options I used for compiling Interpolate:
clang++ -std=c++11 -Wfatal-errors -Wall -Wextra -pedantic -ftemplate-depth=1024 ...filename...
g++ -std=c++11 -Wfatal-errors -Wall -Wextra -pedantic -ftemplate-depth=1024 ...filename...


