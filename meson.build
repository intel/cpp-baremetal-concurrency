project(
    'baremetal-concurrency',
    'cpp',
    version: '0.1.0',
    license: 'BSL-1.0',
    default_options: ['cpp_std=c++23'],
    meson_version: '>=0.63.0',
)

inc = include_directories('include')
src = files(
    'include/conc/atomic.hpp',
    'include/conc/concepts.hpp',
    'include/conc/concurrency.hpp',
)
src_detail = files(
    'include/conc/detail/freestanding.hpp',
)

install_headers(src, subdir: 'conc')
install_headers(src_detail, subdir: 'conc' / 'detail')

conc_dep = declare_dependency(
    include_directories: inc,
)

meson.override_dependency(meson.project_name(), conc_dep)

pkgconfig = import('pkgconfig', required: false)
if pkgconfig.found()
    pkgconfig.generate(
        name: meson.project_name(),
        description: 'A header-only C++ library that provides low-level concurrency hooks.',
        url: 'https://intel.github.io/cpp-baremetal-concurrency/',
        install_dir: get_option('datadir') / 'pkgconfig',
    )
endif

asciidoctor_exe = find_program('asciidoctor', required: get_option('docs'))
subdir('docs', if_found: asciidoctor_exe)

catch2_dep = dependency('catch2-with-main', required: get_option('tests'))
subdir('test', if_found: catch2_dep)
