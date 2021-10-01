Computing graph diameter (experimental)
=======================================

**Warning: this is a work in progress project**: This repository is a reboot of some experiment I have worked on when I was working as EPITA's system lab, it's not backed by any academic results. I work on it for **fun**.

## Computing diameter in less than O(N^3) ##

Blog post comming (soon hopefully).

This is related to this presentation (in french and probably outdated): https://youtu.be/WJsWHILA3YA

## The code ##

This implementation is in _modern_ C++ (at least, I'm building with C++20 flags), it uses no external library for
now, and should build on most standard installation as long as you have `meson` and a C++ compiler.

### Building ###

from the repo:

```
> meson setup builddir -Doptimization=3
> cd builddir
> meson compile
```

### Running the code ###

_Note: the code is still in a very early stage_

First you need a graph in the _nde_ format (see **Graph** section later).

The code expect an undirected connected graph, if you are not sure that your graph is connected, you can use
`prepare-graph` a tiny tool built with the main program that read a graph in nde format, find the largest connected
component and output (on standard out) a new graph from this component. Note that if the graph is already
connected, the tool _reorder_ the graph following a breath first traversal.

Then all you need to do is to run `graph-diameter` with the name of your file.

## Graph ##

The program read graph from the nde format, a very simple text format for graph. The format is organized as follow:

* The order of the graph (number of vertices) on a single line
* A series of lines for each vertex with `id` and `degree` separated by a space
* A series of lines for each edge with source and destination separated by a space

As the graph is undirected, an edge should appears only once and order does not matter.

A simple example:

```
7
0 1
1 3
2 3
3 3
4 1
5 2
6 1
0 1
1 2
1 3
2 4
2 5
3 5
3 6
```

You can find a lot of interesting graphs on this page: https://www.pilucrescenzi.it/wp/networks/

