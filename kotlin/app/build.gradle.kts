import java.io.ByteArrayOutputStream

version = "1.7.1"

plugins {
    id("org.jetbrains.kotlin.jvm") version "2.1.21"
    // https://github.com/GradleUp/shadow
    id("com.gradleup.shadow") version "8.3.6"

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
    implementation("com.squareup.okio:okio:3.11.0")
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

tasks.register("generateBuildInfo") {
    doLast {
        val buildDir = "${layout.buildDirectory.get().asFile}"
        val propsFile = file("$buildDir/generated-resources/build-info.properties")
        propsFile.parentFile.mkdirs()

        fun executeCommand(vararg command: String): String {
            val outputStream = ByteArrayOutputStream()
            project.exec {
                commandLine(*command)
                standardOutput = outputStream
            }
            return outputStream.toString("UTF-8").trim()
        }

        val gitBranch = executeCommand("git", "branch", "--show-current")
        val gitCommit = executeCommand("git", "rev-parse", "--short", "HEAD")
        val buildTime = executeCommand("date", "-u", "+%Y-%m-%d_%H%M")
        val projectVersion = version.toString()

        logger.lifecycle("Writing build version information to: ${propsFile.absolutePath}")

        propsFile.writeText(
            """
            build.branch=$gitBranch
            build.commit=$gitCommit
            build.time=$buildTime
            build.version=$projectVersion
            """.trimIndent(),
        )
    }
}

tasks.named("processResources") {
    dependsOn("generateBuildInfo")
}

tasks.named<Copy>("processResources") {
    duplicatesStrategy = DuplicatesStrategy.INCLUDE // Options: INCLUDE, EXCLUDE, FAIL, WARN
}
