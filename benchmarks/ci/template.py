#!/usr/bin/python

import argparse
import colorama
import json


data = {
    'cpp, component void()': ${basic.invoke_void_void.joint(lang:cpp)[main]} / ${basic.invokeNative_void_void.joint(lang:cpp)[main]},
    'cpp, proxy void()': ${basic.invokeOutgoing_void_void.joint(lang:cpp)[main]} / ${basic.invokeNative_void_void.joint(lang:cpp)[main]},
    'cpp, component i32()': ${basic.invoke_i32_void.joint(lang:cpp)[main]} / ${basic.invokeNative_i32_void.joint(lang:cpp)[main]},
    'cpp, proxy i32()': ${basic.invokeOutgoing_i32_void.joint(lang:cpp)[main]} / ${basic.invokeNative_i32_void.joint(lang:cpp)[main]},
    'cpp, component void(i32)': ${basic.invoke_void_i32.joint(lang:cpp)[main]} / ${basic.invokeNative_void_i32.joint(lang:cpp)[main]},
    'cpp, proxy void(i32)': ${basic.invokeOutgoing_void_i32.joint(lang:cpp)[main]} / ${basic.invokeNative_void_i32.joint(lang:cpp)[main]},
}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--canonize', action='store_true', help='Canonize benchmarks')
    parser.add_argument('--data', required=True, help='Canonized data path')
    args = parser.parse_args()

    if args.canonize:
        with open(args.data, 'w') as f:
            f.write(json.dumps(data, indent=4))
    else:
        has_errors = False
        def error(msg):
            has_errors = True
            print('{fg}{msg}{rs}'.format(msg=msg, fg=colorama.Fore.RED, rs=colorama.Style.RESET_ALL))

        def info(msg):
            print(msg)

        def ok(msg):
            print('{fg}{msg}{rs}'.format(msg=msg, fg=colorama.Fore.GREEN, rs=colorama.Style.RESET_ALL))

        with open(args.data) as f:
            canon_data = json.loads(f.read())
            keys = set(canon_data.keys())
            keys.update(data.keys())
            for key in keys:
                if key not in canon_data:
                    error('New key: {}'.format(key))
                elif key not in data:
                    error('Missing key: {}'.format(key))
                elif data[key] > canon_data[key] * 1.2:
                    error('Slower: {} (canonized: {}, actual: {})'.format(key, canon_data[key], data[key]))
                else:
                    info('OK: {} (canonized: {}, actual: {})'.format(key, canon_data[key], data[key]))

            if has_errors:
                error('{} errors!'.format(len(errors)))
                exit(1)
            else:
                ok('OK')


if (__name__ == '__main__'):
    main()
