project('graph-diameter', 'cpp', default_options: ['warning_level=3', 'cpp_std=c++20'])

incdir = include_directories('include')

src = ['src/nde.cc', 'src/graph-diameter.cc']

executable('graph-diameter', src, include_directories : incdir)
executable('prepare-graph', ['src/tools/prepare-graph.cc', 'src/nde.cc'], include_directories: incdir)

