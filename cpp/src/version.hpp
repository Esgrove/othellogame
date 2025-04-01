//==========================================================
// Version header
// Version information is set in preprocessor definitions
// Akseli Lukkarila
// 2019-2025
//==========================================================

#pragma once
namespace version
{
static constexpr auto APP_NAME = COMPILE_TIME_APP_NAME;
static constexpr auto GIT_BRANCH = COMPILE_TIME_GIT_BRANCH;
static constexpr auto GIT_COMMIT = COMPILE_TIME_GIT_COMMIT;
static constexpr auto BUILD_TIME = COMPILE_TIME_BUILD_TIME;
static constexpr auto VERSION_NUMBER = COMPILE_TIME_VERSION_NUMBER;

/// Compile-time formatted version string.
///
/// Enable once supported by all implementations.
// consteval auto make_version_string()
//{
//     return std::format(
//         "{} {} {} {} {}", APP_NAME, VERSION_NUMBER, BUILD_TIME, GIT_BRANCH, GIT_COMMIT
//     );
// }
//
// static constexpr auto VERSION_STRING = make_version_string();

static constexpr auto VERSION_STRING = COMPILE_TIME_VERSION_STRING;
}  // namespace version
