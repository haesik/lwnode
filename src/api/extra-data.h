/*
 * Copyright (c) 2021-present Samsung Electronics Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <EscargotPublic.h>
#include "utils/gc.h"
#include "v8.h"

namespace EscargotShim {

class ArrayBufferObjectData;
class ValueWrap;
class BackingStoreWrapHolder;

using namespace Escargot;

class ObjectData : public gc {
 public:
  ArrayBufferObjectData* asArrayBufferObjectData() {
    LWNODE_CHECK(isArryBufferObjectData());
    return reinterpret_cast<ArrayBufferObjectData*>(this);
  }

  virtual bool isArryBufferObjectData() const { return false; }
  virtual bool isFunctionData() const { return false; }

  // InternalFields
  int internalFieldCount();
  void setInternalFieldCount(int size);
  void setInternalField(int idx, ValueWrap* lwValue);
  ValueWrap* internalField(int idx);

 private:
  GCContainer<ValueWrap*>* m_internalFields{nullptr};
};

class FunctionData : public ObjectData {
 public:
  FunctionData() = default;
  FunctionData(v8::Isolate* isolate,
               v8::FunctionCallback callback,
               v8::Local<v8::Value> callbackData,
               v8::Local<v8::Signature> signature,
               int length)
      : m_isolate(isolate),
        m_callback(callback),
        m_callbackData(callbackData),
        m_signature(signature),
        m_length(length) {}

  bool isFunctionData() const override { return true; }

  static FunctionData* toFunctionData(void* ptr) {
    LWNODE_CHECK_NOT_NULL(ptr);
    auto data = reinterpret_cast<FunctionData*>(ptr);
    LWNODE_CHECK(data->isFunctionData());
    return data;
  }

  v8::Isolate* isolate() { return m_isolate; }
  v8::FunctionCallback callback() { return m_callback; }
  void setCallback(v8::FunctionCallback callback) { m_callback = callback; }
  v8::Local<v8::Value> callbackData() { return m_callbackData; }
  void setCallbackData(v8::Local<v8::Value> data) { m_callbackData = data; }

 private:
  v8::Isolate* m_isolate{nullptr};
  v8::FunctionCallback m_callback{nullptr};
  v8::Local<v8::Value> m_callbackData;
  v8::Local<v8::Signature> m_signature;
  int m_length{0};
};

class ArrayBufferObjectData : public ObjectData {
 public:
  bool isArryBufferObjectData() const override { return true; }

  void setBackingStoreWrapHolder(BackingStoreWrapHolder* holder) {
    m_backingStoreWrapHolder = holder;
  }

  BackingStoreWrapHolder* backingStoreWrapHolder() {
    return m_backingStoreWrapHolder.get();
  }

 private:
  PersistentRefHolder<BackingStoreWrapHolder> m_backingStoreWrapHolder;
};

}  // namespace EscargotShim