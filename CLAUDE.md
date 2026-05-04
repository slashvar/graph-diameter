# graph-diameter

Personal C++20 project: experimental algorithms for computing graph diameter
(max shortest-path between any two vertices) faster than O(N³). Reboot of an
old EPITA system-lab experiment. Header-only library + 3 driver executables.

## Build / test / lint

The build system is meson + ninja. From the repo root:

```sh
meson setup builddir              # configure (werror=true is the default)
meson compile -C builddir         # build all 3 executables + tests
meson test -C builddir            # run the gtest suite (32 tests)
meson compile -C builddir format-check    # clang-format gate
meson compile -C builddir tidy-check      # clang-tidy gate (WarningsAsErrors=*)
meson compile -C builddir format          # apply clang-format in place
meson compile -C builddir tidy-fix        # apply clang-tidy fixes
```

Build options (in `meson_options.txt`):
- `tests` (bool, default true) — build the gtest suite.
- `extra_warnings` (bool, default true) — adds `-Wconversion`, `-Wshadow`,
  `-Wold-style-cast`, `-Wnon-virtual-dtor`, `-Wnull-dereference`,
  `-Woverloaded-virtual` on top of `warning_level=3`.

`-Werror` is enforced via the project's `werror=true` default option. Any new
warning fails the build.

The `format` and `tidy-*` targets discover sources with `git ls-files
--cached --others --exclude-standard`, so subprojects (cxxopts, gtest) are
excluded automatically. `cxxopts` is also wrapped with `.as_system()` so its
headers are silenced for both the compiler and clang-tidy.

## Layout

| Path | What it is |
|---|---|
| `include/graph.h` | `Graph` class — adjacency lists; `operator[]` returns `std::span<const std::size_t>`. |
| `include/bfs.h` | `BFS` class — encapsulated state, accessors (`distances()`, `last_visited()`, `was_closed()`, `is_leaf()`, etc.). |
| `include/nde.h` + `src/nde.cc` | NDE text format I/O. `nde::load(path)` and `nde::load(std::istream&)` overloads; `nde::serialize<output_stream Stream>(graph, out)`. |
| `include/cut-points.h` | `CutPoints` class — articulation-point iterative DFS. **De-recursified** (heap stack, not call stack); preserve that property when editing. |
| `include/full_diameter.h` | `full::run<bool WithRadius>(graph)` — free function, BFS-from-every-vertex baseline, returns `Result {diameter, radius, bfs_runs, connected}`. |
| `include/smart_diameter.h` | `smart::Diameter` class — same Result shape, with the candidate-pruning heuristic. Ctor takes `(order, std::optional<size_t> source)`. |
| `src/graph-diameter.cc` | Main executable: load, cut-points, smart::Diameter. |
| `src/tools/prepare-graph.cc` | Extracts the largest connected component, BFS-renumbers, re-emits NDE. |
| `src/tools/to-dot.cc` | Emits GraphViz DOT. |
| `tests/` | gtest suite. Helpers in `tests/helpers.h` (graph builders, `TempNDEFile`, `adj_equal`, `graphs_equal`). |

## Code conventions

### Style
- `.clang-format`: `BasedOnStyle: Google` with overrides — `IndentWidth: 4`,
  `ColumnLimit: 115`, `AccessModifierOffset: -4`, aligned consecutive
  decls/assignments, `BreakConstructorInitializers: AfterColon`,
  `BreakBeforeBraces: Mozilla` (so opening braces of class/function
  definitions go on a new line), no bin-packing of args/params, no short
  ifs/loops/non-empty-functions on one line.
- Member fields end in `_` (private encapsulation convention).
- `[[nodiscard]]` on non-mutating methods returning a value of interest.
- `noexcept` only when truly non-throwing (don't lie — vector allocations
  can throw `bad_alloc`).
- Empty-graph guards (`if (order == 0) return ...`) in algorithms.

### C++20 features in use
- `std::span` for non-owning views (return type of `Graph::operator[]`,
  `BFS::distances()`).
- `std::ranges::*` selectively (e.g. `std::ranges::sort`, `std::ranges::fill`).
- Concepts: `nde::output_stream` constrains `nde::serialize`.
- `std::optional` for "no value" semantics (e.g. `smart::Diameter`'s source,
  `CutPoints` parents).
- Designated initializers for aggregate construction.

### Anti-patterns to avoid
- `std::vector<bool>` (use `std::vector<char>` — see `BFS`, `CutPoints`,
  `smart::Diameter`).
- Magic-number sentinels (`-1`, `order`, self-loop "I'm root"). Use
  `std::optional` or a separate flag vector.
- `errx()` and other C-style varargs (use `std::cerr` + `EXIT_FAILURE`).
- Public mutable state on functor classes — encapsulate, expose accessors.
- `noexcept` on functions that allocate.
- Implicit `size_t → bool` conversion (write `count != 0` explicitly).

## Test strategy

- gtest suite in `tests/`, one file per module.
- TDD-flavored for behaviour-changing edits: write the failing test first
  (red), apply the minimal change (green), refactor.
- For pure refactors, the existing tests are the regression net.
- `tests/helpers.h` provides graph builders (`make_path`, `make_cycle`,
  `make_complete`, `make_star`, `make_two_triangles`), `adj_equal` /
  `graphs_equal` comparison helpers, and `TempNDEFile` for NDE I/O tests.
- `EXPECT_THROW(nde::load(...), ...)` needs `(void)`-cast around `nde::load`
  because it's `[[nodiscard]]`.

## PR workflow

- Branch naming: `marwan/<type>/<scope>/<short-description>` (the `/pr`
  skill enforces this).
- Always branch off `main`. **Never stack PRs.** A previous round had PR #2
  stacked on PR #1; when #1 squash-merged, #2 ended up merging into the
  orphaned feature branch and never reached main.
- Single comprehensive commit per PR (matches recent commit style).
- Per-PR workflow:
  1. branch fresh off main
  2. write failing test for behaviour changes
  3. apply refactor
  4. run `/simplify` (multi-agent reuse/quality/efficiency review)
  5. gates: `format-check` + `tidy-check` + `meson test` all green
  6. commit + `/pr`

## Useful one-liners

```sh
# Quick smoke of the executables on a 3-vertex path
printf "3\n0 1\n1 2\n2 1\n0 1\n1 2\n" > /tmp/t.nde
./builddir/graph-diameter /tmp/t.nde
./builddir/prepare-graph  /tmp/t.nde
./builddir/to-dot         /tmp/t.nde

# NDE format reference
# Line 1: order (vertex count)
# Lines 2..order+1: "<vertex_id> <degree>"
# Remaining lines: "<src> <dst>" undirected edges, each edge listed once
```

## Useful graphs

The README mentions <https://www.pilucrescenzi.it/wp/networks/> as a source
of NDE-format graphs.
