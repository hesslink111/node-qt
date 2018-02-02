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
#include <QFrame>
#include "../qt_v8.h"
#include "../QtCore/qsize.h"
#include "qscrollarea.h"
#include "qwidget.h"
#include "qscrollbar.h"

using namespace v8;

Persistent<Function> QScrollAreaWrap::constructor;

// Supported implementations:
//   QScrollArea ( )
//   QScrollArea ( QWidget widget )
QScrollAreaWrap::QScrollAreaWrap(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() == 0) {
    // QScrollArea ( )

    q_ = new QScrollArea;
    return;
  }

  // QScrollArea ( QWidget widget )

  QString arg0_constructor = 
      qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());

  if (arg0_constructor != "QWidget")
    ThrowException(Exception::TypeError(
      String::New("QScrollArea::constructor: bad argument")));

  // Unwrap obj
  QWidgetWrap* q_wrap = ObjectWrap::Unwrap<QWidgetWrap>(
      args[0]->ToObject());
  QWidget* q = q_wrap->GetWrapped();

  q_ = new QScrollArea(q);
}

QScrollAreaWrap::~QScrollAreaWrap() {
  delete q_;
}

void QScrollAreaWrap::Initialize(Handle<Object> target) {
  Isolate *isolate = target->GetIsolate();
  
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "QScrollArea"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Wrapped methods
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "resize"),
      FunctionTemplate::New(Resize)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "show"),
      FunctionTemplate::New(Show)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "size"),
      FunctionTemplate::New(Size)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "width"),
      FunctionTemplate::New(Width)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "height"),
      FunctionTemplate::New(Height)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "parent"),
      FunctionTemplate::New(Parent)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "objectName"),
      FunctionTemplate::New(ObjectName)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setObjectName"),
      FunctionTemplate::New(SetObjectName)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "update"),
      FunctionTemplate::New(Update)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setFocusPolicy"),
      FunctionTemplate::New(SetFocusPolicy)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "move"),
      FunctionTemplate::New(Move)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "x"),
      FunctionTemplate::New(X)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "y"),
      FunctionTemplate::New(Y)->GetFunction());

  // QScrollArea-specific
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setWidget"),
      FunctionTemplate::New(SetWidget)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "widget"),
      FunctionTemplate::New(Widget)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setFrameShape"),
      FunctionTemplate::New(SetFrameShape)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setVerticalScrollBarPolicy"),
      FunctionTemplate::New(SetVerticalScrollBarPolicy)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setHorizontalScrollBarPolicy"),
      FunctionTemplate::New(SetHorizontalScrollBarPolicy)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "verticalScrollBar"),
      FunctionTemplate::New(VerticalScrollBar)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "horizontalScrollBar"),
      FunctionTemplate::New(HorizontalScrollBar)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "QScrollArea"), constructor);
}

Handle<Value> QScrollAreaWrap::New(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = new QScrollAreaWrap(args);
  w->Wrap(args.This());

  return args.This();
}

Handle<Value> QScrollAreaWrap::Resize(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->resize(args[0]->NumberValue(), args[1]->NumberValue());

  args.GetReturnValue().SetUndefined();
}

Handle<Value> QScrollAreaWrap::Show(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->show();

  args.GetReturnValue().SetUndefined();
}

Handle<Value> QScrollAreaWrap::Close(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->close();

  args.GetReturnValue().SetUndefined();
}

Handle<Value> QScrollAreaWrap::Size(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set( QSizeWrap::NewInstance(q->size()) );
}

Handle<Value> QScrollAreaWrap::Width(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set( Integer::New(q->width()) );
}

Handle<Value> QScrollAreaWrap::Height(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set( Integer::New(q->height()) );
}

Handle<Value> QScrollAreaWrap::ObjectName(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set(qt_v8::FromQString(isolate, q->objectName()));
}

Handle<Value> QScrollAreaWrap::SetObjectName(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setObjectName(qt_v8::ToQString(args[0]->ToString()));

  args.GetReturnValue().SetUndefined();
}

//
// QUIRK:
// Here: Parent() returns the parent's name
// Qt: Parent() returns QObject
// Intended mostly for sanity checks
//
Handle<Value> QScrollAreaWrap::Parent(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set(qt_v8::FromQString(isolate, q->parent()->objectName()));
}

Handle<Value> QScrollAreaWrap::Update(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->update();

  args.GetReturnValue().SetUndefined();
}

Handle<Value> QScrollAreaWrap::SetWidget(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QWidget")
    return ThrowException(Exception::TypeError(
      String::New("QScrollArea::SetWidget: bad argument")));

  // Unwrap obj
  QWidgetWrap* widget_wrap = ObjectWrap::Unwrap<QWidgetWrap>(
      args[0]->ToObject());
  QWidget* widget = widget_wrap->GetWrapped();

  q->setWidget(widget);

  args.GetReturnValue().SetUndefined();
}

// QUIRK:
// Does not return QWidget. Returns 1 if child widget exists, 0 otherwise
Handle<Value> QScrollAreaWrap::Widget(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  int retvalue = q->widget() ? 1 : 0;

  args.GetReturnValue().Set(Integer::New(retvalue));
}

Handle<Value> QScrollAreaWrap::SetFrameShape(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setFrameShape((QFrame::Shape)(args[0]->IntegerValue()));

  args.GetReturnValue().SetUndefined();
}

Handle<Value> QScrollAreaWrap::SetFocusPolicy(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setFocusPolicy((Qt::FocusPolicy)(args[0]->IntegerValue()));

  args.GetReturnValue().SetUndefined();
}

// Supported implementations:
//    move (int x, int y)
Handle<Value> QScrollAreaWrap::Move(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->move(args[0]->IntegerValue(), args[1]->IntegerValue());

  args.GetReturnValue().SetUndefined();
}

Handle<Value> QScrollAreaWrap::X(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set(Integer::New(q->x()));
}

Handle<Value> QScrollAreaWrap::Y(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set(Integer::New(q->y()));
}

Handle<Value> 
  QScrollAreaWrap::SetVerticalScrollBarPolicy(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setVerticalScrollBarPolicy((Qt::ScrollBarPolicy)(args[0]->IntegerValue()));

  args.GetReturnValue().SetUndefined();
}

Handle<Value> 
  QScrollAreaWrap::SetHorizontalScrollBarPolicy(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setHorizontalScrollBarPolicy((Qt::ScrollBarPolicy)
      (args[0]->IntegerValue()));

  args.GetReturnValue().SetUndefined();
}

Handle<Value> QScrollAreaWrap::HorizontalScrollBar(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set( 
    QScrollBarWrap::NewInstance(q->horizontalScrollBar()) );
}

Handle<Value> QScrollAreaWrap::VerticalScrollBar(const FunctionCallbackInfo<v8::Value>& args) {
  HandleScope scope;

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set( 
      QScrollBarWrap::NewInstance(q->verticalScrollBar()) );
}
