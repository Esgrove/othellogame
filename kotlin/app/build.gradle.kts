/*
 * This file was generated by the Gradle 'init' task.
 *
 * This generated file contains a sample Kotlin application project to get you started.
 * For more details take a look at the 'Building Java & JVM projects' chapter in the Gradle
 * User Manual available at https://docs.gradle.org/8.1/userguide/building_java_projects.html
 * This project uses @Incubating APIs which are subject to change.
 */

import java.io.ByteArrayOutputStream

version = "1.4.0"

plugins {
    // Apply the org.jetbrains.kotlin.jvm Plugin to add support for Kotlin.
    id("org.jetbrains.kotlin.jvm") version "1.9.23"
    // https://github.com/johnrengelman/shadow
    id("com.github.johnrengelman.shadow") version "8.1.1"

    // Apply the application plugin to add support for building a CLI application in Java.
    application
}

repositories {
    // Use Maven Central for resolving dependencies.
    mavenCentral()
}

dependencies {
    implementation(kotlin("stdlib"))
    implementation("com.google.guava:guava:33.1.0-jre")
    implementation("com.github.ajalt.clikt:clikt:4.4.0")
    implementation("com.squareup.okio:okio:3.9.0")
}

sourceSets {
    main {
        resources {
            srcDir("${layout.buildDirectory.get().asFile}/generated-resources")
        }
    }
}

testing {
    suites {
        // Configure the built-in test suite
        val test by getting(JvmTestSuite::class) {
            // Use Kotlin Test test framework
            useKotlinTest("1.8.10")
        }
    }
}

// Apply a specific Java toolchain to ease working on different environments.
java {
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(17))
    }
}

application {
    // Define the main class for the application.
    mainClass.set("othello.MainKt")
}

tasks.jar {
    manifest {
        attributes["Main-Class"] = application.mainClass
    }
}

tasks.register("generateBuildInfo") {
    doLast {
        val propsFile =
            file("${layout.buildDirectory.get().asFile}/generated-resources/build-info.properties")
        propsFile.parentFile.mkdirs()

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

        logger.lifecycle("Writing build version information to: ${propsFile.absolutePath}")

        propsFile.writeText(
            """
            build.branch=$gitBranch
            build.commit=$gitCommit
            build.date=$buildDate
            build.version=$projectVersion
            """.trimIndent(),
        )
    }
}

// Make the processResources task depend on your new task
// to ensure the properties file is generated before the resources are processed.
tasks.named("processResources") {
    dependsOn("generateBuildInfo")
}

tasks.named("processResources", Copy::class) {
    duplicatesStrategy = DuplicatesStrategy.INCLUDE // INCLUDE, EXCLUDE, FAIL, WARN
}
