// Copyright (c) 2012, Artur Adib
// All rights reserved.
//
// Author(s): Artur Adib <aadib@mozilla.com>
//
// You may use this file under the terms of the New BSD license as follows:
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Artur Adib nor the
//       names of contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL ARTUR ADIB BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "qpointf.h"

using namespace v8;

Persistent<Function> QPointFWrap::constructor;

// Supported implementations:
//   QPointF (qreal x, qreal y)
QPointFWrap::QPointFWrap(const FunctionCallbackInfo<v8::Value>& args) : q_(NULL) {
  if (args[0]->IsNumber() && args[1]->IsNumber()) {
    q_ = new QPointF(args[0]->NumberValue(), args[1]->NumberValue());
  } else {
    q_ = new QPointF;
  }
}

QPointFWrap::~QPointFWrap() {
  delete q_;
}

void QPointFWrap::Initialize(Local<Object> target) {
  Isolate *isolate = target->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "QPointF"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "x"),
      FunctionTemplate::New(isolate, X)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "y"),
      FunctionTemplate::New(isolate, Y)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "isNull"),
      FunctionTemplate::New(isolate, IsNull)->GetFunction());

  constructor.Reset(isolate, tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "QPointF"), tpl->GetFunction());
}

void QPointFWrap::New(const FunctionCallbackInfo<v8::Value>& args) {
  QPointFWrap* w = new QPointFWrap(args);
  w->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

Local<Value> QPointFWrap::NewInstance(Isolate *isolate, QPointF q) {
  Local<Function> cons = Local<Function>::New(isolate, constructor);
  Local<Object> instance = cons->NewInstance(isolate->GetCurrentContext(), 0, NULL).ToLocalChecked();
  QPointFWrap* w = node::ObjectWrap::Unwrap<QPointFWrap>(instance);
  w->SetWrapped(q);

  return instance;
}

void QPointFWrap::X(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPointFWrap* w = ObjectWrap::Unwrap<QPointFWrap>(args.This());
  QPointF* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(isolate, q->x()));
}

void QPointFWrap::Y(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPointFWrap* w = ObjectWrap::Unwrap<QPointFWrap>(args.This());
  QPointF* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(isolate, q->y()));
}

void QPointFWrap::IsNull(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPointFWrap* w = ObjectWrap::Unwrap<QPointFWrap>(args.This());
  QPointF* q = w->GetWrapped();

  args.GetReturnValue().Set(Boolean::New(isolate, q->isNull()));
}
