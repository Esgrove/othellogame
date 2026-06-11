"""
Version
Version information
Akseli Lukkarila
2019-2026
"""

# Updated by build script, do not edit manually!
GIT_BRANCH = "main"
GIT_COMMIT = "700e89d"
BUILD_TIME = "2026-06-11_1440"
VERSION_NUMBER = "3.1.0"

VERSION_STRING = f"Othello Python {VERSION_NUMBER} {BUILD_TIME} {GIT_BRANCH} {GIT_COMMIT}"


def version_info() -> str:
    """Return version info string."""
    return VERSION_STRING
