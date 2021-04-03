#include <torch/csrc/jit/backends/backend_debug_handler.h>

namespace torch {
namespace jit {

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::atomic<int64_t> BackendDebugHandleManager::unique_debug_handle_{0};

int64_t BackendDebugHandleManager::getNextDebugHandleForInlinedCallStackPtr(
    const SourceRange& range,
    const InlinedCallStackPtr& cs_ptr) {
  int64_t debug_handle = unique_debug_handle_;
  handles_to_inlined_callstack_ptrs_[debug_handle] =
      std::make_pair(range, cs_ptr);
  // This increment is with seq memory order.
  // Not trying to perf optimizing this for now.
  unique_debug_handle_++;
  return debug_handle;
}

std::unordered_map<int64_t, DelegateDebugInfoType> BackendDebugHandleManager::
    getCallStackPtrMap() {
  // Note that this is return by copy and since
  // InlinedCallStackPtrs are intrusive ptr it will result in
  // bump of refcount. Not performant, but this is not intented
  // to be used in perf critical path.
  // Alternate might be do move but that will be destructive
  return handles_to_inlined_callstack_ptrs_;
}

} // namespace jit
} // namespace torch
