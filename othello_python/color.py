"""
Color print helper functions
Akseli Lukkarila
2019
"""
import colorama

class Color():
    red     = colorama.Fore.RED
    green   = colorama.Fore.GREEN
    yellow  = colorama.Fore.YELLOW
    blue    = colorama.Fore.BLUE
    magenta = colorama.Fore.MAGENTA
    cyan    = colorama.Fore.CYAN
    white   = colorama.Fore.WHITE

# ==================================================================================

def printBold(text, color = colorama.Fore.WHITE):
    print(colorama.Style.BRIGHT + color + text + colorama.Style.RESET_ALL)

# ==================================================================================

def printColor(text, color = colorama.Fore.WHITE):
    print(color + text + colorama.Style.RESET_ALL)

# ==================================================================================

def printError(text):
    print("{}: {}".format(getColor("Error", Color.red), text))

# ==================================================================================

def getColor(text, color = colorama.Fore.WHITE):
    return color + text + colorama.Style.RESET_ALL

# ==================================================================================

def getBold(text, color = colorama.Fore.WHITE):
    return colorama.Style.BRIGHT + color + text + colorama.Style.RESET_ALL

