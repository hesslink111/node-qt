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
#include "qcolor.h"
#include "../qt_v8.h"

using namespace v8;

Persistent<Function> QColorWrap::constructor;

// Supported implementations:
//   QColor ( int r, int g, int b, int a = 255 )
//   QColor ( QString color )
//   QColor ( QColor )
QColorWrap::QColorWrap(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  if (args.Length() >= 3) {
    // QColor ( int r, int g, int b, int a = 255 )
    q_ = new QColor(
        args[0]->IntegerValue(), 
        args[1]->IntegerValue(),
        args[2]->IntegerValue(), 
        args[3]->IsNumber() ? args[3]->IntegerValue() : 255
    );
  } else if (args[0]->IsString()) {
    // QColor ( QString color )
    q_ = new QColor( qt_v8::ToQString(args[0]->ToString()) );
  } else if (args[0]->IsObject()) {
    // QColor ( QColor color )
    QString arg0_constructor = 
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());

    if (arg0_constructor != "QColor")
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "QColor::QColor: bad argument")));

    // Unwrap obj
    QColorWrap* q_wrap = ObjectWrap::Unwrap<QColorWrap>(
        args[0]->ToObject());
    QColor* q = q_wrap->GetWrapped();

    q_ = new QColor(*q);
  }
}

QColorWrap::~QColorWrap() {
  delete q_;
}

void QColorWrap::Initialize(Local<Object> target) {
  Isolate *isolate = target->GetIsolate();
  
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "QColor"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "red"),
      FunctionTemplate::New(isolate, Red)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "green"),
      FunctionTemplate::New(isolate, Green)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "blue"),
      FunctionTemplate::New(isolate, Blue)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "alpha"),
      FunctionTemplate::New(isolate, Alpha)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "name"),
      FunctionTemplate::New(isolate, Name)->GetFunction());

  constructor.Reset(isolate, tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "QColor"), tpl->GetFunction());
}

void QColorWrap::New(const FunctionCallbackInfo<v8::Value>& args) {
  QColorWrap* w = new QColorWrap(args);
  w->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void QColorWrap::Red(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QColorWrap* w = ObjectWrap::Unwrap<QColorWrap>(args.This());
  QColor* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(isolate, q->red()));
}

void QColorWrap::Green(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QColorWrap* w = ObjectWrap::Unwrap<QColorWrap>(args.This());
  QColor* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(isolate, q->green()));
}

void QColorWrap::Blue(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QColorWrap* w = ObjectWrap::Unwrap<QColorWrap>(args.This());
  QColor* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(isolate, q->blue()));
}

void QColorWrap::Alpha(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QColorWrap* w = ObjectWrap::Unwrap<QColorWrap>(args.This());
  QColor* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(isolate, q->alpha()));
}

void QColorWrap::Name(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QColorWrap* w = ObjectWrap::Unwrap<QColorWrap>(args.This());
  QColor* q = w->GetWrapped();

  QString name = q->name();

  args.GetReturnValue().Set(qt_v8::FromQString(isolate, name));
}
