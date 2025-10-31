import java.io.ByteArrayOutputStream
import javax.inject.Inject
import org.gradle.api.DefaultTask
import org.gradle.api.file.DirectoryProperty
import org.gradle.api.provider.Property
import org.gradle.api.services.*
import org.gradle.api.tasks.*

abstract class ExecHelper @Inject constructor(val execOps: ExecOperations) :
    BuildService<BuildServiceParameters.None>

abstract class WriteVersionTask : DefaultTask() {
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

        val file = outputDir.get().file("othello/VersionInfo.java").asFile
        file.parentFile.mkdirs()
        file.writeText(
            """
            package othello;

            public final class VersionInfo {
                public static final String APP_NAME = "${appName.get()}";
                public static final String BUILD_TIME = "$buildTime";
                public static final String GIT_BRANCH = "$gitBranch";
                public static final String GIT_COMMIT = "$gitCommit";
                public static final String VERSION_NUMBER = "${appVersion.get()}";
                public static final String VERSION_STRING = "$formattedVersion";
            }
            """.trimIndent()
        )
    }
}

version = "1.0.0"

plugins {
    // Apply the application plugin to add support for building a CLI application in Java.
    application
    java

    // https://plugins.gradle.org/plugin/com.diffplug.gradle.spotless
    id("com.diffplug.spotless") version "8.0.0"
}

repositories {
    // Use Maven Central for resolving dependencies.
    mavenCentral()
}

dependencies {
    // https://mvnrepository.com/artifact/org.junit/junit-bom
    testImplementation(platform("org.junit:junit-bom:6.0.1"))
    // https://mvnrepository.com/artifact/org.junit.jupiter/junit-jupiter
    testImplementation("org.junit.jupiter:junit-jupiter")

    // https://mvnrepository.com/artifact/com.google.guava/guava
    implementation("com.google.guava:guava:33.5.0-jre")

    // https://mvnrepository.com/artifact/info.picocli/picocli
    implementation("info.picocli:picocli:4.7.7")
}

// Apply a specific Java toolchain to ease working on different environments.
java {
    toolchain {
        languageVersion = JavaLanguageVersion.of(21)
    }
}

application {
    mainClass.set("othello.Main")
}

val execHelper = gradle.sharedServices.registerIfAbsent("execHelper", ExecHelper::class) {}

val writeVersionFile by tasks.registering(WriteVersionTask::class) {
    outputDir.set(layout.buildDirectory.dir("generated/sources/versioninfo/java"))
    appName.set(project.name)
    appVersion.set(project.version.toString())
    execOps.set(execHelper.map { it.execOps })
}

sourceSets["main"].java {
    srcDir("build/generated/sources/versioninfo/java")
}

tasks.named("compileJava") {
    dependsOn(writeVersionFile)
}

tasks.register<JavaExec>("othello") {
    group = "application"
    mainClass.set("othello.Main")
    classpath = sourceSets["main"].runtimeClasspath
    standardInput = System.`in`

    javaLauncher.set(javaToolchains.launcherFor {
        languageVersion.set(JavaLanguageVersion.of(21))
    })
}

tasks.named<ProcessResources>("processResources") {
    dependsOn(writeVersionFile)
}

tasks.test {
    useJUnitPlatform()
}

spotless {
    ratchetFrom("origin/main")
    java {
        target("src/**/*.java")
        eclipse().configFile("eclipse-format.xml")
        formatAnnotations()
    }
}

tasks.register("format") {
    group = "formatting"
    description = "Runs Spotless Apply to format code"
    dependsOn("spotlessApply")
}

tasks.register("version") {
    dependsOn(writeVersionFile)
    val versionFile = layout.buildDirectory.file("generated/sources/versioninfo/java/othello/VersionInfo.java")
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

tasks.register<Jar>("fatJar") {
    group = "build"
    archiveClassifier.set("all")

    manifest {
        attributes["Main-Class"] = "othello.Main"
    }

    from(sourceSets.main.get().output)
    duplicatesStrategy = DuplicatesStrategy.EXCLUDE

    dependsOn(configurations.runtimeClasspath)
    from({
        configurations.runtimeClasspath.get()
            .filter { it.name.endsWith("jar") }
            .map { zipTree(it) }
    })
}
