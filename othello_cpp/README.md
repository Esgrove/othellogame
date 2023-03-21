# Othello CPP

## Dependencies

* CMake 3.18+

    ```shell
    # Mac
    brew install cmake
    # Windows
    choco install cmake
    ```

* A C++ compiler in PATH :grinning:

Optional:

* [ccache](https://ccache.dev/)
* [Ninja](https://github.com/ninja-build/ninja)

CMake will pull required C++ libraries automatically:

* [fmt](https://github.com/fmtlib/fmt)
* [Googletest](https://github.com/google/googletest)

## Build

Use the provided script to automatically build the app using CMake.

```shell
./build.sh
```

On Windows, use Git Bash provided by [Git For Windows](https://gitforwindows.org/) to run the script :wink:
You will also need to have `bash` in PATH on Windows for the version info generation to work fully.
Add `C:\Program Files\Git\bin` to the system PATH environment variable.

:warning: The build script expects to find Visual Studio 2022 on Windows,
and [Ninja](https://ninja-build.org/) on Mac and Linux.
Other generators could of course be used, or let CMake decide by not specifying anything...

## TODO

* Use C++20 modules
* Replace fmt with C++20/23 standard library methods (`std::format` / `std::print`)
* Write tests
