import pytest

from othello import Othello
from utils import Disk


@pytest.fixture
def othello():
    return Othello()
