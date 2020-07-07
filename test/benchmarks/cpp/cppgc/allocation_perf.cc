// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "include/cppgc/allocation.h"
#include "include/cppgc/garbage-collected.h"
#include "src/heap/cppgc/globals.h"
#include "src/heap/cppgc/heap.h"
#include "test/benchmarks/cpp/cppgc/utils.h"
#include "third_party/google_benchmark/src/include/benchmark/benchmark.h"

namespace cppgc {
namespace internal {
namespace {

using Allocate = testing::BenchmarkWithHeap;

class TinyObject final : public cppgc::GarbageCollected<TinyObject> {
 public:
  void Trace(cppgc::Visitor*) const {}
};

BENCHMARK_F(Allocate, Tiny)(benchmark::State& st) {
  Heap::NoGCScope no_gc(*Heap::From(&heap()));
  for (auto _ : st) {
    benchmark::DoNotOptimize(
        cppgc::MakeGarbageCollected<TinyObject>(heap().GetAllocationHandle()));
  }
  st.SetBytesProcessed(st.iterations() * sizeof(TinyObject));
}

class LargeObject final : public GarbageCollected<LargeObject> {
 public:
  void Trace(cppgc::Visitor*) const {}
  char padding[kLargeObjectSizeThreshold + 1];
};

BENCHMARK_F(Allocate, Large)(benchmark::State& st) {
  Heap::NoGCScope no_gc(*Heap::From(&heap()));
  for (auto _ : st) {
    benchmark::DoNotOptimize(
        cppgc::MakeGarbageCollected<LargeObject>(heap().GetAllocationHandle()));
  }
  st.SetBytesProcessed(st.iterations() * sizeof(LargeObject));
}

}  // namespace
}  // namespace internal
}  // namespace cppgc