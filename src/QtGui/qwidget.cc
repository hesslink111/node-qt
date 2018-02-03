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
#include "../QtCore/qsize.h"
#include "qwidget.h"
#include "qmouseevent.h"
#include "qkeyevent.h"
#include "qvboxlayout.h"

using namespace v8;

Persistent<Function> QWidgetWrap::constructor;

//
// QWidgetImpl()
//

QWidgetImpl::QWidgetImpl(QWidgetImpl* parent) : QWidget(parent) {
  // Initialize callbacks as boolean values so we can test if the callback
  // has been set via ->IsFunction() below
  paintEventCallback_.Reset();
  mousePressCallback_.Reset();
  mouseReleaseCallback_.Reset();
  mouseMoveCallback_.Reset();
  keyPressCallback_.Reset();
  keyReleaseCallback_.Reset();
}

QWidgetImpl::~QWidgetImpl() {
  paintEventCallback_.Reset();
  mousePressCallback_.Reset();
  mouseReleaseCallback_.Reset();
  mouseMoveCallback_.Reset();
  keyPressCallback_.Reset();
  keyReleaseCallback_.Reset();
}

void QWidgetImpl::paintEvent(QPaintEvent* e) {
  Isolate *isolate = Isolate::GetCurrent();

  if (paintEventCallback_.IsEmpty()
      || !paintEventCallback_.Get(isolate)->IsFunction())
    return;

  const unsigned argc = 0;
  Handle<Value> argv[1] = {};
  Local<Function> cb = Local<Function>::Cast(paintEventCallback_.Get(isolate));
    
  cb->Call(Null(isolate), argc, argv);
}

void QWidgetImpl::mousePressEvent(QMouseEvent* e) {
  e->ignore(); // ensures event bubbles up

  Isolate *isolate = Isolate::GetCurrent();
  
  if (mousePressCallback_.IsEmpty()
      || !mousePressCallback_.Get(isolate)->IsFunction())
    return;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QMouseEventWrap::NewInstance(isolate, *e)
  };
  Local<Function> cb = Local<Function>::Cast(mousePressCallback_.Get(isolate));
    
  cb->Call(Null(isolate), argc, argv);
}

void QWidgetImpl::mouseReleaseEvent(QMouseEvent* e) {
  e->ignore(); // ensures event bubbles up

  Isolate *isolate = Isolate::GetCurrent();
  
  if (mouseReleaseCallback_.IsEmpty()
      || !mouseReleaseCallback_.Get(isolate)->IsFunction())
    return;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QMouseEventWrap::NewInstance(isolate, *e)
  };
  Local<Function> cb = Local<Function>::Cast(mouseReleaseCallback_.Get(isolate));
    
  cb->Call(Null(isolate), argc, argv);
}

void QWidgetImpl::mouseMoveEvent(QMouseEvent* e) {
  e->ignore(); // ensures event bubbles up

  Isolate *isolate = Isolate::GetCurrent();
  
  if (mouseMoveCallback_.IsEmpty()
      || !mouseMoveCallback_.Get(isolate)->IsFunction())
    return;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QMouseEventWrap::NewInstance(isolate, *e)
  };
  Local<Function> cb = Local<Function>::Cast(mouseMoveCallback_.Get(isolate));
    
  cb->Call(Null(isolate), argc, argv);
}

void QWidgetImpl::keyPressEvent(QKeyEvent* e) {
  e->ignore(); // ensures event bubbles up

  Isolate *isolate = Isolate::GetCurrent();
  
  if (keyPressCallback_.IsEmpty()
      || !keyPressCallback_.Get(isolate)->IsFunction())
    return;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QKeyEventWrap::NewInstance(isolate, *e)
  };
  Local<Function> cb = Local<Function>::Cast(keyPressCallback_.Get(isolate));
    
  cb->Call(Null(isolate), argc, argv);
}

void QWidgetImpl::keyReleaseEvent(QKeyEvent* e) {
  e->ignore(); // ensures event bubbles up
  
  Isolate *isolate = Isolate::GetCurrent();

  if (keyReleaseCallback_.IsEmpty()
      || !keyReleaseCallback_.Get(isolate)->IsFunction())
    return;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QKeyEventWrap::NewInstance(isolate, *e)
  };
  Local<Function> cb = Local<Function>::Cast(keyReleaseCallback_.Get(isolate));
    
  cb->Call(Null(isolate), argc, argv);
}

//
// QWidgetWrap()
//

QWidgetWrap::QWidgetWrap(QWidgetImpl* parent) {
  q_ = new QWidgetImpl(parent);
}

QWidgetWrap::~QWidgetWrap() {
  delete q_;
}

void QWidgetWrap::Initialize(Local<Object> target) {
  Isolate *isolate = target->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "QWidget"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Wrapped methods
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "resize"),
      FunctionTemplate::New(isolate, Resize)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "show"),
      FunctionTemplate::New(isolate, Show)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "close"),
      FunctionTemplate::New(isolate, Close)->GetFunction());
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
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "hasMouseTracking"),
      FunctionTemplate::New(isolate, HasMouseTracking)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setMouseTracking"),
      FunctionTemplate::New(isolate, SetMouseTracking)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setFocusPolicy"),
      FunctionTemplate::New(isolate, SetFocusPolicy)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "move"),
      FunctionTemplate::New(isolate, Move)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "x"),
      FunctionTemplate::New(isolate, X)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "y"),
      FunctionTemplate::New(isolate, Y)->GetFunction());

  // Events
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "paintEvent"),
      FunctionTemplate::New(isolate, PaintEvent)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "mousePressEvent"),
      FunctionTemplate::New(isolate, MousePressEvent)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "mouseReleaseEvent"),
      FunctionTemplate::New(isolate, MouseReleaseEvent)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "mouseMoveEvent"),
      FunctionTemplate::New(isolate, MouseMoveEvent)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "keyPressEvent"),
      FunctionTemplate::New(isolate, KeyPressEvent)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "keyReleaseEvent"),
      FunctionTemplate::New(isolate, KeyReleaseEvent)->GetFunction());

  constructor.Reset(isolate, tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "QWidget"), tpl->GetFunction());
}

void QWidgetWrap::New(const FunctionCallbackInfo<v8::Value>& args) {  
  QWidgetImpl* q_parent = 0;

  if (args.Length() > 0) {
    QWidgetWrap* w_parent = node::ObjectWrap::Unwrap<QWidgetWrap>(args[0]->ToObject());
    q_parent = w_parent->GetWrapped();
  }

  QWidgetWrap* w = new QWidgetWrap(q_parent);
  w->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void QWidgetWrap::SetLayout(const FunctionCallbackInfo<v8::Value>& args) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  QVBoxLayoutWrap* wl = node::ObjectWrap::Unwrap<QVBoxLayoutWrap>(args[0]);
  QVBoxLayoutImpl* l = wl->GetWrapped();

  q->setLayout(l);

  args.GetReturnValue().SetUndefined();
}

void QWidgetWrap::Resize(const FunctionCallbackInfo<v8::Value>& args) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->resize(args[0]->NumberValue(), args[1]->NumberValue());

  args.GetReturnValue().SetUndefined();
}

void QWidgetWrap::Show(const FunctionCallbackInfo<v8::Value>& args) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->show();

  args.GetReturnValue().SetUndefined();
}

void QWidgetWrap::Close(const FunctionCallbackInfo<v8::Value>& args) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->close();

  args.GetReturnValue().SetUndefined();
}

void QWidgetWrap::Size(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  args.GetReturnValue().Set(QSizeWrap::NewInstance(isolate, q->size()) );
}

void QWidgetWrap::Width(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  args.GetReturnValue().Set(Integer::New(isolate, q->width()) );
}

void QWidgetWrap::Height(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  args.GetReturnValue().Set(Integer::New(isolate, q->height()) );
}

void QWidgetWrap::ObjectName(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  args.GetReturnValue().Set(qt_v8::FromQString(isolate, q->objectName()));
}

void QWidgetWrap::SetObjectName(const FunctionCallbackInfo<v8::Value>& args) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->setObjectName(qt_v8::ToQString(args[0]->ToString()));

  args.GetReturnValue().SetUndefined();
}

//
// QUIRK:
// Here: Parent() returns the parent's name
// Qt: Parent() returns QObject
// Intended mostly for sanity checks
//
void QWidgetWrap::Parent(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();
  
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  args.GetReturnValue().Set(qt_v8::FromQString(isolate, q->parent()->objectName()));
}

//
// PaintEvent()
// Binds a callback to Qt's event
//
void QWidgetWrap::PaintEvent(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->paintEventCallback_.Reset(isolate, Local<Function>::Cast(args[0]));

  args.GetReturnValue().SetUndefined();
}

//
// MousePressEvent()
// Binds a callback to Qt's event
//
void QWidgetWrap::MousePressEvent(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->mousePressCallback_.Reset(isolate, Local<Function>::Cast(args[0]));

  args.GetReturnValue().SetUndefined();
}

//
// MouseReleaseEvent()
// Binds a callback to Qt's event
//
void QWidgetWrap::MouseReleaseEvent(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->mouseReleaseCallback_.Reset(isolate, Local<Function>::Cast(args[0]));

  args.GetReturnValue().SetUndefined();
}

//
// MouseMoveEvent()
// Binds a callback to Qt's event
//
void QWidgetWrap::MouseMoveEvent(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();
  
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->mouseMoveCallback_.Reset(isolate, Local<Function>::Cast(args[0]));

  args.GetReturnValue().SetUndefined();
}

//
// KeyPressEvent()
// Binds a callback to Qt's event
//
void QWidgetWrap::KeyPressEvent(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->keyPressCallback_.Reset(isolate, Local<Function>::Cast(args[0]));

  args.GetReturnValue().SetUndefined();
}

//
// KeyReleaseEvent()
// Binds a callback to Qt's event
//
void QWidgetWrap::KeyReleaseEvent(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->keyReleaseCallback_.Reset(isolate, Local<Function>::Cast(args[0]));

  args.GetReturnValue().SetUndefined();
}

void QWidgetWrap::Update(const FunctionCallbackInfo<v8::Value>& args) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->update();

  args.GetReturnValue().SetUndefined();
}

void QWidgetWrap::HasMouseTracking(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  args.GetReturnValue().Set(Boolean::New(isolate, q->hasMouseTracking()));
}

void QWidgetWrap::SetMouseTracking(const FunctionCallbackInfo<v8::Value>& args) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->setMouseTracking(args[0]->BooleanValue());

  args.GetReturnValue().SetUndefined();
}

void QWidgetWrap::SetFocusPolicy(const FunctionCallbackInfo<v8::Value>& args) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->setFocusPolicy((Qt::FocusPolicy)(args[0]->IntegerValue()));

  args.GetReturnValue().SetUndefined();
}

// Supported implementations:
//    move (int x, int y)
void QWidgetWrap::Move(const FunctionCallbackInfo<v8::Value>& args) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->move(args[0]->IntegerValue(), args[1]->IntegerValue());

  args.GetReturnValue().SetUndefined();
}

void QWidgetWrap::X(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  args.GetReturnValue().Set(Integer::New(isolate, q->x()));
}

void QWidgetWrap::Y(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  args.GetReturnValue().Set(Integer::New(isolate, q->y()));
}
