import pytest

from othello.utils import clamp


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
