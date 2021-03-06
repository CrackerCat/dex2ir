/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ART_RUNTIME_HANDLE_SCOPE_H_
#define ART_RUNTIME_HANDLE_SCOPE_H_

#include "base/logging.h"
#include "base/macros.h"
#include "handle.h"
#include "stack.h"
#include "utils.h"

namespace art {
namespace mirror {
class Object;
}
class Thread;

// HandleScopes can be allocated within the bridge frame between managed and native code backed by
// stack storage or manually allocated in native.
class HandleScope {
 public:
  ~HandleScope() {}

  // Number of references contained within this handle scope.
  uint32_t NumberOfReferences() const {
    return number_of_references_;
  }

  // We have versions with and without explicit pointer size of the following. The first two are
  // used at runtime, so OFFSETOF_MEMBER computes the right offsets automatically. The last one
  // takes the pointer size explicitly so that at compile time we can cross-compile correctly.

  // Returns the size of a HandleScope containing num_references handles.
  static size_t SizeOf(uint32_t num_references) {
    size_t header_size = OFFSETOF_MEMBER(HandleScope, references_);
    size_t data_size = sizeof(StackReference<mirror::Object>) * num_references;
    return header_size + data_size;
  }

  // Get the size of the handle scope for the number of entries, with padding added for potential alignment.
  static size_t GetAlignedHandleScopeSize(uint32_t num_references) {
    size_t handle_scope_size = SizeOf(num_references);
    return RoundUp(handle_scope_size, 8);
  }

  // Get the size of the handle scope for the number of entries, with padding added for potential alignment.
  static size_t GetAlignedHandleScopeSizeTarget(size_t pointer_size, uint32_t num_references) {
    // Assume that the layout is packed.
    size_t header_size = pointer_size + sizeof(number_of_references_);
    // This assumes there is no layout change between 32 and 64b.
    size_t data_size = sizeof(StackReference<mirror::Object>) * num_references;
    size_t handle_scope_size = header_size + data_size;
    return RoundUp(handle_scope_size, 8);
  }

  // Link to previous HandleScope or null.
  HandleScope* GetLink() const {
    return link_;
  }

  void SetLink(HandleScope* link) {
    DCHECK_NE(this, link);
    link_ = link;
  }

  // Sets the number_of_references_ field for constructing tables out of raw memory. Warning: will
  // not resize anything.
  void SetNumberOfReferences(uint32_t num_references) {
    number_of_references_ = num_references;
  }

  mirror::Object* GetReference(size_t i) const SHARED_LOCKS_REQUIRED(Locks::mutator_lock_)
      ALWAYS_INLINE {
    DCHECK_LT(i, number_of_references_);
    return references_[i].AsMirrorPtr();
  }

  Handle<mirror::Object> GetHandle(size_t i) SHARED_LOCKS_REQUIRED(Locks::mutator_lock_)
      ALWAYS_INLINE {
    DCHECK_LT(i, number_of_references_);
    return Handle<mirror::Object>(&references_[i]);
  }

  void SetReference(size_t i, mirror::Object* object) SHARED_LOCKS_REQUIRED(Locks::mutator_lock_)
      ALWAYS_INLINE {
    DCHECK_LT(i, number_of_references_);
    references_[i].Assign(object);
  }

  bool Contains(StackReference<mirror::Object>* handle_scope_entry) const {
    // A HandleScope should always contain something. One created by the
    // jni_compiler should have a jobject/jclass as a native method is
    // passed in a this pointer or a class
    DCHECK_GT(number_of_references_, 0U);
    return ((&references_[0] <= handle_scope_entry)
            && (handle_scope_entry <= (&references_[number_of_references_ - 1])));
  }

  // Offset of link within HandleScope, used by generated code
  static size_t LinkOffset(size_t pointer_size) {
    return 0;
  }

  // Offset of length within handle scope, used by generated code
  static size_t NumberOfReferencesOffset(size_t pointer_size) {
    return pointer_size;
  }

  // Offset of link within handle scope, used by generated code
  static size_t ReferencesOffset(size_t pointer_size) {
    return pointer_size + sizeof(number_of_references_);
  }

 protected:
  explicit HandleScope(size_t number_of_references) :
      link_(nullptr), number_of_references_(number_of_references) {
  }

  HandleScope* link_;
  uint32_t number_of_references_;

  // number_of_references_ are available if this is allocated and filled in by jni_compiler.
  StackReference<mirror::Object> references_[0];

 private:
  template<size_t kNumReferences> friend class StackHandleScope;
  DISALLOW_COPY_AND_ASSIGN(HandleScope);
};

// A wrapper which wraps around Object** and restores the pointer in the destructor.
// TODO: Add more functionality.
template<class T>
class HandleWrapper {
 public:
  HandleWrapper(T** obj, const Handle<T>& handle)
     : obj_(obj), handle_(handle) {
  }

  ~HandleWrapper() {
    *obj_ = handle_.Get();
  }

 private:
  T** obj_;
  Handle<T> handle_;
};

// Scoped handle storage of a fixed size that is usually stack allocated.
template<size_t kNumReferences>
class StackHandleScope : public HandleScope {
 public:
  explicit StackHandleScope(Thread* self);
  ~StackHandleScope();

  template<class T>
  Handle<T> NewHandle(T* object) SHARED_LOCKS_REQUIRED(Locks::mutator_lock_) {
    SetReference(pos_, object);
    return Handle<T>(GetHandle(pos_++));
  }

  template<class T>
  HandleWrapper<T> NewHandleWrapper(T** object) SHARED_LOCKS_REQUIRED(Locks::mutator_lock_) {
    SetReference(pos_, *object);
    Handle<T> h(GetHandle(pos_++));
    return HandleWrapper<T>(object, h);
  }

 private:
  // references_storage_ needs to be first so that it matches the address of references_.
  StackReference<mirror::Object> references_storage_[kNumReferences];
  Thread* const self_;
  size_t pos_;

  template<size_t kNumRefs> friend class StackHandleScope;
};

}  // namespace art

#endif  // ART_RUNTIME_HANDLE_SCOPE_H_
