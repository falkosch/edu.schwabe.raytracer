# SSE & AVX mathfun headers implementing basic trigonometric functions

The third-party code in [sse_mathfun.h](http://gruntthepeon.free.fr/ssemath/sse_mathfun.h)
and [avx_mathfun.h](http://software-lisc.fbk.eu/avx_mathfun/avx_mathfun.h) implements both SSE and AVX versions of basic
trigonometric functions.

Another source for math functions is [cephes at netlib.org](http://www.netlib.org/cephes/).

## Development guidelines

- The license/copyright header in `*_mathfun.h` files MUST be kept intact.
- In case of patches or fixes, a dedicated changelog comment `/* ... */` MUST be used to keep track of any custom
  changes to these files.
- The `*_mathfun.h` files MUST NOT use any code of the actual vectorization library. Using anything from the
  vectorization library would just create a circular dependency.
