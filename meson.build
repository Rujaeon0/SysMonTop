project('SysMonTop', 'c')

gtk = dependency('gtk4')

inc = include_directories('include')

blueprint_compiler = find_program('blueprint-compiler')

ui_files = custom_target('blueprints',
  input: files('ui/SysMonTop.blp'),
  output: 'SysMonTop.ui',
  command: [
    blueprint_compiler, 'compile',
    '--output', join_paths(meson.project_source_root(), 'ui', 'SysMonTop.ui'),
    '@INPUT@'
  ],
  build_by_default: true,
)

executable(
    'SysMonTop',
    [
        'src/cpu.c',
        'src/process.c',
        'src/main.c',
        'src/unit_conversion.c',
        'src/process_object_gtk.c',
        'src/columnview.c',
        'src/column_refresh.c',
        'src/refresh.c',
        'src/network.c',
        'src/network_object_gtk.c',
        'src/graph.c',
        'src/storage.c',
        'src/pkill.c',
        'src/per_cpu.c',
        ui_files
    ],
    include_directories: inc,
    dependencies: gtk,
)