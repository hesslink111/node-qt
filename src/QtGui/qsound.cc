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
#include "../qt_v8.h"
#include "qsound.h"

using namespace v8;

Persistent<Function> QSoundWrap::constructor;

// Supported implementations:
//   QSound ( QString filename )
QSoundWrap::QSoundWrap(const FunctionCallbackInfo<v8::Value>& args) : q_(NULL) {
  q_ = new QSound(qt_v8::ToQString(args[0]->ToString()));
}

QSoundWrap::~QSoundWrap() {
  delete q_;
}

void QSoundWrap::Initialize(Local<Object> target) {
  Isolate *isolate = target->GetIsolate();
  
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "QSound"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "play"),
      FunctionTemplate::New(isolate, Play)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "fileName"),
      FunctionTemplate::New(isolate, FileName)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setLoops"),
      FunctionTemplate::New(isolate, SetLoops)->GetFunction());

  constructor.Reset(isolate, tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "QSound"), tpl->GetFunction());
}

void QSoundWrap::New(const FunctionCallbackInfo<v8::Value>& args) {
  QSoundWrap* w = new QSoundWrap(args);
  w->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void QSoundWrap::Play(const FunctionCallbackInfo<v8::Value>& args) {
  QSoundWrap* w = ObjectWrap::Unwrap<QSoundWrap>(args.This());
  QSound* q = w->GetWrapped();

  q->play();

  args.GetReturnValue().SetUndefined();
}

void QSoundWrap::FileName(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QSoundWrap* w = ObjectWrap::Unwrap<QSoundWrap>(args.This());
  QSound* q = w->GetWrapped();

  args.GetReturnValue().Set(qt_v8::FromQString(isolate, q->fileName()));
}

void QSoundWrap::SetLoops(const FunctionCallbackInfo<v8::Value>& args) {
  QSoundWrap* w = ObjectWrap::Unwrap<QSoundWrap>(args.This());
  QSound* q = w->GetWrapped();

  q->setLoops(args[0]->IntegerValue());

  args.GetReturnValue().SetUndefined();
}
