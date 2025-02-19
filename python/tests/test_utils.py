import pytest

from othello.utils import clamp, calculate_sha256


def test_clamp():
    assert clamp(0, 0, 0) == 0
    assert clamp(0, 0, 1) == 0
    assert clamp(0, 1, 1) == 1
    assert clamp(0, 1, 2) == 1
    assert clamp(0, -1, 0) == 0
    assert clamp(0, -1, 1) == 0
    assert clamp(0, -1, -1) == -1
    assert clamp(0, -2, -1) == -1

    assert clamp(1, 0, 0) == 0
    assert clamp(1, 0, 1) == 1
    assert clamp(1, 0, 2) == 1
    assert clamp(1, -1, 1) == 1
    assert clamp(1, -1, 2) == 1
    assert clamp(1, -1, 0) == 0

    assert clamp(0.0, 0.0, 0.0) == 0.0
    assert clamp(0.0, 0.0, 1.0) == 0.0
    assert clamp(0.0, 1.0, 1.0) == 1.0
    assert clamp(0.0, 1.0, 2.0) == 1.0
    assert clamp(0.0, -1.0, 0.0) == 0.0
    assert clamp(0.0, -1.0, 1.0) == 0.0

    assert clamp(0.5, 0.0, 1.0) == 0.5
    assert clamp(0.5, 0.5, 1.0) == 0.5
    assert clamp(0.5, 0.0, 0.0) == 0.0
    assert clamp(0.5, -1.0, 0.0) == 0.0

    # bigger min value should raise AssertionError
    with pytest.raises(AssertionError):
        assert clamp(0, 1, 0)
        assert clamp(0, 0, -1)
        assert clamp(0, 1, -1)
        assert clamp(0, -1, -2)


def test_calculate_sha256_simple():
    test_string = "test"
    hash_result = calculate_sha256(test_string)
    assert hash_result == "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"


def test_calculate_sha256_log_line():
    test_string = "01: B:(0,1),1;____BBB__BW_____"
    hash_result = calculate_sha256(test_string)
    assert hash_result == "d087cd890b81b7153cc870fba7372396e91cdcd4b6d13b82bbc88b5a4eb889d1"


def test_calculate_sha256_full_log():
    test_log = """\
01: B:(0,1),1;____BBB__BW_____
02: W:(0,0),1;W___BWB__BW_____
03: B:(1,0),1;WB__BBB__BW_____
04: W:(0,2),2;WB__WBB_WWW_____
05: B:(0,3),1;WB__WBB_WBW_B___
06: W:(2,0),3;WWW_WWW_WBW_B___
07: B:(3,0),1;WWWBWWB_WBW_B___
08: W:(1,3),1;WWWBWWB_WWW_BW__
09: B:(2,3),2;WWWBWWB_WWB_BBB_
10: W:(3,2),2;WWWBWWW_WWWWBBB_
11: B:(3,1),1;WWWBWWWBWWBWBBB_
12: W:(3,3),1;WWWBWWWBWWWWBBBW"""
    hash_result = calculate_sha256(test_log)
    assert hash_result == "ad4679949110ed7997aa1bf09441a7eb491b498189c03685ffdd6ddbb70e3c22"
