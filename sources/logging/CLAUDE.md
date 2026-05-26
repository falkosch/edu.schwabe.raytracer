# Logging Library

Thread-safe logging with a dedicated worker thread. Static library with no dependencies beyond the C++ standard library.

## Types

- `LogLevel` -- enum: Error, Warn, Info, Debug
- `LogEntry` -- level + scope string + lazy message factory (`std::function<std::string()>`) + timestamp
- `LogScope` -- scoped logger; creates LogEntries with a component name. Factory: `LogScope scope("name")`
- `Logger` -- singleton; owns a `std::jthread` worker that drains the entry queue
- `DisplayBuffer` -- thread-safe circular buffer (max 10,000 lines, evicts oldest 25% when full)
- `DisplayLine` -- formatted line (level + text) for UI consumption

## Usage

```cpp
auto log = LogScope::scope("MyComponent");
log.info([] { return "message"; });
log.error([] { return "details: " + std::to_string(code); });
```

## Routing

| Level | stdout | stderr | file | DisplayBuffer |
|-------|--------|--------|------|---------------|
| Error | -      | yes    | yes  | yes           |
| Warn  | -      | yes    | yes  | yes           |
| Info  | yes    | -      | yes  | yes           |
| Debug | yes    | -      | yes  | -             |

Output format: `[elapsed_seconds] - LEVEL scope : message`
