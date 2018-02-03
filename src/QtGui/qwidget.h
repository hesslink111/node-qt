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

#ifndef QWIDGETWRAP_H
#define QWIDGETWRAP_H

#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <node_object_wrap.h>
#include <QWidget>

//
// QWidgetImpl()
// Extends QWidget to implement virtual methods from QWidget
//
class QWidgetImpl : public QWidget {
 public:
  QWidgetImpl(QWidgetImpl* parent);
  ~QWidgetImpl();  
  v8::Persistent<v8::Value> paintEventCallback_;
  v8::Persistent<v8::Value> mousePressCallback_;
  v8::Persistent<v8::Value> mouseReleaseCallback_;
  v8::Persistent<v8::Value> mouseMoveCallback_;
  v8::Persistent<v8::Value> keyPressCallback_;
  v8::Persistent<v8::Value> keyReleaseCallback_;

 private:
  void paintEvent(QPaintEvent* e);
  void mousePressEvent(QMouseEvent* e);
  void mouseReleaseEvent(QMouseEvent* e);
  void mouseMoveEvent(QMouseEvent* e);
  void keyPressEvent(QKeyEvent* e);
  void keyReleaseEvent(QKeyEvent* e);
};

//
// QWidgetWrap()
//
class QWidgetWrap : public node::ObjectWrap {
 public:
  static void Initialize(v8::Local<v8::Object> target);
  QWidgetImpl* GetWrapped() const { return q_; };

 private:
  QWidgetWrap(QWidgetImpl* parent);
  ~QWidgetWrap();
  static v8::Persistent<v8::Function> constructor;
  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

  // Wrapped methods
  static void SetLayout(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Resize(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Show(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Close(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Size(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Width(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Height(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void ObjectName(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void SetObjectName(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Parent(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Update(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void SetMouseTracking(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void HasMouseTracking(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void SetFocusPolicy(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Move(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void X(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Y(const v8::FunctionCallbackInfo<v8::Value>& args);

  // QUIRK
  // Event binding. These functions bind implemented event handlers above
  // to the given callbacks. This is necessary as in Qt such handlers
  // are virtual and we can't dynamically implement them from JS
  static void PaintEvent(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void MousePressEvent(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void MouseReleaseEvent(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void MouseMoveEvent(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void KeyPressEvent(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void KeyReleaseEvent(const v8::FunctionCallbackInfo<v8::Value>& args);

  // Wrapped object
  QWidgetImpl* q_;
};

#endif
