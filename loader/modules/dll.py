# https://github.com/qqqwwqeeqqwe/nicotine.cc
# libs
import requests
import time
import os

# connect other project files
from .logger import log


def get_dll():
    """func to download the latest nicotine dll."""
    response = requests.get("https://github.com/qqqwwqeeqqwe/nicotine.cc/raw/main/cheat/build/nicotine-release.dll")

    if response.status_code == 200:  # if the request was successful
        with open("nicotine-release.dll", "wb") as dll:
            dll.write(response.content)
        print(log("+", "dll downloaded"))
    else:
        print(log("!", "unable to download dll from github..."))
        time.sleep(5)
        os._exit(0)
