package othello

import okio.Buffer
import okio.HashingSink
import okio.buffer
import okio.use
import platform.posix.usleep

/** Calculate SHA256 hash for the given string. */
fun calculateSha256(input: String): String {
    val buffer = Buffer()
    val hashingSink = HashingSink.sha256(buffer)
    val bufferedSink = hashingSink.buffer()
    bufferedSink.use { sink -> sink.writeUtf8(input) }
    return hashingSink.hash.hex()
}

/** Blocking sleep (milliseconds). */
fun sleep(millis: Int) {
    require(millis >= 0) { "millis must be non-negative" }
    if (millis == 0) return

    // usleep takes microseconds as an UInt.
    // Convert millis -> micros and clamp to the maximum supported value.
    val microsLong = millis * 1000L
    val maxMicros = UInt.MAX_VALUE.toLong()
    val clampedMicros = if (microsLong > maxMicros) maxMicros else microsLong

    usleep(clampedMicros.toUInt())
}
