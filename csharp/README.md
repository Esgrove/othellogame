# Othello C\#

## Usage

```console
Description:
  A simple Othello CLI game implementation

Usage:
  othello_csharp [<size>] [options]

Arguments:
  <size>  Optional board size (4..10) []

Options:
  -a, --autoplay    Enable autoplay mode
  -d, --default     Play with default settings
  -l, --log         Show log after a game
  -n, --no-helpers  Hide disk placement hints
  -t, --test        Enable test mode
  -v                Print version and exit
  --version         Show version information
  -?, -h, --help    Show help and usage information
```

## Dependencies

* [System.CommandLine](https://github.com/dotnet/command-line-api)
* [Pastel](https://github.com/silkfire/Pastel)

## Build

Using [.NET CLI](https://learn.microsoft.com/en-us/dotnet/core/tools/):

```shell
dotnet build
dotnet run --project Othello
dotnet format
```

Create standalone binary with shell script. Uses `dotnet publish` to create a self-contained native executable,
and copy it to project root:

```shell
./build.sh
```

## Tests

Tests use [xUnit](https://xunit.net/)

```shell
dotnet test
```

## Code formatting

Using [CSharpier](https://github.com/belav/csharpier)

```shell
dotnet tool install --global csharpier

dotnet csharpier .
```
