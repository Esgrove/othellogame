package othello

import java.util.*

object BuildInfo {
    private val properties: Properties = Properties().apply {
        val propFileName = "build-info.properties"
        val classLoader = javaClass.classLoader ?: ClassLoader.getSystemClassLoader()
        classLoader.getResourceAsStream(propFileName)?.use {
            load(it)
        } ?: Properties()
    }

    val branch: String = properties.getProperty("build.branch", "unknown")
    val commit: String = properties.getProperty("build.commit", "unknown")
    val date: String = properties.getProperty("build.time", "unknown")
    val version: String = properties.getProperty("build.version", "unknown")
}

fun versionInfo(): String =
    "${BuildInfo.version} ${BuildInfo.date} ${BuildInfo.branch} ${BuildInfo.commit}"
