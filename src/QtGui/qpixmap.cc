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
#include "qpixmap.h"
#include "qcolor.h"

using namespace v8;

Persistent<Function> QPixmapWrap::constructor;

QPixmapWrap::QPixmapWrap(int width, int height) : q_(NULL) {
  q_ = new QPixmap(width, height);
}
QPixmapWrap::~QPixmapWrap() {
  delete q_;
}

void QPixmapWrap::Initialize(Local<Object> target) {
  Isolate *isolate = target->GetIsolate();
  
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "QPixmap"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "width"),
      FunctionTemplate::New(isolate, Width)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "height"),
      FunctionTemplate::New(isolate, Height)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "save"),
      FunctionTemplate::New(isolate, Save)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "fill"),
      FunctionTemplate::New(isolate, Fill)->GetFunction());

  constructor.Reset(isolate, tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "QPixmap"), tpl->GetFunction());
}

void QPixmapWrap::New(const FunctionCallbackInfo<v8::Value>& args) {
  QPixmapWrap* w = new QPixmapWrap(args[0]->IntegerValue(), 
      args[1]->IntegerValue());
  w->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

Local<Value> QPixmapWrap::NewInstance(Isolate *isolate, QPixmap q) {
  Local<Function> cons = Local<Function>::New(isolate, constructor);
  Local<Object> instance = cons->NewInstance(isolate->GetCurrentContext(), 0, NULL).ToLocalChecked();
  QPixmapWrap* w = node::ObjectWrap::Unwrap<QPixmapWrap>(instance);
  w->SetWrapped(q);

  return instance;
}

void QPixmapWrap::Width(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPixmapWrap* w = ObjectWrap::Unwrap<QPixmapWrap>(args.This());
  QPixmap* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(isolate, q->width()));
}

void QPixmapWrap::Height(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPixmapWrap* w = ObjectWrap::Unwrap<QPixmapWrap>(args.This());
  QPixmap* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(isolate, q->height()));
}

void QPixmapWrap::Save(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPixmapWrap* w = ObjectWrap::Unwrap<QPixmapWrap>(args.This());
  QPixmap* q = w->GetWrapped();

  QString file(qt_v8::ToQString(args[0]->ToString()));

  args.GetReturnValue().Set(Boolean::New(isolate, q->save(file) ));
}

// Supports:
//    fill()
//    fill(QColor color)
void QPixmapWrap::Fill(const FunctionCallbackInfo<v8::Value>& args) {
  QPixmapWrap* w = ObjectWrap::Unwrap<QPixmapWrap>(args.This());
  QPixmap* q = w->GetWrapped();

  if (args[0]->IsObject()) {
    // Unwrap QColor
    QColorWrap* color_wrap = ObjectWrap::Unwrap<QColorWrap>(
        args[0]->ToObject());
    QColor* color = color_wrap->GetWrapped();

    q->fill(*color);
  } else {
    q->fill();
  }

  args.GetReturnValue().SetUndefined();
}
