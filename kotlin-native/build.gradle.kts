import java.io.ByteArrayOutputStream
import org.jetbrains.kotlin.gradle.targets.native.tasks.KotlinNativeTest

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

version = "2.0.0"

plugins {
    alias(libs.plugins.kotlinMultiplatform)
    alias(libs.plugins.kotlinxSerialization)

    // https://plugins.gradle.org/plugin/com.diffplug.gradle.spotless
    id("com.diffplug.spotless") version "8.0.0"
}

repositories {
    mavenCentral()
}

kotlin {
    val hostOs = System.getProperty("os.name")
    val isArm64 = System.getProperty("os.arch") == "aarch64"
    val isMingwX64 = hostOs.startsWith("Windows")
    val nativeTarget = when {
        hostOs == "Mac OS X" && isArm64 -> macosArm64("native")
        hostOs == "Mac OS X" && !isArm64 -> macosX64("native")
        hostOs == "Linux" && isArm64 -> linuxArm64("native")
        hostOs == "Linux" && !isArm64 -> linuxX64("native")
        isMingwX64 -> mingwX64("native")
        else -> throw GradleException("Host OS is not supported in Kotlin/Native.")
    }

    nativeTarget.apply {
        binaries {
            executable {
                entryPoint = "othello.main"
            }
        }
    }

    sourceSets {
        val commonMain by getting
        val commonTest by getting

        val nativeMain by getting {
            kotlin.srcDir("build/generated/sources/versioninfo/kotlin")
            dependsOn(commonMain)
            dependencies {
                implementation(libs.kotlinxSerializationJson)
                // https://github.com/ajalt/clikt
                implementation("com.github.ajalt.clikt:clikt:5.0.3")
                // https://github.com/square/okio
                implementation("com.squareup.okio:okio:3.16.2")
            }
        }

        val nativeTest by getting {
            dependsOn(commonTest)
            dependencies {
                implementation(kotlin("test"))
            }
        }
    }
}

tasks.withType<org.jetbrains.kotlin.gradle.tasks.KotlinNativeCompile>().configureEach {
    dependsOn(writeVersionFile)
}

spotless {
    ratchetFrom("origin/main")
    kotlin {
        ktlint("1.8.0").setEditorConfigPath("$projectDir/.editorconfig")
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

tasks.withType<KotlinNativeTest>().configureEach {
    testLogging {
        events("PASSED", "FAILED", "SKIPPED")
        showStandardStreams = true
        exceptionFormat = org.gradle.api.tasks.testing.logging.TestExceptionFormat.SHORT
        showExceptions = true
        showCauses = true
        showStackTraces = false
    }
}
