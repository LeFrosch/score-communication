// Demo program built for both QNX and Linux via the platform_transition_binary
// targets in the BUILD file (:main_qnx and :main_gcc).

#include "score/mw/com/types.h"

#include <sys/utsname.h>

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string_view>

// ---------------------------------------------------------------------------
// Platform-specific includes. Only the active branch is compiled, and in the
// IDE only the selected toolchain's headers resolve.
// ---------------------------------------------------------------------------
#if defined(__QNX__)
#include <sys/neutrino.h>  // ClockCycles()
#include <sys/syspage.h>   // SYSPAGE_ENTRY, cycles-per-second
#else
#include <time.h>    // clock_gettime()
#include <unistd.h>  // sysconf()
#endif

namespace
{

// Compile-time platform tag. The IDE dims whichever branch is inactive for the
// current configuration.
#if defined(__QNX__)
constexpr std::string_view kPlatformName{"QNX"};
#else
constexpr std::string_view kPlatformName{"Linux"};
#endif

// A high-resolution timestamp obtained through genuinely different OS APIs.
// On QNX this reads the CPU cycle counter; on Linux it queries the monotonic
// clock. Same signature, completely different implementation and symbols.
std::uint64_t MonotonicTicks() noexcept
{
#if defined(__QNX__)
    return static_cast<std::uint64_t>(ClockCycles());
#else
    struct timespec ts{};
    ::clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<std::uint64_t>(ts.tv_sec) * 1'000'000'000ULL + static_cast<std::uint64_t>(ts.tv_nsec);
#endif
}

}  // namespace

int main()
{
    // Exercise the //score/mw/com dependency: linking this already pulls in the
    // platform-specific middleware binding selected by the transition.
    const auto instance_specifier =
        score::mw::com::InstanceSpecifier::Create(std::string{"score/transition/DemoInstance"});
    if (!instance_specifier.has_value())
    {
        std::cerr << "Failed to create InstanceSpecifier: " << instance_specifier.error() << '\n';
        return EXIT_FAILURE;
    }

    std::cout << "mw::com InstanceSpecifier: " << instance_specifier.value().ToString() << '\n';
    std::cout << "Compiled for: " << kPlatformName << '\n';

    const std::uint64_t t0 = MonotonicTicks();
    const std::uint64_t t1 = MonotonicTicks();
    std::cout << "Tick delta:   " << (t1 - t0) << '\n';

    return EXIT_SUCCESS;
}
