// swift-tools-version:6.2
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "Othello",
    platforms: [
        // Minimum macOS version
        .macOS(.v14),
    ],
    dependencies: [
        .package(url: "https://github.com/mtynior/ColorizeSwift.git", from: "1.6.0"),
        .package(url: "https://github.com/apple/swift-argument-parser", from: "1.3.0"),
        .package(url: "https://github.com/apple/swift-crypto.git", from: "3.2.0"),
    ],
    targets: [
        .target(
            name: "VersionInfo"
        ),
        .executableTarget(
            name: "othello_swift",
            dependencies: [
                .target(name: "VersionInfo"),
                .product(name: "ColorizeSwift", package: "ColorizeSwift"),
                .product(name: "ArgumentParser", package: "swift-argument-parser"),
                .product(name: "Crypto", package: "swift-crypto"),
            ],
            path: "Sources/Othello"
        ),
    ]
)
