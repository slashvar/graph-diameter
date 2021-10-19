project('graph-diameter', 'cpp', default_options: ['warning_level=3', 'cpp_std=c++20'])

cxxopts_proj = subproject('cxxopts')
cxxopts_dep = cxxopts_proj.get_variable('cxxopts_dep')

incdir = include_directories('include')

src = ['src/nde.cc', 'src/graph-diameter.cc']

executable('graph-diameter', src, include_directories : incdir, dependencies: [cxxopts_dep])
executable('prepare-graph', ['src/tools/prepare-graph.cc', 'src/nde.cc'], include_directories: incdir)
executable('to-dot', ['src/tools/to-dot.cc', 'src/nde.cc'], include_directories: incdir)

