#!/usr/bin/python

import argparse
import mimetypes
import magic
import os
import subprocess
import sys


def list_files(root_dir):
    def normpath(p):
        return os.path.normpath(os.path.abspath(p))

    files_blacklist = {
        normpath(os.path.join(root_dir, p)) for p in [
            '.gitmodules'
        ]
    }

    git_ls_files_ok = True
    out = subprocess.check_output(['git', 'ls-files', '--exclude-standard', root_dir])
    for filename in out.split('\n'):
        if not filename or normpath(filename) in files_blacklist or os.path.isdir(filename):
            continue

        yield filename


def main():
    parser = argparse.ArgumentParser(description='Whitespaces validator')
    parser.add_argument('--dir', help='Sources directory', required=True)
    args = parser.parse_args()

    exclude_dirs = {'.ci', '.git', 'build'}
    exclude_files = {os.path.join(args.dir, 'tags')}
    exclude_extensions = {'pyc'}

    exit_status = 0

    for filename in list_files(args.dir):
        magic_type_str = magic.from_file(filename)
        if 'text' not in magic_type_str:
            continue

        with open(filename) as f:
            for i, l in enumerate(f.readlines()):
                if '\t' in l:
                    sys.stdout.write('{}:{}: tabs here\n{}\n'.format(filename, i + 1, l))
                    exit_status = 1

    exit(exit_status)

if __name__ == '__main__':
    main()
