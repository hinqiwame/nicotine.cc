# https://github.com/qqqwwqeeqqwe/nicotine.cc
# libs
from colorama import Fore, init
from datetime import datetime

init = init


def log(prefix: str, text: str):
    """func defining a log message that will be displayed in terminal, both print and input."""
    return f"[{Fore.LIGHTBLACK_EX}{prefix}{Fore.RESET}] {Fore.LIGHTBLACK_EX}{datetime.now().strftime('%H:%M:%S')}{Fore.RESET} | {text}"


# print(log("~", "test"))
