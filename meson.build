project(
    'graph-diameter',
    'cpp',
    default_options: ['warning_level=3', 'cpp_std=c++20', 'werror=true'],
)

if get_option('extra_warnings')
  add_project_arguments(
    [
      '-Wconversion',
      '-Wshadow',
      '-Wold-style-cast',
      '-Wnon-virtual-dtor',
      '-Wnull-dereference',
      '-Woverloaded-virtual',
    ],
    language: 'cpp',
  )
endif

cxxopts_proj = subproject('cxxopts')
cxxopts_dep = cxxopts_proj.get_variable('cxxopts_dep').as_system('system')

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

clang_tidy = find_program('clang-tidy', required: false)
git_ls_cc = run_command(
  'git', 'ls-files', '--cached', '--others', '--exclude-standard',
  '*.cc',
  check: false,
)
if clang_tidy.found() and git_ls_cc.returncode() == 0
  tidy_files = files(git_ls_cc.stdout().strip().split('\n'))
  tidy_extra_args = []
  if host_machine.system() == 'darwin'
    sdk = run_command('xcrun', '--show-sdk-path', check: false)
    if sdk.returncode() == 0
      tidy_extra_args += ['--extra-arg-before=-isysroot', '--extra-arg-before=' + sdk.stdout().strip()]
    endif
  endif
  run_target(
    'tidy-check',
    command: [clang_tidy, '-p', meson.current_build_dir(), '--quiet', tidy_extra_args, tidy_files],
  )
  run_target(
    'tidy-fix',
    command: [clang_tidy, '-p', meson.current_build_dir(), '--quiet', '--fix', tidy_extra_args, tidy_files],
  )
endif
