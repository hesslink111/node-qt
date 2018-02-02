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
  Isolate *isolate = args.GetIsolate();

  if (args.Length() == 0) {
    // QScrollArea ( )

    q_ = new QScrollArea;
    return;
  }

  // QScrollArea ( QWidget widget )

  QString arg0_constructor = 
      qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());

  if (arg0_constructor != "QWidget")
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "QScrollArea::constructor: bad argument")));

  // Unwrap obj
  QWidgetWrap* q_wrap = ObjectWrap::Unwrap<QWidgetWrap>(
      args[0]->ToObject());
  QWidget* q = q_wrap->GetWrapped();

  q_ = new QScrollArea(q);
}

QScrollAreaWrap::~QScrollAreaWrap() {
  delete q_;
}

void QScrollAreaWrap::Initialize(Local<Object> target) {
  Isolate *isolate = target->GetIsolate();
  
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "QScrollArea"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Wrapped methods
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "resize"),
      FunctionTemplate::New(isolate, Resize)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "show"),
      FunctionTemplate::New(isolate, Show)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "size"),
      FunctionTemplate::New(isolate, Size)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "width"),
      FunctionTemplate::New(isolate, Width)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "height"),
      FunctionTemplate::New(isolate, Height)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "parent"),
      FunctionTemplate::New(isolate, Parent)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "objectName"),
      FunctionTemplate::New(isolate, ObjectName)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setObjectName"),
      FunctionTemplate::New(isolate, SetObjectName)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "update"),
      FunctionTemplate::New(isolate, Update)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setFocusPolicy"),
      FunctionTemplate::New(isolate, SetFocusPolicy)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "move"),
      FunctionTemplate::New(isolate, Move)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "x"),
      FunctionTemplate::New(isolate, X)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "y"),
      FunctionTemplate::New(isolate, Y)->GetFunction());

  // QScrollArea-specific
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setWidget"),
      FunctionTemplate::New(isolate, SetWidget)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "widget"),
      FunctionTemplate::New(isolate, Widget)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setFrameShape"),
      FunctionTemplate::New(isolate, SetFrameShape)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setVerticalScrollBarPolicy"),
      FunctionTemplate::New(isolate, SetVerticalScrollBarPolicy)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setHorizontalScrollBarPolicy"),
      FunctionTemplate::New(isolate, SetHorizontalScrollBarPolicy)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "verticalScrollBar"),
      FunctionTemplate::New(isolate, VerticalScrollBar)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "horizontalScrollBar"),
      FunctionTemplate::New(isolate, HorizontalScrollBar)->GetFunction());

  constructor.Reset(isolate, tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "QScrollArea"), tpl->GetFunction());
}

void QScrollAreaWrap::New(const FunctionCallbackInfo<v8::Value>& args) {
  QScrollAreaWrap* w = new QScrollAreaWrap(args);
  w->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void QScrollAreaWrap::Resize(const FunctionCallbackInfo<v8::Value>& args) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->resize(args[0]->NumberValue(), args[1]->NumberValue());

  args.GetReturnValue().SetUndefined();
}

void QScrollAreaWrap::Show(const FunctionCallbackInfo<v8::Value>& args) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->show();

  args.GetReturnValue().SetUndefined();
}

void QScrollAreaWrap::Close(const FunctionCallbackInfo<v8::Value>& args) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->close();

  args.GetReturnValue().SetUndefined();
}

void QScrollAreaWrap::Size(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set(QSizeWrap::NewInstance(isolate, q->size()));
}

void QScrollAreaWrap::Width(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set(Integer::New(isolate, q->width()));
}

void QScrollAreaWrap::Height(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set(Integer::New(isolate, q->height()));
}

void QScrollAreaWrap::ObjectName(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set(qt_v8::FromQString(isolate, q->objectName()));
}

void QScrollAreaWrap::SetObjectName(const FunctionCallbackInfo<v8::Value>& args) {
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
void QScrollAreaWrap::Parent(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set(qt_v8::FromQString(isolate, q->parent()->objectName()));
}

void QScrollAreaWrap::Update(const FunctionCallbackInfo<v8::Value>& args) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->update();

  args.GetReturnValue().SetUndefined();
}

void QScrollAreaWrap::SetWidget(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QWidget")
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "QScrollArea::SetWidget: bad argument")));

  // Unwrap obj
  QWidgetWrap* widget_wrap = ObjectWrap::Unwrap<QWidgetWrap>(
      args[0]->ToObject());
  QWidget* widget = widget_wrap->GetWrapped();

  q->setWidget(widget);

  args.GetReturnValue().SetUndefined();
}

// QUIRK:
// Does not return QWidget. Returns 1 if child widget exists, 0 otherwise
void QScrollAreaWrap::Widget(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  int retvalue = q->widget() ? 1 : 0;

  args.GetReturnValue().Set(Integer::New(isolate, retvalue));
}

void QScrollAreaWrap::SetFrameShape(const FunctionCallbackInfo<v8::Value>& args) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setFrameShape((QFrame::Shape)(args[0]->IntegerValue()));

  args.GetReturnValue().SetUndefined();
}

void QScrollAreaWrap::SetFocusPolicy(const FunctionCallbackInfo<v8::Value>& args) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setFocusPolicy((Qt::FocusPolicy)(args[0]->IntegerValue()));

  args.GetReturnValue().SetUndefined();
}

// Supported implementations:
//    move (int x, int y)
void QScrollAreaWrap::Move(const FunctionCallbackInfo<v8::Value>& args) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->move(args[0]->IntegerValue(), args[1]->IntegerValue());

  args.GetReturnValue().SetUndefined();
}

void QScrollAreaWrap::X(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set(Integer::New(isolate, q->x()));
}

void QScrollAreaWrap::Y(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set(Integer::New(isolate, q->y()));
}

void
  QScrollAreaWrap::SetVerticalScrollBarPolicy(const FunctionCallbackInfo<v8::Value>& args) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setVerticalScrollBarPolicy((Qt::ScrollBarPolicy)(args[0]->IntegerValue()));

  args.GetReturnValue().SetUndefined();
}

void
  QScrollAreaWrap::SetHorizontalScrollBarPolicy(const FunctionCallbackInfo<v8::Value>& args) {
  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setHorizontalScrollBarPolicy((Qt::ScrollBarPolicy)
      (args[0]->IntegerValue()));

  args.GetReturnValue().SetUndefined();
}

void QScrollAreaWrap::HorizontalScrollBar(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set( 
    QScrollBarWrap::NewInstance(isolate, q->horizontalScrollBar()) );
}

void QScrollAreaWrap::VerticalScrollBar(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  args.GetReturnValue().Set( 
      QScrollBarWrap::NewInstance(isolate, q->verticalScrollBar()) );
}
