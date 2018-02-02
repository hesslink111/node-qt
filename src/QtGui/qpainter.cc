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
#include "qpainter.h"
#include "qpixmap.h"
#include "qcolor.h"
#include "qpen.h"
#include "qwidget.h"
#include "qbrush.h"
#include "qimage.h"
#include "qpainterpath.h"
#include "qfont.h"
#include "qmatrix.h"

using namespace v8;

Persistent<Function> QPainterWrap::constructor;

QPainterWrap::QPainterWrap() {
  q_ = new QPainter();
}
QPainterWrap::~QPainterWrap() {
  delete q_;
}

void QPainterWrap::Initialize(Handle<Object> target) {
  Isolate *isolate = target->GetIsolate();
  
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "QPainter"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "begin"),
      FunctionTemplate::New(isolate, Begin)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "end"),
      FunctionTemplate::New(isolate, End)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "isActive"),
      FunctionTemplate::New(isolate, IsActive)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "save"),
      FunctionTemplate::New(isolate, Save)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "restore"),
      FunctionTemplate::New(isolate, Restore)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setPen"),
      FunctionTemplate::New(isolate, SetPen)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setFont"),
      FunctionTemplate::New(isolate, SetFont)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "setMatrix"),
      FunctionTemplate::New(isolate, SetMatrix)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "fillRect"),
      FunctionTemplate::New(isolate, FillRect)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "drawText"),
      FunctionTemplate::New(isolate, DrawText)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "drawPixmap"),
      FunctionTemplate::New(isolate, DrawPixmap)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "drawImage"),
      FunctionTemplate::New(isolate, DrawImage)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(isolate, "strokePath"),
      FunctionTemplate::New(isolate, StrokePath)->GetFunction());

  constructor.Reset(isolate, tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "QPainter"), tpl->GetFunction());
}

void QPainterWrap::New(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  if (args.Length()>0) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "QPainterWrap: use begin() for initialization")));
  }

  QPainterWrap* w = new QPainterWrap();
  w->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void QPainterWrap::Begin(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  if (!args[0]->IsObject())
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "QPainterWrap:Begin: bad arguments")));

  QString constructor_name = 
    qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  
  // Determine argument type (from its constructor) so we can unwrap it
  if (constructor_name == "QPixmap") {
    // QPixmap
    QPixmapWrap* pixmap_wrap = ObjectWrap::Unwrap<QPixmapWrap>(
        args[0]->ToObject());
    QPixmap* pixmap = pixmap_wrap->GetWrapped();

    args.GetReturnValue().Set(Boolean::New(isolate, q->begin(pixmap)));
  } else if (constructor_name == "QWidget") {
    // QWidget
    QWidgetWrap* widget_wrap = ObjectWrap::Unwrap<QWidgetWrap>(
        args[0]->ToObject());
    QWidget* widget = widget_wrap->GetWrapped();

    args.GetReturnValue().Set(Boolean::New(isolate, q->begin(widget)));
  }

  // Unknown argument type
  args.GetReturnValue().Set(Boolean::New(isolate, false));
}

void QPainterWrap::End(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  args.GetReturnValue().Set(Boolean::New(isolate, q->end()));
}

void QPainterWrap::IsActive(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  args.GetReturnValue().Set(Boolean::New(isolate, q->isActive()));
}

void QPainterWrap::Save(const FunctionCallbackInfo<v8::Value>& args) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  q->save();

  args.GetReturnValue().SetUndefined();
}

void QPainterWrap::Restore(const FunctionCallbackInfo<v8::Value>& args) {
  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  q->restore();

  args.GetReturnValue().SetUndefined();
}

// Supported implementations:
//   setPen( QPen pen )
void QPainterWrap::SetPen(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QPen")
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "QPainterWrap::SetPen: bad argument")));

  // Unwrap obj
  QPenWrap* pen_wrap = ObjectWrap::Unwrap<QPenWrap>(
      args[0]->ToObject());
  QPen* pen = pen_wrap->GetWrapped();

  q->setPen(*pen);

  args.GetReturnValue().SetUndefined();
}

void QPainterWrap::SetFont(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QFont")
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "QPainterWrap::SetFont: bad argument")));

  // Unwrap obj
  QFontWrap* font_wrap = ObjectWrap::Unwrap<QFontWrap>(
      args[0]->ToObject());
  QFont* font = font_wrap->GetWrapped();

  q->setFont(*font);

  args.GetReturnValue().SetUndefined();
}

// This seems to be undocumented in Qt, but it exists!
void QPainterWrap::SetMatrix(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QMatrix")
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "QPainterWrap::SetMatrix: bad argument")));

  // Unwrap obj
  QMatrixWrap* matrix_wrap = ObjectWrap::Unwrap<QMatrixWrap>(
      args[0]->ToObject());
  QMatrix* matrix = matrix_wrap->GetWrapped();

  q->setMatrix(*matrix, args[1]->BooleanValue());

  args.GetReturnValue().SetUndefined();
}

// Supported versions:
//   fillRect(int x, int y, int w, int h, QBrush brush)
//   fillRect(int x, int y, int w, int h, QColor color)
//   fillRect(int x, int y, int w, int h, Qt::GlobalColor color)
void QPainterWrap::FillRect(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() ||
      !args[3]->IsNumber())
    args.GetReturnValue().SetUndefined();
      
  QString arg4_constructor;
  if (args[4]->IsObject()) {
    arg4_constructor = 
        qt_v8::ToQString(args[4]->ToObject()->GetConstructorName());
  }
  
  if (arg4_constructor == "QBrush") {
    // fillRect(int x, int y, int w, int h, QBrush brush)

    // Unwrap QBrush
    QBrushWrap* brush_wrap = ObjectWrap::Unwrap<QBrushWrap>(
        args[4]->ToObject());
    QBrush* brush = brush_wrap->GetWrapped();

    q->fillRect(args[0]->IntegerValue(), args[1]->IntegerValue(),
                args[2]->IntegerValue(), args[3]->IntegerValue(), 
                *brush);
  } else if (arg4_constructor == "QColor") {
    // fillRect(int x, int y, int w, int h, QColor color)

    // Unwrap QColor
    QColorWrap* color_wrap = ObjectWrap::Unwrap<QColorWrap>(
        args[4]->ToObject());
    QColor* color = color_wrap->GetWrapped();

    q->fillRect(args[0]->IntegerValue(), args[1]->IntegerValue(),
                args[2]->IntegerValue(), args[3]->IntegerValue(), 
                *color);
  } else if (args[4]->IsNumber()) {
    // fillRect(int x, int y, int w, int h, Qt::GlobalColor color)

    q->fillRect(args[0]->IntegerValue(), args[1]->IntegerValue(),
                args[2]->IntegerValue(), args[3]->IntegerValue(), 
                (Qt::GlobalColor)args[4]->IntegerValue());
  } else {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "QPainterWrap:fillRect: bad arguments")));
  }

  args.GetReturnValue().SetUndefined();
}

// Supported versions:
//   drawText(int x, int y, "text")
void QPainterWrap::DrawText(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsString())
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "QPainterWrap:DrawText: bad arguments")));
      
  q->drawText(args[0]->IntegerValue(), args[1]->IntegerValue(), 
      qt_v8::ToQString(args[2]->ToString()));

  args.GetReturnValue().SetUndefined();
}

// Supported versions:
//   drawPixmap(int x, int y, QPixmap pixmap)
void QPainterWrap::DrawPixmap(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  QString arg2_constructor;
  if (args[2]->IsObject()) {
    arg2_constructor = 
        qt_v8::ToQString(args[2]->ToObject()->GetConstructorName());
  }

  if (arg2_constructor != "QPixmap" ) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "QPainterWrap::DrawPixmap: pixmap argument not recognized")));
  }
  
  // Unwrap QPixmap
  QPixmapWrap* pixmap_wrap = ObjectWrap::Unwrap<QPixmapWrap>(
      args[2]->ToObject());
  QPixmap* pixmap = pixmap_wrap->GetWrapped();

  if (pixmap->isNull()) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "QPainterWrap::DrawPixmap: pixmap is null, no size set?")));
  }

  q->drawPixmap(args[0]->IntegerValue(), args[1]->IntegerValue(), *pixmap);

  args.GetReturnValue().SetUndefined();
}

// Supported versions:
//   drawImage( int x, int y, QImage image )
void QPainterWrap::DrawImage(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  QString arg2_constructor;
  if (args[2]->IsObject()) {
    arg2_constructor = 
        qt_v8::ToQString(args[2]->ToObject()->GetConstructorName());
  }

  if (arg2_constructor != "QImage" ) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "QPainterWrap::DrawImage: image argument not recognized")));
  }
  
  // Unwrap QImage
  QImageWrap* image_wrap = ObjectWrap::Unwrap<QImageWrap>(
      args[2]->ToObject());
  QImage* image = image_wrap->GetWrapped();

  if (image->isNull()) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "QPainterWrap::DrawImage: image is null, no size set?")));
  }

  q->drawImage(args[0]->IntegerValue(), args[1]->IntegerValue(), *image);

  args.GetReturnValue().SetUndefined();
}

// Supported versions:
//   strokePath( QPainterPath path, QPen pen )
void QPainterWrap::StrokePath(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate *isolate = args.GetIsolate();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QPainterPath") {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "QPainterWrap::StrokePath: bad arguments")));
  }
  
  QString arg1_constructor;
  if (args[1]->IsObject()) {
    arg1_constructor = 
        qt_v8::ToQString(args[1]->ToObject()->GetConstructorName());
  }

  if (arg1_constructor != "QPen") {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "QPainterWrap::StrokePath: bad arguments")));
  }

  // Unwrap QPainterPath
  QPainterPathWrap* path_wrap = ObjectWrap::Unwrap<QPainterPathWrap>(
      args[0]->ToObject());
  QPainterPath* path = path_wrap->GetWrapped();

  // Unwrap QPen
  QPenWrap* pen_wrap = ObjectWrap::Unwrap<QPenWrap>(
      args[1]->ToObject());
  QPen* pen = pen_wrap->GetWrapped();

  q->strokePath(*path, *pen);

  args.GetReturnValue().SetUndefined();
}
