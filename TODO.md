
# TODO

NodeTetApp
* Reference SDK WinMDs the right way.  Can we use winapp?
* build.js nuget restore -- clean it up.  Can we use winapp?
* Better error messages.
* RTTI -- can we remove?

* Testing -- copies TestComponent.dll locally and node.exe locally so we can run and take advantage
of how they're already made?
* Testing -- can we just copy the previous tests?  Looks like they are nice.
  * Looks like YES, we may want to do es6 mode.
* Testing -- Building TestComponent.dll only as non-AppContainer for now, we probably need to support
the UWP way too.

* Test suite should be in test folder
* Share as much test collateral as possible
* Bothers me how we have multiple places where we hook up setters.  Should we remove WinRTWrappedObject.
* How do we do this in RN?  I should have the sample running so I can debug
* Need to map this out end-to-end.  what's the customer experience and what happens?

* object caching and object lifetimes.  I don't feel good about this.
Does it really work?
