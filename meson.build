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

test('basic', exe)
