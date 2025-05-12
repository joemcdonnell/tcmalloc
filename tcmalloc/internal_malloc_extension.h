// Copyright 2019 The TCMalloc Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Extra extensions exported by some malloc implementations.  These
// extensions are accessed through a virtual base class so an
// application can link against a malloc that does not implement these
// extensions, and it will get default versions that do nothing.

#ifndef TCMALLOC_INTERNAL_MALLOC_EXTENSION_H_
#define TCMALLOC_INTERNAL_MALLOC_EXTENSION_H_

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "absl/base/attributes.h"
#include "absl/time/time.h"
#include "tcmalloc/malloc_extension.h"

namespace tcmalloc {
namespace tcmalloc_internal {

// AllocationProfilingTokenAccessor and ProfileAccessor provide access to the
// private constructors of AllocationProfilingToken and Profile that take a
// pointer.
class AllocationProfilingTokenAccessor {
 public:
  static MallocExtension::AllocationProfilingToken MakeToken(
      std::unique_ptr<AllocationProfilingTokenBase> p) {
    return MallocExtension::AllocationProfilingToken(std::move(p));
  }
};

class ProfileAccessor {
 public:
  static Profile MakeProfile(std::unique_ptr<const ProfileBase> p) {
    return Profile(std::move(p));
  }
};

}  // namespace tcmalloc_internal
}  // namespace tcmalloc

#if ABSL_HAVE_ATTRIBUTE_WEAK && !defined(__APPLE__) && !defined(__EMSCRIPTEN__)

extern "C" {

void TCMalloc_Internal_ForceCpuCacheActivation();

tcmalloc::AddressRegionFactory*
MallocExtension_Internal_GetRegionFactory();
void MallocExtension_Internal_SetRegionFactory(
    tcmalloc::AddressRegionFactory* factory);

const tcmalloc::tcmalloc_internal::ProfileBase*
MallocExtension_Internal_SnapshotCurrent(tcmalloc::ProfileType type);

tcmalloc::tcmalloc_internal::AllocationProfilingTokenBase*
MallocExtension_Internal_StartAllocationProfiling();
tcmalloc::tcmalloc_internal::AllocationProfilingTokenBase*
MallocExtension_Internal_StartLifetimeProfiling();

void MallocExtension_Internal_ActivateGuardedSampling();
tcmalloc::MallocExtension::Ownership
MallocExtension_Internal_GetOwnership(const void* ptr);
size_t MallocExtension_Internal_GetMemoryLimit(
    tcmalloc::MallocExtension::LimitKind limit_kind);
bool MallocExtension_Internal_GetNumericProperty(
    const char* name_data, size_t name_size, size_t* value);
bool MallocExtension_Internal_GetPerCpuCachesActive();
int32_t MallocExtension_Internal_GetMaxPerCpuCacheSize();
bool
MallocExtension_Internal_GetBackgroundProcessActionsEnabled();
void
MallocExtension_Internal_GetBackgroundProcessSleepInterval(absl::Duration* ret);
void MallocExtension_Internal_GetSkipSubreleaseInterval(
    absl::Duration* ret);
void
MallocExtension_Internal_GetSkipSubreleaseShortInterval(absl::Duration* ret);
void MallocExtension_Internal_GetSkipSubreleaseLongInterval(
    absl::Duration* ret);
void
MallocExtension_Internal_GetCacheDemandReleaseShortInterval(
    absl::Duration* ret);
void
MallocExtension_Internal_GetCacheDemandReleaseLongInterval(absl::Duration* ret);
void MallocExtension_Internal_GetProperties(
    std::map<std::string, tcmalloc::MallocExtension::Property>* ret);
void MallocExtension_Internal_GetExperiments(
    std::map<std::string, tcmalloc::MallocExtension::Property>* ret);
void MallocExtension_Internal_GetStats(std::string* ret);
void MallocExtension_Internal_SetMaxPerCpuCacheSize(
    int32_t value);
void
MallocExtension_Internal_SetBackgroundProcessActionsEnabled(bool value);
void
MallocExtension_Internal_SetBackgroundProcessSleepInterval(
    absl::Duration value);
void MallocExtension_Internal_SetSkipSubreleaseInterval(
    absl::Duration value);
void
MallocExtension_Internal_SetSkipSubreleaseShortInterval(absl::Duration value);
void MallocExtension_Internal_SetSkipSubreleaseLongInterval(
    absl::Duration value);
void
MallocExtension_Internal_SetCacheDemandReleaseShortInterval(
    absl::Duration value);
void
MallocExtension_Internal_SetCacheDemandReleaseLongInterval(
    absl::Duration value);
size_t MallocExtension_Internal_ReleaseCpuMemory(int cpu);
size_t
MallocExtension_Internal_ReleaseMemoryToSystem(size_t bytes);
void MallocExtension_Internal_SetMemoryLimit(
    size_t limit, tcmalloc::MallocExtension::LimitKind limit_kind);

size_t MallocExtension_Internal_GetEstimatedAllocatedSize(
    size_t bytes, tcmalloc::hot_cold_t hot_cold);
size_t
MallocExtension_Internal_GetAllocatedSize(const void* ptr);
void MallocExtension_Internal_MarkThreadBusy();
void MallocExtension_Internal_MarkThreadIdle();

int64_t
MallocExtension_Internal_GetProfileSamplingInterval();
void MallocExtension_Internal_SetProfileSamplingInterval(
    int64_t);

void MallocExtension_Internal_ProcessBackgroundActions();

tcmalloc::MallocExtension::BytesPerSecond
MallocExtension_Internal_GetBackgroundReleaseRate();
void MallocExtension_Internal_SetBackgroundReleaseRate(
    tcmalloc::MallocExtension::BytesPerSecond);

int64_t
MallocExtension_Internal_GetGuardedSamplingInterval();
void MallocExtension_Internal_SetGuardedSamplingInterval(
    int64_t);

int64_t
MallocExtension_Internal_GetMaxTotalThreadCacheBytes();
void MallocExtension_Internal_SetMaxTotalThreadCacheBytes(
    int64_t value);
}

#endif

#endif  // TCMALLOC_INTERNAL_MALLOC_EXTENSION_H_
