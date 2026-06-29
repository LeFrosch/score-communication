# Yor task — split into 3 parts

Each part is a separate prompt for a fresh session. Run them in order in the **same** working tree:
Part 2 depends on Part 1's BUILD edits, Part 3 on Part 2's test target. Start from a clean slate
(no `score/message_passing/protocol/` directory, original `score/message_passing/BUILD`).

Convention note (applies to all parts): use `score::cpp::string_view` from `<score/string_view.hpp>`
(provided by `@score_baselibs//score/language/futurecpp`). Do NOT substitute `std::string_view`, even
though the codebase uses it elsewhere — using the `score::cpp` type from the external dependency is intentional.

---

## Part 1 — Create the implementation, then build

In this Bazel/C++ repository, add a small utility (header + source) in a new subdirectory
`score/message_passing/protocol/`, wire it into an existing target, and build.

1. Create `score/message_passing/protocol/protocol_version.h`: standard Apache-2.0 copyright block, include
   guard `SCORE_LIB_MESSAGE_PASSING_PROTOCOL_PROTOCOL_VERSION_H`, namespace `score::message_passing`. Include
   `<score/string_view.hpp>` and `<cstdint>`. Declare, each with a one-line `\brief` doc comment:
   - `constexpr std::uint32_t kProtocolVersion = 1U;` — "The message-passing wire protocol version."
   - `std::uint32_t GetProtocolVersion() noexcept;` — "Returns the message-passing wire protocol version."
   - `score::cpp::string_view GetProtocolVersionName() noexcept;` — "Returns the human-readable name of the message-passing wire protocol version."
2. Create `score/message_passing/protocol/protocol_version.cpp` (same copyright block). Include
   `"score/message_passing/protocol/protocol_version.h"`; in namespace `score::message_passing`, implement
   `GetProtocolVersion()` to return `kProtocolVersion`, and `GetProtocolVersionName()` to return
   `score::cpp::string_view{"message_passing/v1"}`.
3. Do NOT create a BUILD file in the new `protocol/` directory — the files must stay part of the existing
   `//score/message_passing` package. In `score/message_passing/BUILD`, edit the `message_passing_common`
   `cc_library`: add `"protocol/protocol_version.cpp"` to `srcs`, `"protocol/protocol_version.h"` to `hdrs`,
   and `"@score_baselibs//score/language/futurecpp"` to `deps` (keep each list alphabetically sorted).
4. Build: `bazel build //score/message_passing:message_passing_common`. Report the result.

---

## Part 2 — Create the test, then build

Continuing in the same repository, add a unit test for the `protocol_version` utility under
`score/message_passing/protocol/`, and build the test target.

1. Create `score/message_passing/protocol/protocol_version_test.cpp`: standard Apache-2.0 copyright block, then
   `#include "score/message_passing/protocol/protocol_version.h"` followed by `#include <gtest/gtest.h>`. In an
   anonymous namespace inside `score::message_passing`, add two tests:
   - `TEST(ProtocolVersionTest, ReturnsProtocolVersion)` — `EXPECT_EQ(GetProtocolVersion(), kProtocolVersion);`
     and `EXPECT_EQ(GetProtocolVersion(), 1U);`
   - `TEST(ProtocolVersionTest, ReturnsProtocolVersionName)` —
     `EXPECT_TRUE(GetProtocolVersionName() == score::cpp::string_view{"message_passing/v1"});`
2. In `score/message_passing/BUILD`, add a new `cc_unit_test` target named `protocol_version_test` with
   `srcs = ["protocol/protocol_version_test.cpp"]`, the same `features` block used by the other tests
   (`treat_warnings_as_errors`, `strict_warnings`, `additional_warnings`), and `deps = [":message_passing_common"]`.
3. Build the test target: `bazel build //score/message_passing:protocol_version_test`. Report the result.

---

## Part 3 — Run the test

Continuing in the same repository, run the unit test target and report the outcome:

`bazel test //score/message_passing:protocol_version_test --test_output=all`

Confirm both `ProtocolVersionTest.ReturnsProtocolVersion` and `ProtocolVersionTest.ReturnsProtocolVersionName` pass.
