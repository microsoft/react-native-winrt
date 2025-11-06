---
applyTo: '**'
---

# How to build and test

Start in the NodeTestApp dir.

```
npm run build
```

If you need a clean build:

```
npm run rebuild
```

Run tests:

```
npm test
```

Run specific test:
```
.\node test.js -t "test name"
```


# What are we trying to do?

We're working on NodeJS WinRT projections.  We're building on an existing project for react-native called react-native-winrt.

This is a test project that allows us to iterate quickly.  When it's working well, we'll copy the code into templates.

