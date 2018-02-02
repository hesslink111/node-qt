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
#include "qfont.h"
#include "../qt_v8.h"

using namespace v8;

Persistent<Function> QFontWrap::constructor;

// Supported implementations:
//   QFont ( )
//   QFont ( const QString & family, int pointSize = -1, int weight = -1, 
//     bool italic = false )
//   QFont ( QFont font )
QFontWrap::QFontWrap(const FunctionCallbackInfo<v8::Value>& args) : q_(NULL) {
  Isolate *isolate = args.GetIsolate();

  if (args.Length() == 0) {
    // QFont ()

    q_ = new QFont;
    return;
  }

  // QFont ( QFont font )

  if (args.Length() == 1 && args[0]->IsObject()) {
    QString arg0_constructor = 
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());

    if (arg0_constructor != "QFont")
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "QFont::QFont: bad argument")));

    // Unwrap obj
    QFontWrap* q_wrap = ObjectWrap::Unwrap<QFontWrap>(
        args[0]->ToObject());
    QFont* q = q_wrap->GetWrapped();

    q_ = new QFont(*q);
  }

  // QFont ( const QString & family, int pointSize = -1, int weight = -1, 
  //   bool italic = false )

  if (args.Length() == 1 && args[0]->IsString()) {
    q_ = new QFont(qt_v8::ToQString(args[0]->ToString()));
    return;
  }

  if (args.Length() == 2) {
    q_ = new QFont(qt_v8::ToQString(args[0]->ToString()), 
        args[1]->IntegerValue());
    return;
  }

  if (args.Length() == 3) {
    q_ = new QFont(qt_v8::ToQString(args[0]->ToString()), 
        args[1]->IntegerValue(), args[2]->IntegerValue());
    return;
  }

  if (args.Length() == 4) {
    q_ = new QFont(qt_v8::ToQString(args[0]->ToString()), 
        args[1]->IntegerValue(), args[2]->IntegerValue(),
        args[3]->BooleanValue());
    return;
  }
}

QFontWrap::~QFontWrap() {
  delete q_;
}

void QFontWrap::Initialize(Local<Object> target) {
  Isolate *isolate = target->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "QFont"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setFamily"),
      FunctionTemplate::New(isolate, SetFamily)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "family"),
      FunctionTemplate::New(isolate, Family)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setPixelSize"),
      FunctionTemplate::New(isolate, SetPixelSize)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "pixelSize"),
      FunctionTemplate::New(isolate, PixelSize)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setPointSize"),
      FunctionTemplate::New(isolate, SetPointSize)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "pointSize"),
      FunctionTemplate::New(isolate, PointSize)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setPointSizeF"),
      FunctionTemplate::New(isolate, SetPointSizeF)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "pointSizeF"),
      FunctionTemplate::New(isolate, PointSizeF)->GetFunction());

  constructor.Reset(isolate, tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "QFont"), tpl->GetFunction());
}

void QFontWrap::New(const FunctionCallbackInfo<v8::Value>& args) {
  QFontWrap* w = new QFontWrap(args);
  w->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

Local<Value> QFontWrap::NewInstance(Isolate *isolate, QFont q) {
  Local<Function> cons = Local<Function>::New(isolate, constructor);
  Local<Object> instance = cons->NewInstance(isolate->GetCurrentContext(), 0, NULL).ToLocalChecked();
  assert(instance->InternalFieldCount() > 0);
  QFontWrap* w = node::ObjectWrap::Unwrap<QFontWrap>(instance);
  w->SetWrapped(q);

  return instance;
}

void QFontWrap::SetFamily(const FunctionCallbackInfo<v8::Value>& args) {
  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(args.This());
  QFont* q = w->GetWrapped();

  q->setFamily(qt_v8::ToQString(args[0]->ToString()));

  args.GetReturnValue().SetUndefined();
}

void QFontWrap::Family(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(args.This());
  QFont* q = w->GetWrapped();

  args.GetReturnValue().Set(qt_v8::FromQString(isolate, q->family()));
}

void QFontWrap::SetPixelSize(const FunctionCallbackInfo<v8::Value>& args) {
  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(args.This());
  QFont* q = w->GetWrapped();

  q->setPixelSize(args[0]->IntegerValue());

  args.GetReturnValue().SetUndefined();
}

void QFontWrap::PixelSize(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(args.This());
  QFont* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(isolate, q->pixelSize()));
}

void QFontWrap::SetPointSize(const FunctionCallbackInfo<v8::Value>& args) {
  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(args.This());
  QFont* q = w->GetWrapped();

  q->setPointSize(args[0]->IntegerValue());

  args.GetReturnValue().SetUndefined();
}

void QFontWrap::PointSize(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(args.This());
  QFont* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(isolate, q->pointSize()));
}

void QFontWrap::SetPointSizeF(const FunctionCallbackInfo<v8::Value>& args) {
  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(args.This());
  QFont* q = w->GetWrapped();

  q->setPointSizeF(args[0]->NumberValue());

  args.GetReturnValue().SetUndefined();
}

void QFontWrap::PointSizeF(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QFontWrap* w = ObjectWrap::Unwrap<QFontWrap>(args.This());
  QFont* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(isolate, q->pointSizeF()));
}
