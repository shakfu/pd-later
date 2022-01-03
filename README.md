# pd-later

A place for deferred and suspended pure-data external projects.


## Deferred Projects

- [pd-dbm](pd-dbm]: the intent was to wrap a dbm type database for puredata.
  Found Tokyo cabinet too heavy and then tried QDBM which looked promising.
  Suspended due to seemingly unfixable error at opening of database (perhaps due  to threading issues).

- [pd-zforth](pd-zforth): a wrapper around zforth language. Suspended due to
  apparent difficulties of integration of necessary key functions, and possible
  need to rewrite zforth implementation functions to include a first arg which
  is consistent with the external's signature.

