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

#include <v8.h>
#include <string>

namespace Escargot {
class ValueRef;
class ExecutionStateRef;
}  // namespace Escargot

namespace LWNode {

enum Encoding { ONE_BYTE, TWO_BYTE };

struct FileData {
  void* buffer{nullptr};
  long int size{0};
  Encoding encoding{ONE_BYTE};

  FileData(void* buffer_, long int size_, Encoding encoding_) {
    buffer = buffer_;
    size = size_;
    encoding = encoding_;
  }
  FileData() = default;
};

class Loader {
 public:
  static FileData readFile(std::string filename);

  // should return string buffer
  typedef void* (*LoadCallback)(void* callbackData);
  // should free memoryPtr
  typedef void (*UnloadCallback)(void* memoryPtr, void* callbackData);

  class ReloadableSourceData {
   public:
    void* preloadedData{nullptr};

    const char* path() { return path_; }
    size_t preloadedDataLength() { return preloadedDataLength_; }
    size_t stringLength() {
      return isOneByteString_ ? preloadedDataLength_ : preloadedDataLength_ / 2;
    }
    bool isOneByteString() { return isOneByteString_; }
    static ReloadableSourceData* create(std::string sourcePath,
                                        void* preloadedData,
                                        size_t preloadedDataLength,
                                        bool isOneByteString);

   private:
    char* path_{nullptr};
    size_t preloadedDataLength_{0};
    bool isOneByteString_{false};
    ReloadableSourceData() = default;
  };

  static Escargot::ValueRef* CreateReloadableSourceFromFile(
      Escargot::ExecutionStateRef* state, std::string fileName);

  static v8::MaybeLocal<v8::String> NewReloadableString(
      v8::Isolate* isolate,
      ReloadableSourceData* data,
      LoadCallback loadCallback,
      UnloadCallback unloadCallback);
};

}  // namespace LWNode