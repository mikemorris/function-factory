# function-factory

```
npm install && node --expose-gc addon.js
```

The heap keeps growing with each iteration because of leaking `v8::FunctionTemplate` objects - is there a better pattern for a function factory where the returned functions still have a reference to the unique object from which they were created?

Creating a static `Nan::Persistent<v8::FunctionTemplate>` seems to eliminate the possibility of attaching a `void*` accessible via `info.Data()`. I've tried using `v8::Function::SetHiddenValue`, and `Nan::SetInternalFieldPointer` as well but can't seem to get a workable non-leaking solution.
