/*
 * This file was generated by the Gradle 'init' task.
 *
 * This generated file contains a sample Kotlin application project to get you started.
 * For more details take a look at the 'Building Java & JVM projects' chapter in the Gradle
 * User Manual available at https://docs.gradle.org/8.1/userguide/building_java_projects.html
 * This project uses @Incubating APIs which are subject to change.
 */

import java.io.ByteArrayOutputStream

version = "1.0.0"

plugins {
    // Apply the org.jetbrains.kotlin.jvm Plugin to add support for Kotlin.
    id("org.jetbrains.kotlin.jvm") version "1.8.10"

    // Apply the application plugin to add support for building a CLI application in Java.
    application
}

repositories {
    // Use Maven Central for resolving dependencies.
    mavenCentral()
}

dependencies {
    implementation(kotlin("stdlib"))
    implementation("com.google.guava:guava:31.1-jre")
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
    mainClass.set("othello_kotlin.OthelloKt")
}

tasks.jar {
    manifest {
        attributes["Main-Class"] = application.mainClass
    }
}

// https://github.com/Baeldung/kotlin-tutorials/blob/master/kotlin-self-executable-jar/kotlin-executable-jar/build.gradle.kts
tasks {
    val fatJar = register<Jar>("fatJar") {
        // We need this for Gradle optimization to work
        dependsOn.addAll(listOf("compileJava", "compileKotlin", "processResources"))
        // Naming the jar
        archiveClassifier.set("othello-kotlin")
        duplicatesStrategy = DuplicatesStrategy.EXCLUDE
        // Provided we set it up in the application plugin configuration
        manifest { attributes(mapOf("Main-Class" to application.mainClass)) }
        val sourcesMain = sourceSets.main.get()
        val contents = configurations.runtimeClasspath.get()
            .map { if (it.isDirectory) it else zipTree(it) } +
                sourcesMain.output
        from(contents)
    }
    build {
        // Trigger fat jar creation during build
        dependsOn(fatJar)
    }
}

tasks.register("generateBuildInfo") {
    doLast {
        val propsFile = file("${layout.buildDirectory.get().asFile}/resources/main/build-info.properties")
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

        propsFile.writeText("""
            build.branch=$gitBranch
            build.commit=$gitCommit
            build.date=$buildDate
            build.version=$projectVersion
        """.trimIndent())
    }
}

tasks.named("processResources") {
    dependsOn("generateBuildInfo")
}
