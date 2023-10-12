# https://github.com/qqqwwqeeqqwe/nicotine.cc
# libs
import os
import time
from sys import exc_info

# connect other project files
from modules.logger import init, log
from modules.dll import get_dll
from modules.injector import get_pid, inject

init()

os.system("title nicotine.cc / https://github.com/qqqwwqeeqqwe/nicotine.cc")


def cli():
    # let's check for dll first
    # if it's not here the script will download the latest build
    if not os.path.exists("nicotine-release.dll"):
        print(log("~", "nicotine dll was not found, downloading..."))
        get_dll()
    else:
        pass
    pid = get_pid("csgo.exe")
    if pid != None:
        inject(pid, "nicotine-release.dll")
    else:
        print(log("!", "csgo process was not found."))
        time.sleep(5)
        os._exit(1)


if __name__ == "__main__":
    try:
        cli()
    except KeyboardInterrupt:
        os._exit(0)
    except:
        print(log("!", f"during execution of script following error occured: {exc_info()}"))
        input(log("~", "press enter to exit."))
        os._exit(1)
