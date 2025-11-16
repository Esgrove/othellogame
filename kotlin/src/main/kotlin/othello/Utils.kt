package othello

import okio.Buffer
import okio.HashingSink
import okio.buffer

/**
 * Calculate the SHA-256 hash for the given string.
 *
 * @param input The input string to hash.
 * @return The SHA-256 hash as a hexadecimal string.
 */
fun calculateSha256(input: String): String {
    val buffer = Buffer()
    val hashingSink = HashingSink.sha256(buffer)
    val bufferedSink = hashingSink.buffer()
    bufferedSink.use { sink -> sink.writeUtf8(input) }
    return hashingSink.hash.hex()
}
