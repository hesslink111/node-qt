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
#include "qmatrix.h"
#include "../qt_v8.h"

using namespace v8;

Persistent<Function> QMatrixWrap::constructor;

// Supported implementations:
//   QMatrix ( )
//   QMatrix ( qreal m11, qreal m12, qreal m21, qreal m22, qreal dx, qreal dy )
//   QMatrix ( QMatrix matrix )
QMatrixWrap::QMatrixWrap(const FunctionCallbackInfo<v8::Value>& args) : q_(NULL) {
  if (args.Length() == 0) {
    // QMatrix ( )

    q_ = new QMatrix;
  } else if (args[0]->IsObject()) {
    // QMatrix ( QMatrix matrix )

    QString arg0_constructor = 
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());

    if (arg0_constructor != "QMatrix")
      ThrowException(Exception::TypeError(
        String::New("QMatrix::QMatrix: bad argument")));

    // Unwrap obj
    QMatrixWrap* q_wrap = ObjectWrap::Unwrap<QMatrixWrap>(
        args[0]->ToObject());
    QMatrix* q = q_wrap->GetWrapped();

    q_ = new QMatrix(*q);
  } else if (args.Length() == 6) {
    // QMatrix(qreal m11, qreal m12, qreal m21, qreal m22, qreal dx, qreal dy)

    q_ = new QMatrix(args[0]->NumberValue(), args[1]->NumberValue(),
                     args[2]->NumberValue(), args[3]->NumberValue(),
                     args[4]->NumberValue(), args[5]->NumberValue());
  }
}

QMatrixWrap::~QMatrixWrap() {
  delete q_;
}

void QMatrixWrap::Initialize(Handle<Object> target) {
  Isolate *isolate = target->GetIsolate();
  
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "QMatrix"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "m11"),
      FunctionTemplate::New(M11)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "m12"),
      FunctionTemplate::New(M12)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "m21"),
      FunctionTemplate::New(M21)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "m22"),
      FunctionTemplate::New(M22)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "dx"),
      FunctionTemplate::New(Dx)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "dy"),
      FunctionTemplate::New(Dy)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "translate"),
      FunctionTemplate::New(Translate)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "scale"),
      FunctionTemplate::New(Scale)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "QMatrix"), constructor);
}

Handle<Value> QMatrixWrap::New(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QMatrixWrap* w = new QMatrixWrap(args);
  w->Wrap(args.This());

  return args.This();
}

Handle<Value> QMatrixWrap::NewInstance(QMatrix q) {
  HandleScope scope;
  
  Local<Object> instance = constructor->NewInstance(0, NULL);
  QMatrixWrap* w = node::ObjectWrap::Unwrap<QMatrixWrap>(instance);
  w->SetWrapped(q);

  args.GetReturnValue().Set(instance);
}

Handle<Value> QMatrixWrap::M11(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(args.This());
  QMatrix* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(q->m11()));
}

Handle<Value> QMatrixWrap::M12(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(args.This());
  QMatrix* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(q->m12()));
}

Handle<Value> QMatrixWrap::M21(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(args.This());
  QMatrix* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(q->m21()));
}

Handle<Value> QMatrixWrap::M22(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(args.This());
  QMatrix* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(q->m22()));
}

Handle<Value> QMatrixWrap::Dx(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(args.This());
  QMatrix* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(q->dx()));
}

Handle<Value> QMatrixWrap::Dy(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(args.This());
  QMatrix* q = w->GetWrapped();

  args.GetReturnValue().Set(Number::New(q->dy()));
}

Handle<Value> QMatrixWrap::Translate(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(args.This());
  QMatrix* q = w->GetWrapped();

  q->translate(args[0]->NumberValue(), args[1]->NumberValue());

  args.GetReturnValue().Set(args.This());
}

Handle<Value> QMatrixWrap::Scale(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QMatrixWrap* w = ObjectWrap::Unwrap<QMatrixWrap>(args.This());
  QMatrix* q = w->GetWrapped();

  q->scale(args[0]->NumberValue(), args[1]->NumberValue());

  args.GetReturnValue().Set(args.This());
}
