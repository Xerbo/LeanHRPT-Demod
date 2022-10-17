# LeanHRPT Generic Demod

A generic demodulator. For the original software see [LeanHRPT Demod](https://github.com/Xerbo/LeanHRPT-Demod) and [LeanHRPT Decode](https://github.com/Xerbo/LeanHRPT-Decode).

![Screenshot of LeanHRPT-Demod demodulating Cloudsat](images/example.webp)

## Compiling

To compile from source you will need `qtbase5-dev`, [`libcorrect`](https://github.com/quiet/libcorrect), `libsndfile1-dev`, `libsoapysdr-dev`, `libfftw3-dev` and a C++11 compiler (tested with gcc, clang and MSVC) installed. After that you can compile with:

```sh
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```
You can also enable some experimental features by adding `-DEXPERIMENTAL=ON` to cmake.
