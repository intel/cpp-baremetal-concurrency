atomic_override_cdata = configuration_data()
atomic_override_cdata.set_quoted(
    'ATOMIC_CFG',
    meson.current_source_dir() / 'atomic_cfg.hpp',
)

tests = {
    'atomic_injected_policy': {
        'cpp_args': [
            '-DATOMIC_CFG=' + atomic_override_cdata.get('ATOMIC_CFG'),
        ],
    },
    'atomic_standard_policy': {},
    'conc_standard_policy': {},
    'conc_test_policy': {},
    'concepts': {},
    'freestanding_conc_injected_policy': {},
    'hosted_conc_injected_policy': {},
}

threads_dep = dependency('threads', required: false)

foreach name, args : tests
    test(
        name,
        executable(
            f'@name@_test',
            f'@name@.cpp',
            dependencies: [conc_dep, catch2_dep, threads_dep],
            cpp_args: args.get('cpp_args', []),
            build_by_default: false,
        ),
        args: ['-r', 'tap'],
        protocol: 'tap',
    )
endforeach
