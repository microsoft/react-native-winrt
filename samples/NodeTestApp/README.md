# NodeTestApp

Experimental sample exercising rnwinrt Node.js projection mode.

## Scripts

- `npm run generate:winrt` invokes `rnwinrt.exe -node` to generate placeholder projection files into `generated/`.
- `npm run build:addon` builds the native Node addon.
- `npm run build` runs generation then builds.
- `npm start` runs the sample.
- `npm test` copies test DLL and runs the comprehensive test suite.
- `npm run test:basic` runs tests without copying DLL (assumes it's already present).

## Test Suite

This project includes a comprehensive test suite adapted from the React Native WinRT tests. The test suite validates that the Node.js addon correctly projects WinRT APIs.

**Current Status**: 78 tests (4 enum tests + 74 basic function tests)

To run tests:
```bash
npm run test:basic
```

For more information, see:
- [Test Suite README](test/README.md) - Guide to running and porting tests
- [Test Migration Summary](TEST_MIGRATION.md) - Overview of what was ported
- [Porting Guide](PORTING_GUIDE.md) - Quick reference for porting more tests
- [Architecture](ARCHITECTURE.md) - Visual diagrams of test architecture

## Status

Phase 1: scaffolding only. Generated WinRT bindings are not yet functional.

## Generated WinRT Projections

After running the generation script, the folder `generated/rnwinrt` will contain the projection sources.

In Node mode (`-node` flag), the usual React Native JSI `base.h` / `base.cpp` are intentionally replaced by placeholder
Node variants. These stubs will later host Node-specific helpers (N-API bridging, argument marshalling, object/cache
management). For now they only expose a no-op `Placeholder()` function so downstream build steps can link cleanly.
