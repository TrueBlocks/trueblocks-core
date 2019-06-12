from setuptools import find_packages
from distutils.core import setup, Extension

extension = Extension(name='_quickblocks',
        include_dirs = ['/usr/local/include', '/usr/include/python3.6','/usr/local/qblocks/include', '../../libs/etherlib', '../../libs/utillib' ],
        libraries = ['pthread'],
        sources = ['extension.cpp'],
        language='c++',
        extra_compile_args = ['-std=c++11', '-Werror', '-Wall', '-O2', '-fPIC'],
        extra_link_args = ['-L/usr/local/qblocks/lib', '-l../../libs/acct', '-l../../libs/ether', '-l../../libs/util', '-lcurl'])

setup(name = 'QuickBlocks',
        version = '0.1.0',
        description = '',
        author = 'Mike Zhai',
        scripts=['bin/print_blocks.py'],
        packages=find_packages(),
        ext_modules=[extension])
