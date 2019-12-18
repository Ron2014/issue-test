usages:

deault
g++ test2.cc -o test

use mutex
g++ test2.cc -o test -DUSE_MUTEX

use atomic
g++ test2.cc -o test -DUSE_ATOMIC

for linux
add -lpthread