# Deprecated TypeScript Annotations in react-native-winrt

## Overview

The `rnwinrt` tool generates `/** @deprecated message */` JSDoc annotations for WinRT types
and members that have the `Windows.Foundation.Metadata.DeprecatedAttribute` in their metadata.

## Usage

To include deprecated types in the generated output, use the `-deprecatedincluded` flag:

```bash
rnwinrt.exe -input local -include Windows.Media.PlayTo -tsoutput ./output -deprecatedincluded
```

Without `-deprecatedincluded`, deprecated types are excluded entirely from the output.

## Generated Output Example

```typescript
/** @deprecated PlayToConnection may be altered or unavailable for releases after Windows 10. Instead, use CastingConnection. */
class PlayToConnection {
    public readonly state: Windows.Media.PlayTo.PlayToConnectionState;
    /** @deprecated PlayToConnection may be altered or unavailable ... */
    public addEventListener(type: "statechanged", listener: any): void;
}

/** @deprecated PlayToConnectionState may be altered or unavailable for releases after Windows 10. Instead, use CastingConnectionState. */
enum PlayToConnectionState {
    /** @deprecated ... */
    disconnected,
    /** @deprecated ... */
    connected,
    /** @deprecated ... */
    rendering,
}
```

## What Gets Annotated

- **Classes/Interfaces**: The class declaration gets `@deprecated`
- **Enums**: Both the enum type and individual enum values get `@deprecated`
- **Methods**: Instance and static methods get `@deprecated`
- **Properties**: Read-only and read-write properties get `@deprecated`
- **Events**: addEventListener/removeEventListener pairs get `@deprecated`
- **Delegates**: Delegate type aliases get `@deprecated`
- **Structs**: Interface declarations (for value types) get `@deprecated`

## IDE Support

TypeScript-aware editors (VS Code, WebStorm, etc.) will show:
- Strikethrough on deprecated symbols
- Warning messages in hover tooltips
- Diagnostics when deprecated APIs are used

## Verification

Run the verification script to confirm deprecated annotations are generated correctly:

```powershell
.\tests\TestArtifacts\verify_deprecated.ps1 -rnwinrtPath .\rnwinrt\x64\Release\rnwinrt.exe
```
