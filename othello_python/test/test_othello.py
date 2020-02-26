import pytest

from othello import Othello
from util import Disk


@pytest.fixture
def othello():
    return Othello()
