import java.io.ByteArrayOutputStream

version = "2.0.0"

plugins {
    kotlin("multiplatform") version "2.1.0"
}

repositories {
    mavenCentral()
}

kotlin {
    macosX64("macos") {
        binaries {
            executable {
                entryPoint = "othello.MainKt"
            }
        }
    }
    linuxX64("linux") {
        binaries {
            executable {
                entryPoint = "othello.MainKt"
            }
        }
    }
    mingwX64("windows") {
        binaries {
            executable {
                entryPoint = "othello.MainKt"
            }
        }
    }

    sourceSets {
        val commonMain by getting {
            kotlin.srcDirs("app/src/main/kotlin")
            kotlin.srcDir("${layout.buildDirectory.get().asFile}/generated-sources/build-info")
            dependencies {
                implementation("com.github.ajalt.clikt:clikt:5.0.2")
                implementation("com.squareup.okio:okio:3.9.1")
            }
        }
        val commonTest by getting {
            kotlin.srcDirs("app/src/test/kotlin")
            dependencies {
                implementation(kotlin("test"))
            }
        }

        val macosMain by getting
        val macosTest by getting

        val linuxMain by getting
        val linuxTest by getting

        val windowsMain by getting
        val windowsTest by getting
    }
}

tasks {
    val generateBuildInfo by registering {
        val outputDir = layout.buildDirectory.dir("generated-sources/build-info").get().asFile
        val buildInfoFile = file("$outputDir/BuildInfo.kt")

        doLast {
            outputDir.mkdirs()

            val gitBranch: String = ByteArrayOutputStream().use { outputStream ->
                exec {
                    commandLine("git", "branch", "--show-current")
                    standardOutput = outputStream
                }
                outputStream.toString("UTF-8").trim()
            }

            val gitCommit: String = ByteArrayOutputStream().use { outputStream ->
                exec {
                    commandLine("git", "rev-parse", "--short", "HEAD")
                    standardOutput = outputStream
                }
                outputStream.toString("UTF-8").trim()
            }

            val buildDate: String = ByteArrayOutputStream().use { outputStream ->
                exec {
                    commandLine("date", "+%Y-%m-%d_%H%M")
                    standardOutput = outputStream
                }
                outputStream.toString("UTF-8").trim()
            }

            val projectVersion: String = version.toString()

            buildInfoFile.writeText(
                """
                object BuildInfo {
                    const val branch: String = "$gitBranch"
                    const val commit: String = "$gitCommit"
                    const val date: String = "$buildDate"
                    const val version: String = "$projectVersion"
                }
                """.trimIndent(),
            )
        }
    }

    named("compileKotlinLinux") {
        dependsOn(generateBuildInfo)
    }
    named("compileKotlinMacos") {
        dependsOn(generateBuildInfo)
    }
    named("compileKotlinWindows") {
        dependsOn(generateBuildInfo)
    }
}
