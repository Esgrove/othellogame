"""
Colorprint
Interface for coloured terminal printing
Akseli Lukkarila
2019-2026
"""

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


def get_color(text: str, color: str = "", bold: bool = False) -> str:
    """Format string with colour using Colorama."""
    if bold and color:
        # Combine bold and colour into a single ANSI escape sequence
        # to match the output of the Rust `colored` crate
        return f"\033[1;{color[2:]}{text}{colorama.Style.RESET_ALL}"
    if bold or color:
        return f"{colorama.Style.BRIGHT if bold else color}{text}{colorama.Style.RESET_ALL}"
    return text


def print_color(text: str, color: str = "", bold: bool = False, **kwargs):
    """
    Print text with specified colour using Colorama.

    You can pass optional extra arguments for the Python default print function if wanted:

    - sep='separator': Specify how to separate the objects, if there is more than one.
        Default is ' '
    - end='end': Specify what to print at the end. Default is '\\n' (line feed)
    - file: An object with a write method. Default is sys.stdout
    - flush: A Boolean, specifying if the output is flushed (True) or buffered (False).
        Default is False
    """
    print(get_color(text, color, bold), **kwargs)


def print_bold(text: str):
    """Print bold text."""
    print_color(text, bold=True)


def print_green(text: str):
    """Print text in green."""
    print_color(text, Color.green)


def print_green_bold(text: str):
    """Print bold text in green."""
    print_color(text, Color.green, bold=True)


def print_yellow(text: str):
    """Print text in yellow."""
    print_color(text, Color.yellow)


def print_yellow_bold(text: str):
    """Print bold text in yellow."""
    print_color(text, Color.yellow, bold=True)


def print_red(text: str):
    """Print text in red."""
    print_color(text, Color.red)


def print_red_bold(text: str):
    """Print bold text in red."""
    print_color(text, Color.red, bold=True)


def print_error(message: str, **kwargs):
    """Print error message with red colour."""
    indent, text = _split_leading_whitespace(message)
    print_color(f"{indent}Error: {text}", Color.red, **kwargs)


def print_warn(message: str, **kwargs):
    """Print warning message with yellow colour."""
    indent, text = _split_leading_whitespace(message)
    print_color(f"{indent}Warning: {text}", Color.yellow, **kwargs)


def _split_leading_whitespace(message: str) -> tuple[str, str]:
    """Split a string into the leading whitespace and the rest of the string."""
    indent_size = next((i for i, c in enumerate(message) if not c.isspace()), len(message))
    indent = message[:indent_size]
    text = message[indent_size:]
    return indent, text
