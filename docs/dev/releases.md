
# React Native WinRT Releases
As mentioned elsewhere in this repo, releases follow the major/minor version scheme of React Native/React Native Windows.
This is to make it very explicit which version(s) of React Native WinRT work with which version(s) of React Native (Windows).
This versioning scheme is reflected in both the Nuget package as well as the NPM package, however it's also reflected in the branching scheme.
The `main` branch in this repo is intended to serve as active development.
The tests/samples in this branch target the latest supported version of React Native (Windows) and no releases are published from this branch.
Instead, releases are published from separate `release/<version>` branches (e.g. `release/0.72` for version `0.72`).
Features/bug fixes submitted to `main` that are desired to be backported to any specific release(s) should be cherry-picked before the new release is published.

## Publishing on NPM
When publishing a new release on NPM, it is recommended that you follow these steps:

* Check out the release branch you wish to publish.
  * If you need to cherry-pick a change, check out a new branch & do that now.
* Run `git clean -fxd` to ensure no stale builds/files are being published.
* Run `scripts/build-npm-package.cmd` to ensure no errors, etc.
* [OPTIONAL] Ensure the tests run and pass. Ideally this will be validated by the CI and is therefore not essential to be done locally.
* [OPTIONAL] Ensure the sample app builds & runs. This is _not_ done as a part of the CI and it also tests the layout of the `package` directory and is therefore a good sanity check before publishing.
* Run `scripts/publish-npm-package.cmd` to upload to [npmjs.com](https://www.npmjs.com/package/react-native-winrt).

If the final step generates an error related to `packfiles.old` and `packfiles.new` not containing the same contents, inspect these two files to see what the differences are and if it is expected or not.
If these differences are expected, update `packfiles.old` & submit the change before retrying, cherry-picking this change to `main`/other release branches as necessary.
