unit_tests = [
  'box',
  'euler',
  'frustum',
  'matrix',
  'plane',
  'point',
  'point3d',
  'quad',
  'quaternion',
  'ray',
  'rect',
  'simd',
  'size',
  'sphere',
  'triangle',
  'vec2',
  'vec3',
  'vec4'
]

python = python3.find_python()
gen_installed_test = join_paths(meson.current_source_dir(), 'gen-installed-test.py')

test_suffix = join_paths('installed-tests', graphene_api_path)

test_datadir = join_paths(get_option('installed_test_datadir'), test_suffix)
if test_datadir == ''
    test_datadir = join_paths(get_option('prefix'), get_option('datadir'), test_suffix)
endif

test_bindir = join_paths(get_option('installed_test_bindir'), test_suffix)
if test_bindir == ''
    test_bindir = join_paths(get_option('prefix'), get_option('libexecdir'), test_suffix)
endif

foreach unit: unit_tests
  wrapper = '@0@.test'.format(unit)
  data = custom_target(wrapper,
                       output: wrapper,
                       command: [
                         python,
                         gen_installed_test,
                         '--testdir=@0@'.format(test_bindir),
                         '--testname=@0@'.format(unit),
                         '--outdir=@OUTDIR@',
                         '--outfile=@0@'.format(wrapper),
                       ],
                       install: true,
                       install_dir: test_datadir)

  exe = executable(unit, unit + '.c',
                   dependencies: graphene_dep,
                   include_directories: graphene_inc,
                   c_args: common_cflags + [
                     '-DG_LOG_DOMAIN="Graphene-Test"',
                     '-DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_2_30',
                     '-DGLIB_VERSION_MAX_ALLOWED=GLIB_VERSION_2_32',
                     '-DGLIB_DISABLE_DEPRECATION_WARNINGS',
                   ],
                   install: true,
                   install_dir: test_bindir)

  test(unit, exe, args: [ '--tap', '-k' ])
endforeach
