package othello

import okio.Buffer
import okio.HashingSink
import okio.buffer

/** Calculate SHA256 hash for the given string. */
fun calculateSha256(input: String): String {
    val buffer = Buffer()
    val hashingSink = HashingSink.sha256(buffer)
    val bufferedSink = hashingSink.buffer()
    bufferedSink.use { sink -> sink.writeUtf8(input) }
    return hashingSink.hash.hex()
}
