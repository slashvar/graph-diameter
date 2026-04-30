project('graph-diameter', 'cpp', default_options: ['warning_level=3', 'cpp_std=c++20'])

cxxopts_proj = subproject('cxxopts')
cxxopts_dep = cxxopts_proj.get_variable('cxxopts_dep')

incdir = include_directories('include')

src = ['src/nde.cc', 'src/graph-diameter.cc']

executable('graph-diameter', src, include_directories : incdir, dependencies: [cxxopts_dep])
executable('prepare-graph', ['src/tools/prepare-graph.cc', 'src/nde.cc'], include_directories: incdir)
executable('to-dot', ['src/tools/to-dot.cc', 'src/nde.cc'], include_directories: incdir)

if get_option('tests')
  subdir('tests')
endif

clang_format = find_program('clang-format', required: false)
git_ls_files = run_command(
  'git', 'ls-files', '--cached', '--others', '--exclude-standard',
  '*.h', '*.cc',
  check: false,
)
if clang_format.found() and git_ls_files.returncode() == 0
  format_files = files(git_ls_files.stdout().strip().split('\n'))
  run_target('format', command: [clang_format, '-i', format_files])
  run_target('format-check', command: [clang_format, '--dry-run', '--Werror', format_files])
endif
