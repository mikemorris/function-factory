#include <nan.h>
#include <iostream>

class Obj : public Nan::ObjectWrap {
public:
  static void Init(v8::Local<v8::Object>, v8::Local<v8::Object>);

  static void New(const Nan::FunctionCallbackInfo<v8::Value>&);
  static Nan::Persistent<v8::Function> constructor;

  static void MyFunction(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void CreateFunction(const Nan::FunctionCallbackInfo<v8::Value>& info);

private:
  int num = 0;
};

Nan::Persistent<v8::Function> Obj::constructor;

void Obj::MyFunction(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  info.GetReturnValue().Set(reinterpret_cast<Obj*>(info.Data().As<v8::External>()->Value())->num++);
}

void Obj::CreateFunction(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  // Enter a new v8::Context to prevent leaking v8::FunctionTemplate
  v8::Local<v8::Context> context = v8::Context::New(v8::Isolate::GetCurrent());
  v8::Context::Scope scope(context);

  Obj* obj = Nan::ObjectWrap::Unwrap<Obj>(info.This());

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(MyFunction, Nan::New<v8::External>(obj));
  v8::Local<v8::Function> fn = tpl->GetFunction();

  std::cout << obj->num << std::endl;

  info.GetReturnValue().Set(fn);
}

void Obj::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  auto obj = new Obj();
  obj->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

void Obj::Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);

  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "create", CreateFunction);

  constructor.Reset(tpl->GetFunction());

  Nan::Set(module, Nan::New("exports").ToLocalChecked(), tpl->GetFunction());
}

NODE_MODULE(addon, Obj::Init)
