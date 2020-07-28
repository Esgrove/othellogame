"""
Color print helper
Akseli Lukkarila
2019
"""
import colorama
colorama.init()


class Color:
    """Nicer interface to Colorama colors."""
    blue = colorama.Fore.BLUE
    cyan = colorama.Fore.CYAN
    green = colorama.Fore.GREEN
    magenta = colorama.Fore.MAGENTA
    red = colorama.Fore.RED
    white = colorama.Fore.WHITE
    yellow = colorama.Fore.YELLOW


def get_color(text, color=Color.white, bold=False):
    return f"{colorama.Style.BRIGHT if bold else ''}{color}{text}{colorama.Style.RESET_ALL}"


def print_bold(text, color=Color.white):
    print(get_color(text, color, True))


def print_color(text, color=Color.white, bold=False):
    print(get_color(text, color, bold))


def print_error(text):
    print(f"{get_color('Error:', Color.red)} {text}")
