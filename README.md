# Scalpa Compiler

STAVRIDIS Adonis

## Compile the Scalpa Compiler

In order to compile the project, you need:

- gcc - `sudo apt install gcc`
- bison - `sudo apt install bison`
- libbison - `sudo apt install libbison-dev`
- flex - `sudo apt install flex`
- libfl - `sudo apt install libfl-dev`
- spim - `sudo apt install spim`

Then, you can go to the root of the project and run

```
make
```

The binary executable `scalpa` is in `./bin`

## Synopsis

```
scalpa [-version] [-tos] [-o outfile] infile
```

## Projet handling

You can compile the entire project by running

```
make
```

Open the documentation with

```bash
make doc
```

You can run the test suite with the following command

```
make test
```

Clean the entire project with

```
make clean
```
