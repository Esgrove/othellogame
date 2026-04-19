// swift-tools-version:6.2
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "Othello",
    platforms: [
        // Minimum macOS version
        .macOS(.v15)
    ],
    dependencies: [
        .package(url: "https://github.com/mtynior/ColorizeSwift.git", from: "1.6.0"),
        .package(url: "https://github.com/apple/swift-argument-parser", from: "1.3.0"),
        .package(url: "https://github.com/apple/swift-crypto.git", from: "4.2.0"),
    ],
    targets: [
        .plugin(
            name: "VersionGeneratorPlugin",
            capability: .buildTool()
        ),
        .target(
            name: "VersionInfo",
            plugins: [.plugin(name: "VersionGeneratorPlugin")]
        ),
        .target(
            name: "OthelloLib",
            dependencies: [
                .target(name: "VersionInfo"),
                .product(name: "ColorizeSwift", package: "ColorizeSwift"),
                .product(name: "ArgumentParser", package: "swift-argument-parser"),
                .product(name: "Crypto", package: "swift-crypto"),
            ],
            path: "Sources/OthelloLib",
            sources: [
                "board.swift",
                "colorprint.swift",
                "models.swift",
                "othello.swift",
                "player.swift",
                "settings.swift",
                "utils.swift",
                "version.swift",
            ]
        ),
        .executableTarget(
            name: "othello_swift",
            dependencies: [
                .target(name: "OthelloLib"),
                .product(name: "ColorizeSwift", package: "ColorizeSwift"),
                .product(name: "ArgumentParser", package: "swift-argument-parser"),
            ],
            path: "Sources/Othello",
            exclude: [
                "board.swift",
                "colorprint.swift",
                "models.swift",
                "othello.swift",
                "player.swift",
                "settings.swift",
                "utils.swift",
                "version.swift",
            ],
            sources: ["main.swift"]
        ),
        .testTarget(
            name: "OthelloTests",
            dependencies: [
                .target(name: "OthelloLib")
            ]
        ),
    ]
)
