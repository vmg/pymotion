import sys, os
from distutils.core import setup, Extension
from distutils.command.build_ext import build_ext

pymotion_core = Extension(
    name = 'pymotion',
    sources = ['pymotion.c', 'unimotion.c'],
    extra_link_args = \
        ['-framework', 'IOKit',
        '-framework', 'CoreFoundation',
        '-framework', 'Carbon'],
)

result = setup(
	name = 'pymotion',
	author = 'Vicent Marti',
	author_email = 'tanoku@gmail.com',
	url  = 'http://www.bellverde.org',
	version = '0.3',
	description = \
        """Python bindings to access Apple 
        MacBook series accelerometer data.""",

	ext_modules = [pymotion_core],
)
