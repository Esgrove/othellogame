# Othello C\#

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
