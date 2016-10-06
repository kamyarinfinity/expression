# -*- coding: utf-8 -*-
from __future__ import absolute_import

import os.path as path
from setuptools import setup


library = 'calculate'
with open('{}/__init__.py'.format(library), 'r') as file:
    metadata = {entry.split('=')[0].strip(' '): eval(entry.split('=')[-1])
                for entry in file.read().split('\n') if '=' in entry}

extensions = ['so', 'dylib', 'dll', '']
library_name = 'lib' + library
for extension in extensions:
    basedir = path.realpath(__file__).replace(path.basename(__file__), '')
    basedir = path.join(basedir, library)
    library_path = path.join(basedir, library_name + '.' + extension)
    if path.lexists(library_path):
        break
else:
    raise EnvironmentError('Missing shared library')

setup(
    name=library,
    version=metadata['__version__'],
	license=metadata['__license__'],
    author=metadata['__author__'],
    author_email=metadata['__email__'],
    home_page=metadata['__site__'],
    description=metadata['__description__'],
    install_requires=[
	    'cffi>=1.0.1'
	],
    packages=[library],
    package_data={library: [library_name + '.' + extension]}
)