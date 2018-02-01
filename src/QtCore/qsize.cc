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
#include "qsize.h"

using namespace v8;

Persistent<Function> QSizeWrap::constructor;

QSizeWrap::QSizeWrap() : q_(NULL) {
  // Standalone constructor not implemented
  // Use SetWrapped()  
}

QSizeWrap::~QSizeWrap() {
  delete q_;
}

void QSizeWrap::Initialize(Handle<Object> target) {
  Isolate *isolate = target->GetIsolate();
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "QSize"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "width"),
      FunctionTemplate::New(isolate, Width)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "height"),
      FunctionTemplate::New(isolate, Height)->GetFunction());

  constructor.Reset(isolate, tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "QSize"), tpl->GetFunction());
}

void QSizeWrap::New(const FunctionCallbackInfo<v8::Value>& args) {
  QSizeWrap* w = new QSizeWrap();
  w->Wrap(args.This());
  args.GetReturnValue().Set(args.This());
}

void QSizeWrap::Width(const FunctionCallbackInfo<v8::Value>& args) {
  QSizeWrap* w = ObjectWrap::Unwrap<QSizeWrap>(args.This());
  QSize* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(q->width()));
}

void QSizeWrap::Height(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QSizeWrap* w = ObjectWrap::Unwrap<QSizeWrap>(args.This());
  QSize* q = w->GetWrapped();

  args.GetReturnValue(Number::New(q->height()));
}
