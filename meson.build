project('SysMonTop', 'c')

gtk = dependency('gtk4')

inc = include_directories('include')

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
        'src/refresh.c'
    ],
    include_directories: inc,
    dependencies: gtk
)




