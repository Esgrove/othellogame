import pytest

from othello import Othello


@pytest.fixture
def othello():
    return Othello()
