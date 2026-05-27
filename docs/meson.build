custom_target(
    'index.html',
    command: [
        asciidoctor_exe,
        '@INPUT@',
        '-D',
        '@OUTDIR@',
    ],
    input: files('index.adoc'),
    output: ['index.html'],
    install: true,
    install_tag: 'doc',
    install_dir: get_option('datadir') / 'doc' / meson.project_name(),
    build_by_default: false,
)
