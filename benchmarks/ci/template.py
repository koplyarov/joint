#!/usr/bin/python

from __future__ import division

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
    'cpp, component string3()': ${basic.invoke_string3_void.joint(lang:cpp)[main]} / ${basic.invokeNative_string3_void.joint(lang:cpp)[main]},
    'cpp, proxy string3()': ${basic.invokeOutgoing_string3_void.joint(lang:cpp)[main]} / ${basic.invokeNative_string3_void.joint(lang:cpp)[main]},
    'cpp, component void(string3)': ${basic.invoke_void_string3.joint(lang:cpp)[main]} / ${basic.invokeNative_void_string3.joint(lang:cpp)[main]},
    'cpp, proxy void(string3)': ${basic.invokeOutgoing_void_string3.joint(lang:cpp)[main]} / ${basic.invokeNative_void_string3.joint(lang:cpp)[main]},
    'cpp, component string100()': ${basic.invoke_string100_void.joint(lang:cpp)[main]} / ${basic.invokeNative_string100_void.joint(lang:cpp)[main]},
    'cpp, proxy string100()': ${basic.invokeOutgoing_string100_void.joint(lang:cpp)[main]} / ${basic.invokeNative_string100_void.joint(lang:cpp)[main]},
    'cpp, component void(string100)': ${basic.invoke_void_string100.joint(lang:cpp)[main]} / ${basic.invokeNative_void_string100.joint(lang:cpp)[main]},
    'cpp, proxy void(string100)': ${basic.invokeOutgoing_void_string100.joint(lang:cpp)[main]} / ${basic.invokeNative_void_string100.joint(lang:cpp)[main]},

    'cpp, component cast': ${cast.castComponentSide.joint(lang:cpp)[main]} / ${cast.castNative.joint(lang:cpp)[main]},
    'cpp, proxy cast': ${cast.castProxySide.joint(lang:cpp)[main]} / ${cast.castNative.joint(lang:cpp)[main]},
    'cpp, component throw': ${exception.throwComponentSide.joint(lang:cpp)[main]} / ${exception.throwNative.joint(lang:cpp)[main]},
    'cpp, proxy throw': ${exception.throwProxySide.joint(lang:cpp)[main]} / ${exception.throwNative.joint(lang:cpp)[main]},
    'cpp, component enum()': ${enum.invoke_void_enum.joint(lang:cpp)[main]} / ${enum.invokeNative_void_enum.joint(lang:cpp)[main]} ,
    'cpp, proxy enum()': ${enum.invokeOutgoing_void_enum.joint(lang:cpp)[main]} / ${enum.invokeNative_void_enum.joint(lang:cpp)[main]},

    'c, component void()': ${basic.invoke_void_void.joint(lang:c)[main]} / ${basic.invokeNative_void_void.joint(lang:c)[main]},
    'c, proxy void()': ${basic.invokeOutgoing_void_void.joint(lang:c)[main]} / ${basic.invokeNative_void_void.joint(lang:c)[main]},
    'c, component i32()': ${basic.invoke_i32_void.joint(lang:c)[main]} / ${basic.invokeNative_i32_void.joint(lang:c)[main]},
    'c, proxy i32()': ${basic.invokeOutgoing_i32_void.joint(lang:c)[main]} / ${basic.invokeNative_i32_void.joint(lang:c)[main]},
    'c, component void(i32)': ${basic.invoke_void_i32.joint(lang:c)[main]} / ${basic.invokeNative_void_i32.joint(lang:c)[main]},
    'c, proxy void(i32)': ${basic.invokeOutgoing_void_i32.joint(lang:c)[main]} / ${basic.invokeNative_void_i32.joint(lang:c)[main]},
    'c, component string3()': ${basic.invoke_string3_void.joint(lang:c)[main]} / ${basic.invokeNative_string3_void.joint(lang:c)[main]},
    'c, proxy string3()': ${basic.invokeOutgoing_string3_void.joint(lang:c)[main]} / ${basic.invokeNative_string3_void.joint(lang:c)[main]},
    'c, component void(string3)': ${basic.invoke_void_string3.joint(lang:c)[main]} / ${basic.invokeNative_void_string3.joint(lang:c)[main]},
    'c, proxy void(string3)': ${basic.invokeOutgoing_void_string3.joint(lang:c)[main]} / ${basic.invokeNative_void_string3.joint(lang:c)[main]},
    'c, component string100()': ${basic.invoke_string100_void.joint(lang:c)[main]} / ${basic.invokeNative_string100_void.joint(lang:c)[main]},
    'c, proxy string100()': ${basic.invokeOutgoing_string100_void.joint(lang:c)[main]} / ${basic.invokeNative_string100_void.joint(lang:c)[main]},
    'c, component void(string100)': ${basic.invoke_void_string100.joint(lang:c)[main]} / ${basic.invokeNative_void_string100.joint(lang:c)[main]},
    'c, proxy void(string100)': ${basic.invokeOutgoing_void_string100.joint(lang:c)[main]} / ${basic.invokeNative_void_string100.joint(lang:c)[main]},

    'c, component cast': ${cast.castComponentSide.joint(lang:c)[main]} / ${cast.castNative.joint(lang:cpp)[main]},
    'c, proxy cast': ${cast.castProxySide.joint(lang:c)[main]} / ${cast.castNative.joint(lang:cpp)[main]},
    'c, component throw': ${exception.throwComponentSide.joint(lang:c)[main]} / ${exception.throwNative.joint(lang:cpp)[main]},
    'c, proxy throw': ${exception.throwProxySide.joint(lang:c)[main]} / ${exception.throwNative.joint(lang:cpp)[main]},
    'c, component enum()': ${enum.invoke_void_enum.joint(lang:c)[main]} / ${enum.invokeNative_void_enum.joint(lang:c)[main]} ,
    'c, proxy enum()': ${enum.invokeOutgoing_void_enum.joint(lang:c)[main]} / ${enum.invokeNative_void_enum.joint(lang:c)[main]},

    'python, component void()': ${basic.invoke_void_void.joint(lang:python)[main]} / ${basic.invokeNative_void_void.joint(lang:python)[main]},
    'python, proxy void()': ${basic.invokeOutgoing_void_void.joint(lang:python)[main]} / ${basic.invokeNative_void_void.joint(lang:python)[main]},
    'python, component i32()': ${basic.invoke_i32_void.joint(lang:python)[main]} / ${basic.invokeNative_i32_void.joint(lang:python)[main]},
    'python, proxy i32()': ${basic.invokeOutgoing_i32_void.joint(lang:python)[main]} / ${basic.invokeNative_i32_void.joint(lang:python)[main]},
    'python, component void(i32)': ${basic.invoke_void_i32.joint(lang:python)[main]} / ${basic.invokeNative_void_i32.joint(lang:python)[main]},
    'python, proxy void(i32)': ${basic.invokeOutgoing_void_i32.joint(lang:python)[main]} / ${basic.invokeNative_void_i32.joint(lang:python)[main]},
    'python, component string3()': ${basic.invoke_string3_void.joint(lang:python)[main]} / ${basic.invokeNative_string3_void.joint(lang:python)[main]},
    'python, proxy string3()': ${basic.invokeOutgoing_string3_void.joint(lang:python)[main]} / ${basic.invokeNative_string3_void.joint(lang:python)[main]},
    'python, component void(string3)': ${basic.invoke_void_string3.joint(lang:python)[main]} / ${basic.invokeNative_void_string3.joint(lang:python)[main]},
    'python, proxy void(string3)': ${basic.invokeOutgoing_void_string3.joint(lang:python)[main]} / ${basic.invokeNative_void_string3.joint(lang:python)[main]},
    'python, component string100()': ${basic.invoke_string100_void.joint(lang:python)[main]} / ${basic.invokeNative_string100_void.joint(lang:python)[main]},
    'python, proxy string100()': ${basic.invokeOutgoing_string100_void.joint(lang:python)[main]} / ${basic.invokeNative_string100_void.joint(lang:python)[main]},
    'python, component void(string100)': ${basic.invoke_void_string100.joint(lang:python)[main]} / ${basic.invokeNative_void_string100.joint(lang:python)[main]},
    'python, proxy void(string100)': ${basic.invokeOutgoing_void_string100.joint(lang:python)[main]} / ${basic.invokeNative_void_string100.joint(lang:python)[main]},

    'python, component cast': ${cast.castComponentSide.joint(lang:python)[main]} / ${cast.castNative.joint(lang:cpp)[main]},
    'python, proxy cast': ${cast.castProxySide.joint(lang:python)[main]} / ${cast.castNative.joint(lang:cpp)[main]},
    'python, component throw': ${exception.throwComponentSide.joint(lang:python)[main]} / ${exception.throwNative.joint(lang:python)[main]},
    'python, proxy throw': ${exception.throwProxySide.joint(lang:python)[main]} / ${exception.throwNative.joint(lang:python)[main]},
    'python, component enum()': ${enum.invoke_void_enum.joint(lang:python)[main]} / ${enum.invokeNative_void_enum.joint(lang:cpp)[main]} ,
    'python, proxy enum()': ${enum.invokeOutgoing_void_enum.joint(lang:python)[main]} / ${enum.invokeNative_void_enum.joint(lang:cpp)[main]},

    'vs swig, python, component void()': ${basic.invoke_void_void.joint(lang:python)[main]} / ${basic.invoke_void_void.swig(lang:python)[main]},
    'vs swig, python, proxy void()': ${basic.invokeOutgoing_void_void.joint(lang:python)[main]} / ${basic.invoke_void_void.swig(lang:python)[main]},
    'vs swig, python, component i32()': ${basic.invoke_i32_void.joint(lang:python)[main]} / ${basic.invoke_i32_void.swig(lang:python)[main]},
    'vs swig, python, proxy i32()': ${basic.invokeOutgoing_i32_void.joint(lang:python)[main]} / ${basic.invoke_i32_void.swig(lang:python)[main]},
    'vs swig, python, component void(i32)': ${basic.invoke_void_i32.joint(lang:python)[main]} / ${basic.invoke_void_i32.swig(lang:python)[main]},
    'vs swig, python, proxy void(i32)': ${basic.invokeOutgoing_void_i32.joint(lang:python)[main]} / ${basic.invoke_void_i32.swig(lang:python)[main]},
    'vs swig, python, component string3()': ${basic.invoke_string3_void.joint(lang:python)[main]} / ${basic.invoke_string3_void.swig(lang:python)[main]},
    'vs swig, python, proxy string3()': ${basic.invokeOutgoing_string3_void.joint(lang:python)[main]} / ${basic.invoke_string3_void.swig(lang:python)[main]},
    'vs swig, python, component void(string3)': ${basic.invoke_void_string3.joint(lang:python)[main]} / ${basic.invoke_void_string3.swig(lang:python)[main]},
    'vs swig, python, proxy void(string3)': ${basic.invokeOutgoing_void_string3.joint(lang:python)[main]} / ${basic.invoke_void_string3.swig(lang:python)[main]},
    'vs swig, python, component string100()': ${basic.invoke_string100_void.joint(lang:python)[main]} / ${basic.invoke_string100_void.swig(lang:python)[main]},
    'vs swig, python, proxy string100()': ${basic.invokeOutgoing_string100_void.joint(lang:python)[main]} / ${basic.invoke_string100_void.swig(lang:python)[main]},
    'vs swig, python, component void(string100)': ${basic.invoke_void_string100.joint(lang:python)[main]} / ${basic.invoke_void_string100.swig(lang:python)[main]},
    'vs swig, python, proxy void(string100)': ${basic.invokeOutgoing_void_string100.joint(lang:python)[main]} / ${basic.invoke_void_string100.swig(lang:python)[main]},
}


class Context:
    def __init__(self):
        self.num_errors = 0

    def error(self, msg):
        self.num_errors += 1
        print('{fg}{msg}{rs}'.format(msg=msg, fg=colorama.Fore.RED, rs=colorama.Style.RESET_ALL))

    def warning(self,msg):
        print('{fg}{msg}{rs}'.format(msg=msg, fg=colorama.Fore.YELLOW, rs=colorama.Style.RESET_ALL))

    def info(self,msg):
        print(msg)

    def ok(self,msg):
        print('{fg}{msg}{rs}'.format(msg=msg, fg=colorama.Fore.GREEN, rs=colorama.Style.RESET_ALL))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--canonize-keys', nargs='*', help='Keys for canonization')
    parser.add_argument('--canonize-new', action='store_true', help='Keys for canonization')
    parser.add_argument('--data', required=True, help='Canonized data path')
    args = parser.parse_args()

    ctx = Context()

    if args.canonize_keys or args.canonize_new:
        with open(args.data) as f:
            canon_data = json.loads(f.read())

        for key in args.canonize_keys or []:
            try:
                canon_val = canon_data[key]
            except KeyError:
                ctx.error('Key "{}" not present in canonized data'.format(key))
            else:
                canon_data[key] = data[key]
                ctx.info('Updated: "{}" ({} -> {})'.format(key, canon_val, data[key]))

        if args.canonize_new:
            added_keys = set(data.keys())
            added_keys.difference_update(canon_data.keys())

            removed_keys = set(canon_data)
            removed_keys.difference_update(data.keys())

            for key in removed_keys:
                canon_val = canon_data.pop(key)
                ctx.info('Removed: "{}" ({})'.format(key, canon_val))

            for key in added_keys:
                canon_data[key] = data[key]
                ctx.info('Added: "{}" ({})'.format(key, data[key]))

            if not added_keys and not removed_keys:
                ctx.info('No added or removed keys')

        if ctx.num_errors:
            ctx.error('{} errors!'.format(ctx.num_errors))
            exit(1)
        else:
            with open(args.data, 'w') as f:
                f.write(json.dumps(canon_data, indent=4, sort_keys=True))
            ctx.ok('OK')
    else:
        with open(args.data) as f:
            canon_data = json.loads(f.read())
            keys = set(canon_data.keys())
            keys.update(data.keys())
            for key in sorted(keys):
                if key not in canon_data:
                    ctx.error('New key: "{}"'.format(key))
                elif key not in data:
                    ctx.error('Missing key: "{}"'.format(key))
                else:
                    d = data[key]
                    c = canon_data[key]
                    msg = '"{}" (canonized: {}, actual: {})'.format(key, c, d)

                    if d > c * 1.5:
                        ctx.error('Slower: {}'.format(msg))
                    elif d > c * 1.2:
                        ctx.warning('A bit slower: {}'.format(msg))
                    elif d * 1.5 < c:
                        ctx.ok('Faster: {}'.format(msg))
                    else:
                        ctx.info('OK: {}'.format(msg))

        if ctx.num_errors:
            ctx.error('{} errors!'.format(ctx.num_errors))
            exit(1)
        else:
            ctx.ok('OK')


if (__name__ == '__main__'):
    main()
