from setuptools import find_packages
from distutils.core import setup, Extension

static_lib_dir='../../libs'
static_libraries=['acct', 'ether','util']
extra_objects = ['{}/lib{}.a'.format(static_lib_dir, l) for l in static_libraries]

extension = Extension(name='_quickblocks',
        include_dirs = ['/usr/local/include', '/usr/include/python3.6', '../../libs/etherlib', '../../libs/utillib' ],
        extra_objects=extra_objects,
        libraries = ['pthread'],
        sources = ['extension.cpp'],
        language='c++',
        extra_compile_args = ['-std=c++11', '-Werror', '-Wall', '-O2', '-fPIC'],
        extra_link_args = ['-L../../libs', '-lcurl'])

setup(name = 'QuickBlocks',
        version = '0.1.0',
        description = '',
        author = 'Mike Zhai',
        scripts=['print_blocks.py'],
        packages=find_packages(),
        ext_modules=[extension])
