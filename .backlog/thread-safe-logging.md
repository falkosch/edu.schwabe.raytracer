# Thread-safe logging

## Motivation

The raytracer worker thread and the UI thread both write to `std::cout`. While individual `operator<<` calls are
thread-safe in C++, interleaved output between lines from different threads can produce garbled console output.

## Proposed improvement

Replace direct `std::cout` usage with a thread-safe logger that serializes full log lines (e.g., a lock-guarded
`println`-style function, or a lock-free message queue drained by the UI thread).
