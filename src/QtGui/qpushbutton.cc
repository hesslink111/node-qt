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
#include "qwidget.h"
#include "qlayout.h"

using namespace v8;

Persistent<Function> QWidgetWrap::constructor;

//
// QPushButtonImpl()
//

QPushButtonImpl::QPushButtonImpl(QWidgetImpl* parent) : QPushButton(parent) {
  // Initialize callbacks as boolean values so we can test if the callback
  // has been set via ->IsFunction() below
  clickedCallback_.Reset();
}

QPushButtonImpl::~QPushButtonImpl() {
  clickedCallback_.Reset();
}

void QPushButtonImpl::clicked() {
  Isolate *isolate = Isolate::GetCurrent();

  if (clickedCallback_.IsEmpty()
      || !clickedCallback_.Get(isolate)->IsFunction())
    return;

  const unsigned argc = 0;
  Handle<Value> argv[1] = {};
  Local<Function> cb = Local<Function>::Cast(clickedCallback_.Get(isolate));
    
  cb->Call(Null(isolate), argc, argv);
}

//
// QPushButtonWrap()
//

QPushButtonWrap::QPushButtonWrap(QWidgetImpl* parent) {
  q_ = new QPushButtonImpl(parent);
}

QPushButtonWrap::~QPushButtonWrap() {
  delete q_;
}

void QPushButtonWrap::Initialize(Local<Object> target) {
  Isolate *isolate = target->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "QPushButton"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Wrapped methods
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setText"),
      FunctionTemplate::New(isolate, SetText)->GetFunction());

  // Events
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "clicked"),
      FunctionTemplate::New(isolate, Clicked)->GetFunction());

  constructor.Reset(isolate, tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "QPushButton"), tpl->GetFunction());
}

void QPushButtonWrap::New(const FunctionCallbackInfo<v8::Value>& args) {  
  QWidgetImpl* q_parent = 0;

  if (args.Length() > 0) {
    QWidgetWrap* w_parent = node::ObjectWrap::Unwrap<QWidgetWrap>(args[0]->ToObject());
    q_parent = w_parent->GetWrapped();
  }

  QPushButtonWrap* w = new QPushButtonWrap(q_parent);
  w->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void QPushButtonWrap::SetText(const FunctionCallbackInfo<v8::Value>& args) {
  QPushButtonWrap* w = node::ObjectWrap::Unwrap<QPushButtonWrap>(args.This());
  QPushButtonImpl* q = w->GetWrapped();

  q->setText(qt_v8::ToQString(args[0]->ToString());

  args.GetReturnValue().SetUndefined();
}

//
// Clicked()
// Binds a callback to Qt's event
//
void QPushButtonWrap::Clicked(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPushButtonWrap* w = node::ObjectWrap::Unwrap<QPushButtonWrap>(args.This());
  QPushButtonImpl* q = w->GetWrapped();

  q->clickedCallback_.Reset(isolate, Local<Function>::Cast(args[0]));

  args.GetReturnValue().SetUndefined();
}

