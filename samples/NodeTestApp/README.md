# NodeTestApp

Experimental sample exercising rnwinrt Node.js projection mode.

## Scripts

- `npm run generate:winrt` invokes `rnwinrt.exe -node` to generate placeholder projection files into `generated/`.
- `npm run build:addon` builds the native Node addon.
- `npm run build` runs generation then builds.
- `npm start` runs the sample.

## Status

Phase 1: scaffolding only. Generated WinRT bindings are not yet functional.

## Generated WinRT Projections

After running the generation script, the folder `generated/rnwinrt` will contain the projection sources.

In Node mode (`-node` flag), the usual React Native JSI `base.h` / `base.cpp` are intentionally replaced by placeholder
Node variants. These stubs will later host Node-specific helpers (N-API bridging, argument marshalling, object/cache
management). For now they only expose a no-op `Placeholder()` function so downstream build steps can link cleanly.
