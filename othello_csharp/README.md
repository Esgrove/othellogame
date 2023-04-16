# Othello C\#

Using [.NET CLI](https://learn.microsoft.com/en-us/dotnet/core/tools/):

```shell
dotnet build
dotnet run --project Othello
dotnet format
```

Shell script. Uses `dotnet publish` to create a self-contained native executable,
and copy it to project root:

```shell
./build.sh
```
