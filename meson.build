project(
  'monte-carlo-pi',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=3', 'cpp_std=c++23'],
)

sources = [
  'src/main.cpp',
]

exe = executable(
  'main',
  sources,
  install : true,
)

test('cli no args fails', exe, should_fail : true)
test('cli invalid type fails', exe, args : ['invalid', '1000', '1'], should_fail : true)
test('cli single_thread', exe, args : ['single_thread', '100000', '1'])
test('cli multi_thread', exe, args : ['multi_thread', '100000', '1', '4'])

test_exe = executable(
  'test_pi',
  'tests/test_pi.cpp',
)

test('approximate_pi', test_exe)
