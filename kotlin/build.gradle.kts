import org.gradle.api.DefaultTask
import org.gradle.api.file.DirectoryProperty
import org.gradle.api.model.ObjectFactory
import org.gradle.api.provider.Property
import org.gradle.api.services.BuildService
import org.gradle.api.services.BuildServiceParameters
import org.gradle.api.tasks.*
import org.gradle.process.ExecOperations
import java.io.ByteArrayOutputStream
import javax.inject.Inject

abstract class ExecHelper @Inject constructor(val execOps: ExecOperations) :
    BuildService<BuildServiceParameters.None>

abstract class WriteVersionTask @Inject constructor(private val objects: ObjectFactory) :
    DefaultTask() {

    @get:OutputDirectory
    abstract val outputDir: DirectoryProperty

    @get:Input
    abstract val appName: Property<String>

    @get:Input
    abstract val appVersion: Property<String>

    @get:Internal
    abstract val execOps: Property<ExecOperations>

    @TaskAction
    fun generate() {
        fun execAndCapture(vararg args: String): String {
            val out = ByteArrayOutputStream()
            execOps.get().exec {
                commandLine(*args)
                standardOutput = out
                isIgnoreExitValue = true
            }
            return out.toString().trim().ifBlank { "dev" }
        }

        val gitBranch = execAndCapture("git", "rev-parse", "--abbrev-ref", "HEAD")
        val gitCommit = execAndCapture("git", "rev-parse", "--short", "HEAD")
        val buildTime = execAndCapture("date", "-u", "+%Y-%m-%d_%H%M")
        val formattedVersion = "${appName.get()} ${appVersion.get()} $buildTime $gitBranch $gitCommit"

        val file = outputDir.get().file("othello/VersionInfo.kt").asFile
        file.parentFile.mkdirs()
        file.writeText(
            """
            package othello

            object VersionInfo {
                const val APP_NAME = "${appName.get()}"
                const val BUILD_TIME = "$buildTime"
                const val GIT_BRANCH = "$gitBranch"
                const val GIT_COMMIT = "$gitCommit"
                const val VERSION_NUMBER = "${appVersion.get()}"
                const val VERSION_STRING = "$formattedVersion"
            }
            """.trimIndent(),
        )
    }
}

val execHelper = gradle.sharedServices.registerIfAbsent("execHelper", ExecHelper::class) {}

val writeVersionFile by tasks.registering(WriteVersionTask::class) {
    outputDir.set(layout.buildDirectory.dir("generated/sources/versioninfo/kotlin"))
    appName.set(project.name)
    appVersion.set(project.version.toString())
    execOps.set(execHelper.map { it.execOps })
}

version = "1.7.2"

plugins {
    id("org.jetbrains.kotlin.jvm") version "2.2.0"
    // https://github.com/GradleUp/shadow
    id("com.gradleup.shadow") version "8.3.8"

    // https://plugins.gradle.org/plugin/com.diffplug.gradle.spotless
    id("com.diffplug.spotless") version "7.2.0"

    // Apply the application plugin to add support for building a CLI application in Java.
    application
}

repositories {
    mavenCentral()
}

dependencies {
    implementation(kotlin("stdlib"))
    implementation("com.google.guava:guava:33.4.8-jre")
    // https://github.com/ajalt/clikt
    implementation("com.github.ajalt.clikt:clikt:5.0.3")
    // https://github.com/square/okio
    implementation("com.squareup.okio:okio:3.15.0")
}

sourceSets["main"].kotlin {
    srcDir("build/generated/sources/versioninfo/kotlin")
}

tasks.named("compileKotlin") {
    dependsOn(writeVersionFile)
}

testing {
    suites {
        // Configure the built-in test suite
        val test by getting(JvmTestSuite::class) {
            // Use Kotlin Test framework
            useKotlinTest("1.8.10")
        }
    }
}

java {
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(21))
    }
}

application {
    mainClass.set("othello.MainKt")
}

tasks.jar {
    manifest {
        attributes["Main-Class"] = application.mainClass
    }
}

tasks.named<Copy>("processResources") {
    duplicatesStrategy = DuplicatesStrategy.INCLUDE // Options: INCLUDE, EXCLUDE, FAIL, WARN
}

spotless {
    ratchetFrom("origin/main")
    kotlin {
        ktlint("1.6.0").setEditorConfigPath("$projectDir/.editorconfig")
    }
}

tasks.named("spotlessKotlin") {
    dependsOn(writeVersionFile)
}

tasks.register("format") {
    group = "formatting"
    description = "Runs Spotless Apply to format code"
    dependsOn("spotlessApply")
}

tasks.register("version") {
    dependsOn(writeVersionFile)
    val versionFile = layout.buildDirectory.file(
        "generated/sources/versioninfo/kotlin/othello/VersionInfo.kt",
    )
    doLast {
        val file = versionFile.get().asFile
        println("Generated build info to: ${file.absolutePath}")
        val versionLine = file.readLines().find { it.contains("VERSION_STRING") }
        val versionString = versionLine
            ?.substringAfter('=')
            ?.trim()
            ?.removePrefix("\"")
            ?.removeSuffix(";")
            ?.removeSuffix("\"")

        println("Version: ${versionString ?: "info missing"}")
    }
}

tasks.named("build") {
    dependsOn("version")
}
