#!/bin/sh
# Generate versionInfo.swift with build metadata.
# Usage: generate_version.sh <output_dir> <version_number>
set -e

OUTPUT_DIR="$1"
VERSION_NUMBER="$2"

GIT_COMMIT=$(git rev-parse --short HEAD 2> /dev/null || echo "unknown")
GIT_BRANCH=$(git rev-parse --abbrev-ref HEAD 2> /dev/null || echo "unknown")
BUILD_TIME=$(date -u +"%Y-%m-%d_%H%M" 2> /dev/null || echo "unknown")
VERSION_STRING="Othello Swift ${VERSION_NUMBER} ${BUILD_TIME} ${GIT_BRANCH} ${GIT_COMMIT}"

mkdir -p "$OUTPUT_DIR"

cat > "$OUTPUT_DIR/versionInfo.swift" << EOF
// Generated automatically by build plugin; DO NOT EDIT MANUALLY.
public enum VersionInfo {
    public static let buildTime = "${BUILD_TIME}"
    public static let gitCommit = "${GIT_COMMIT}"
    public static let gitBranch = "${GIT_BRANCH}"
    public static let versionNumber = "${VERSION_NUMBER}"
    public static let versionString = "${VERSION_STRING}"
}
EOF
