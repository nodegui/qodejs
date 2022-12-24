
{
    'sources': [
        'qode.cc',
        'integration/node_integration.cc',
        'helpers/qode_helper.cc',
    ],
    'defines': [
        'NODE_WANT_INTERNALS=1',
        'HAVE_OPENSSL=1',
        'HAVE_INSPECTOR=1',
        'NODE_SHARED_MODE',
    ],
    'include_dirs': [
        '.',
        '..',
        '../deps',
    ],
    "cflags": [
        "-std=c++14"
    ],
    'conditions': [
        ['OS=="mac"', {
            'sources': [
                'integration/node_integration_mac.mm',
            ],
        }],
        ['OS=="win"', {
            'sources': [
                'qode.rc',
                'integration/node_integration_win.cc',                    
            ],
            'msvs_settings': {
                'VCManifestTool': {
                    # Manifest file.
                    'EmbedManifest': 'true',
                    'AdditionalManifestFiles': 'qode/qode.exe.manifest'
                },
                'VCLinkerTool': {
                    # Using 5.01 would make Windows turn on compatibility mode for
                    # certain win32 APIs, which would return wrong results.
                    'MinimumRequiredVersion': '5.02',
                    # A win32 GUI program use 2 and for CUI use 1.
                    'SubSystem': '1',
                    # Defined in node target, required for building x86.
                    'ImageHasSafeExceptionHandlers': 'false',
                    # Disable incremental linking, for smaller program.
                    'LinkIncremental': 1,
                },
            },
        }],
        ['OS in "linux freebsd"', {
            'sources': [
                'integration/node_integration_linux.cc',
            ],
            'libraries': [
                '<!@(pkg-config glib-2.0 --libs)',
            ],
            'include_dirs': [
                '<!@(pkg-config glib-2.0 --cflags-only-I | sed s/-I//g)',
            ],
            'ldflags': [
                '-Wl,--no-whole-archive',
                "-Wl,-rpath,'$$ORIGIN/lib'"
            ],
        }],
    ],
}