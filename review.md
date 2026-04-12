## Known Limitations

### `min`/`max` NaN asymmetry (review #18)

SSE `MINSS`/`MAXSS` have asymmetric NaN handling: `min(NaN, 5.0f)` returns `5.0f` but
`min(5.0f, NaN)` returns `NaN`. This is inherent to the x86 instruction set and accepted
as-is. Debug asserts on normalize, inverse, rotate, and unproject catch NaN sources upstream
before they reach min/max.

### `benchmarkMachineEpsilon` and `/fp:fast` (review #3)

The epsilon benchmark loop works correctly in practice with current MSVC but is theoretically
fragile under `/fp:fast`. Accepted as-is since MSVC does not currently optimize it away.
