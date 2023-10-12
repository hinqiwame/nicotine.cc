# https://github.com/qqqwwqeeqqwe/nicotine.cc
# libs
import psutil
import pyinjector
import time
import os
from sys import exc_info

# connecting other project files
from .logger import log


def get_pid(process: str):
    """func to get pid by process name."""
    for proc in psutil.process_iter(["pid", "name"]):
        if proc.info["name"] == process:
            return proc.info["pid"]
    return None


def inject(pid: int, dll: str):
    """func to inject dll into process i guess."""
    try:
        print(log("~", f"injecting {dll} into {pid}..."))
        pyinjector.inject(pid, dll)
        print(log("+","injected! have fun <3"))
        input(log("~", "press enter to exit."))
        os._exit(1)
    except pyinjector.api.LibraryNotFoundException:
        print(log("!", f"{dll} was not found."))
        time.sleep(5)
        os._exit(1)
    except:
        print(log("!", f"during injection following exception occured: {exc_info()}"))
        input(log("~", "press enter to exit."))
        os._exit(1)
