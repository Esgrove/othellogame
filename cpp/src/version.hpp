//==========================================================
// Version header
// Version information is set in preprocessor definitions
// Akseli Lukkarila
// 2019-2025
//==========================================================

#pragma once
namespace version
{
static constexpr auto APP_NAME = BUILDTIME_APP_NAME;
static constexpr auto BRANCH = BUILDTIME_BRANCH;
static constexpr auto BUILD_NAME = BUILDTIME_BUILD_NAME;
static constexpr auto COMMIT = BUILDTIME_COMMIT;
static constexpr auto DATE = BUILDTIME_DATE;
static constexpr auto TIMESTAMP = BUILDTIME_TIMESTAMP;
static constexpr auto VERSION_NUMBER = BUILDTIME_VERSION_NUMBER;
}  // namespace version
