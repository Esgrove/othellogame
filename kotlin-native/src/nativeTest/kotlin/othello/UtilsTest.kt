package othello

import kotlin.test.Test
import kotlin.test.assertEquals

class UtilsTest {
    @Test
    fun testCalculateSha256Simple() {
        val testString = "test"
        val hash = calculateSha256(testString)
        assertEquals(
            "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08",
            hash,
        )
    }

    @Test
    fun testCalculateSha256LogLine() {
        val testString = "01: B:(0,1),1;____BBB__BW_____"
        val hash = calculateSha256(testString)
        assertEquals(
            "d087cd890b81b7153cc870fba7372396e91cdcd4b6d13b82bbc88b5a4eb889d1",
            hash,
        )
    }

    @Test
    fun testCalculateSha256FullLog() {
        val testLog = buildString {
            append("01: B:(0,1),1;____BBB__BW_____\n")
            append("02: W:(0,0),1;W___BWB__BW_____\n")
            append("03: B:(1,0),1;WB__BBB__BW_____\n")
            append("04: W:(0,2),2;WB__WBB_WWW_____\n")
            append("05: B:(0,3),1;WB__WBB_WBW_B___\n")
            append("06: W:(2,0),3;WWW_WWW_WBW_B___\n")
            append("07: B:(3,0),1;WWWBWWB_WBW_B___\n")
            append("08: W:(1,3),1;WWWBWWB_WWW_BW__\n")
            append("09: B:(2,3),2;WWWBWWB_WWB_BBB_\n")
            append("10: W:(3,2),2;WWWBWWW_WWWWBBB_\n")
            append("11: B:(3,1),1;WWWBWWWBWWBWBBB_\n")
            append("12: W:(3,3),1;WWWBWWWBWWWWBBBW")
        }

        val hash = calculateSha256(testLog)
        assertEquals(
            "ad4679949110ed7997aa1bf09441a7eb491b498189c03685ffdd6ddbb70e3c22",
            hash,
        )
    }
}
