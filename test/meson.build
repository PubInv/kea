project('test_kea', 'cpp')

inc = include_directories('../src', '../../kea_collections/src')

kea_sources = [
  '../src/kea_core.cpp',
  '../src/kea_scheduler.cpp',
  '../src/kea_task.cpp',
  '../src/kea_time.cpp',
  '../src/kea_debug.cpp',
  '../src/kea_machine.cpp',
  '../src/kea_error_handler.cpp'
]

executable('test_kea', 'test_kea.cpp', kea_sources, include_directories : inc)
