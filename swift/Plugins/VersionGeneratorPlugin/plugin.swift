import Foundation
import PackagePlugin

@main
struct VersionGeneratorPlugin: BuildToolPlugin {
    func createBuildCommands(context: PluginContext, target: Target) throws -> [Command] {
        let outputDir = context.pluginWorkDirectoryURL.appending(path: "GeneratedSources")
        let versionFileURL = context.package.directoryURL.appending(path: "VERSION")
        let version = try String(contentsOf: versionFileURL, encoding: .utf8)
            .trimmingCharacters(in: .whitespacesAndNewlines)
        let scriptURL = context.package.directoryURL
            .appending(path: "Plugins/VersionGeneratorPlugin/generate_version.sh")

        return [
            .prebuildCommand(
                displayName: "Generate version info",
                executable: URL(filePath: "/bin/sh"),
                arguments: [scriptURL.path(), outputDir.path(), version],
                outputFilesDirectory: outputDir
            ),
        ]
    }
}
