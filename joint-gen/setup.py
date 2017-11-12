#!/usr/bin/env python

from distutils.core import setup


setup(
    name='joint-gen',
    version='0.0',
    description='Joint adapters generator',
    author='Dmitry Koplyarov',
    author_email='koplyarov.da@gmail.com',
    url='https://github.com/koplyarov/joint',
    packages=['joint', 'joint.generators'],
    scripts=['joint-gen.py']
)
