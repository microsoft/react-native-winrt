# Node Support Plan

This project (react-native-winrt or rnwinrt) supports React Native, but it will also now support Node JS.  Node JS
is also a runtime environment, but we'll need to use different APIs (node-add-on APIs) to interop with the Javascript.

To build rnwinrt:

```
cd rnwinrt
msbuild /p:Platform=x64
```

# Phase 1 -- Set up the project

TODO list:

- [x] Add a "-node" flag to rnwinrt.exe that puts it into "node" mode.  (we will keep it named "rnwinrt.exe" for now).
- [x] Next to the "rnwinrt\rnwinrt\react" dir we should have a "rnwinrt\rnwinrt\node" dir, that will have node-specific template files.
Let's start with empty node_base.h and node_base.cpp files for now.
- [x] In "samples", we want a NodeTestApp, that is a simple nodejs app.
- [x] NodeTestApp uses node-gyp to build a .node file.
- [x] NodeTestApp runs rnwinrt.exe (with the -node flag of course) to build projections for the TestComponent in this repo.

It won't actually be _functional_ at this point of course, but the main pieces are in place, and the test project should successfully
generate the files, but the build won't pass yet.

# Phase 2 -- Make it build
coming soon...