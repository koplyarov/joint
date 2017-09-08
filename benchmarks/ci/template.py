#!/usr/bin/python

import argparse
import json


data = {
    'cpp: component void()': ${basic.invoke_void_void.joint(lang:cpp)[main]} / ${basic.invokeNative_void_void.joint(lang:cpp)[main]},
    'cpp: proxy void()': ${basic.invokeOutgoing_void_void.joint(lang:cpp)[main]} / ${basic.invokeNative_void_void.joint(lang:cpp)[main]},
    'cpp: component i32()': ${basic.invoke_i32_void.joint(lang:cpp)[main]} / ${basic.invokeNative_i32_void.joint(lang:cpp)[main]},
    'cpp: proxy i32()': ${basic.invokeOutgoing_i32_void.joint(lang:cpp)[main]} / ${basic.invokeNative_i32_void.joint(lang:cpp)[main]},
    'cpp: component void(i32)': ${basic.invoke_void_i32.joint(lang:cpp)[main]} / ${basic.invokeNative_void_i32.joint(lang:cpp)[main]},
    'cpp: proxy void(i32)': ${basic.invokeOutgoing_void_i32.joint(lang:cpp)[main]} / ${basic.invokeNative_void_i32.joint(lang:cpp)[main]},
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
        errors = []
        with open(args.data) as f:
            canon_data = json.loads(f.read())
            keys = set(canon_data.keys())
            keys.update(data.keys())
            for key in keys:
                if key not in canon_data:
                    errors.append('New key: {}'.format(key))
                elif key not in data:
                    errors.append('Missing key: {}'.format(key))
                elif data[key] > canon_data[key] * 1.1:
                    errors.append('Slower key: {} (canonized: {}, actual: {})'.format(key, data[key], canon_data[key]))

            if errors:
                for e in errors:
                    print(e)
                exit(1)


if (__name__ == '__main__'):
    main()
