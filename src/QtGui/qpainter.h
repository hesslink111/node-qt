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

#ifndef QPAINTERWRAP_H
#define QPAINTERWRAP_H

#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <node_object_wrap.h>
#include <QPainter>

class QPainterWrap : public node::ObjectWrap {
 public:
  static void Initialize(v8::Handle<v8::Object> target);
  QPainter* GetWrapped() const { return q_; };

 private:
  QPainterWrap();
  ~QPainterWrap();
  static v8::Persistent<v8::Function> constructor;
  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

  //
  // Wrapped methods exposed to JS
  //

  static void Begin(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void End(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void IsActive(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Save(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Restore(const v8::FunctionCallbackInfo<v8::Value>& args);

  // State
  static void SetPen(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void SetFont(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void SetMatrix(const v8::FunctionCallbackInfo<v8::Value>& args);

  // Paint actions
  static void FillRect(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void DrawText(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void DrawPixmap(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void DrawImage(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void StrokePath(const v8::FunctionCallbackInfo<v8::Value>& args);

  // Wrapped object
  QPainter* q_;
};

#endif
