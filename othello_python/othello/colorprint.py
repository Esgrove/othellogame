"""
Colorprint
Color print helper
Akseli Lukkarila
2019-2023
"""
import sys

import colorama

colorama.init()


class Color:
    """Interface to Colorama colors."""

    blue = colorama.Fore.BLUE
    cyan = colorama.Fore.CYAN
    green = colorama.Fore.GREEN
    magenta = colorama.Fore.MAGENTA
    red = colorama.Fore.RED
    white = colorama.Fore.WHITE
    yellow = colorama.Fore.YELLOW


def get_color(text: str, color=Color.white, bold=False) -> str:
    """Format string with colour using Colorama."""
    return f"{colorama.Style.BRIGHT if bold else ''}{color}{text}{colorama.Style.RESET_ALL}"


def print_bold(text: str, color=Color.white, **kwargs):
    """Print bold text."""
    print(get_color(text, color, True), **kwargs)


def print_color(text: str, color=Color.white, bold=False, **kwargs):
    """
    Print text with colour using Colorama.

    You can pass optional extra arguments for the Python default print function if wanted:

    - sep='separator': Specify how to separate the objects, if there is more than one.
        Default is ' '
    - end='end': Specify what to print at the end. Default is '\\n' (line feed)
    - file: An object with a write method. Default is sys.stdout
    - flush: A Boolean, specifying if the output is flushed (True) or buffered (False).
        Default is False
    """
    print(get_color(text, color, bold), **kwargs)


def print_error_and_exit(message: str, exit_code=1):
    """Print error message and exit with given exit code."""
    print_error(message)
    sys.exit(exit_code)


def print_error(message: str, bold=False, **kwargs):
    """Print error message with red colour."""
    indent, text = _split_leading_whitespace(message)
    print_color(f"{indent}ERROR: {text}", Color.red, bold, **kwargs)


def print_warn(message: str, bold=False, **kwargs):
    """Print warning message with yellow colour."""
    indent, text = _split_leading_whitespace(message)
    print_color(f"{indent}WARNING: {text}", Color.yellow, bold, **kwargs)


def print_green(text: str, bold=False, **kwargs):
    print_color(text, Color.green, bold, **kwargs)


def print_yellow(text: str, bold=False, **kwargs):
    print_color(text, Color.yellow, bold, **kwargs)


def print_red(text: str, bold=False, **kwargs):
    print_color(text, Color.red, bold, **kwargs)


def print_blue(text: str, bold=False, **kwargs):
    print_color(text, Color.red, bold, **kwargs)


def print_magenta(text: str, bold=False, **kwargs):
    print_color(text, Color.magenta, bold, **kwargs)


def print_cyan(text: str, bold=False, **kwargs):
    print_color(text, Color.cyan, bold, **kwargs)


def _split_leading_whitespace(message: str) -> (str, str):
    """Split a string to the leading whitespace and rest of the string."""
    indent_size = next((i for i, c in enumerate(message) if not c.isspace()), len(message))
    indent = message[:indent_size]
    text = message[indent_size:]
    return indent, text
