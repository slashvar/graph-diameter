gtest_dep = dependency('gtest_main', required: true)

unit_tests = executable(
    'unit_tests',
    [
        '../src/nde.cc',
        'test_graph.cc',
        'test_bfs.cc',
        'test_nde.cc',
        'test_cut_points.cc',
        'test_full_diameter.cc',
        'test_smart_diameter.cc',
    ],
    include_directories: include_directories('../include', '.'),
    dependencies: [gtest_dep],
    override_options: ['warning_level=2'],
)

test('unit', unit_tests, protocol: 'gtest')
