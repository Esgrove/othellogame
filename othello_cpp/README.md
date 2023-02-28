# Othello CPP

Use the provided script to automatically build the app using CMake.

```shell
./build.sh
```

On Windows, use Git Bash provided by [Git For Windows](https://gitforwindows.org/) to run the script :wink:

You will also need to have bash in PATH on Windows for the CMake version info generation to work fully.
Add `C:\Program Files\Git\bin` to the system PATH environment variable.

:warning: Note: the script expects to find Visual Studio 2022 on Windows,
and [Ninja](https://ninja-build.org/) on Mac and Linux.
Other generators could of course be used, or let CMake decide by not specifying anything...
