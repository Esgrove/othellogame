import pytest

from othello.othello import Othello


@pytest.fixture
def othello():
    return Othello()
