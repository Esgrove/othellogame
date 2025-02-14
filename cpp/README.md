# Othello CPP

## Usage

```console
A simple Othello CLI game implementation in C++
Usage:
  othello_cpp [OPTIONS] [SIZE]

Arguments:
  [SIZE]            Optional board size (4..10)

 Optional options:
  -a, --autoplay    Enable autoplay mode
  -d, --default     Play with default settings
  -l, --log         Show log after a game
  -n, --no-helpers  Hide disk placement hints
  -t, --test        Enable test mode
  -c, --check       Only print hash to check the result
  -h, --help        Print help and exit
  -v, --version     Print version and exit
```

## Dependencies

* CMake 3.18+

    ```shell
    # Mac
    brew install cmake
    # Windows
    choco install cmake
    # Debian
    sudo apt install cmake
    ```

* OpenSSL

    ```shell
    # Mac
    brew install openssl
    # Windows
    choco install openssl
    # Debian
    sudo apt install libssl-dev
    ```

* A C++ compiler in PATH :grinning:

> [!NOTE]
> The build script expects to find Visual Studio 2022 on Windows,
> and [Ninja](https://ninja-build.org/) on Mac and Linux for CMake.

Optional:

* [ccache](https://ccache.dev/)
* [Ninja](https://github.com/ninja-build/ninja)

CMake will fetch the required C++ libraries automatically:

* [fmt](https://github.com/fmtlib/fmt) for string formatting, printing, and terminal colours.
* [cxxopts](https://github.com/jarro2783/cxxopts) for command line argument parsing.
* [Googletest](https://github.com/google/googletest) for unit tests.

## Build

Use the provided script to automatically build the app using CMake:

```shell
./build.sh
```

On Windows, use Git Bash provided by [Git For Windows](https://gitforwindows.org/) to run the script :wink:
You will also need to have `bash` in PATH on Windows for the version info generation to work fully.
Add `C:\Program Files\Git\bin` to the system PATH environment variable.

## TODO

* Use C++20 modules
* Replace fmt with C++20/23 standard library methods (`std::format` / `std::print`)
* Write more tests
