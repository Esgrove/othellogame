// swift-tools-version:5.8
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "othello_swift",
    platforms: [
        // Minimum macOS version
        .macOS(.v12),
    ],
    dependencies: [
        .package(url: "https://github.com/mtynior/ColorizeSwift.git", from: "1.6.0"),
        .package(url: "https://github.com/apple/swift-argument-parser", from: "1.3.0"),
        .package(url: "https://github.com/apple/swift-crypto.git", from: "3.2.0"),
    ],
    targets: [
        .target(
            name: "BuildInfo",
            publicHeadersPath: "include"
        ),
        .executableTarget(
            name: "othello_swift",
            dependencies: [
                .target(name: "BuildInfo"),
                .product(name: "ColorizeSwift", package: "ColorizeSwift"),
                .product(name: "ArgumentParser", package: "swift-argument-parser"),
                .product(name: "Crypto", package: "swift-crypto"),
            ]
        ),
    ]
)
