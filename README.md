
<center><img src="./misc/pipeBB.png" alt="image" style="width:15.0%" /></center>

`pipeBB` is a library with (partially stateful) building blocks for processing
pipelines. It enables transfer from visually implemented directed acyclical
graphs (DAGs) with processing "blocks" and a directed data flow to be
implemented intuitively in C++.


## Current To Dos

- get this readme up to speed (!)


## Build and Test

Required to build and test the library are:

- `CMake` (3.8.0+)
- `make` (4.2.1+)
- `gcc` (7.0+)

Simply run

```bash
# create new directory "build" and switch into it
mkdir build && cd build

# execute "cmake" in the parent directory, the call make in this directory
cmake ..
make

# run test suite
make test
```

