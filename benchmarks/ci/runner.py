#!/usr/bin/python

from collections import defaultdict, namedtuple

import argparse
import colorama
import json
import subprocess
import traceback


ResultEntry = namedtuple("ResultEntry", "current, reference, error")


def run(executable, num_passes, id, lang):
    out_json = subprocess.check_output([executable, '-j', '-c', str(num_passes), '-b', id, '--params', 'lang:{}'.format(lang)])
    out = json.loads(out_json)
    return out['times']['main']


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
    parser = argparse.ArgumentParser(description='Joint adapters generator')
    parser.add_argument('--executable', help='joint-benchmarks executable', required=True)
    parser.add_argument('--reference-executable', help='joint-benchmarks reference executable', required=True)
    parser.add_argument('--benchmarks', help='benchmarks.json file', required=True)
    parser.add_argument('--num-passes', help='number of joint-benchmarks passes required to measure performance', type=int, default=1)
    args = parser.parse_args()

    ctx = Context()

    with open(args.benchmarks) as benchmarks_file:
        benchmarks = json.loads(benchmarks_file.read())

    result = defaultdict(lambda: {})

    has_errors = False
    current_number = 1
    total_count = sum(len(ids) for ids in benchmarks.values())
    for lang in sorted(benchmarks):
        for id in benchmarks[lang]:
            ctx.info('{}/{}: {}, {}'.format(current_number, total_count, lang, id))
            current_number += 1

            try:
                current = run(args.executable, args.num_passes, id, lang)
                reference = run(args.reference_executable, args.num_passes, id, lang)
                result[lang][id] = ResultEntry(reference=reference, current=current, error=None)
            except subprocess.CalledProcessError:
                has_errors = True
                result[lang][id] = ResultEntry(reference=None, current=None, error=traceback.format_exc())

    for lang in sorted(result):
        lang_result = result[lang]
        for id in sorted(lang_result):
            entry = lang_result[id]
            if entry.error:
                ctx.error('{}(lang:{}):\n{}'.format(id, lang, entry.error))
            else:
                def msg(text):
                    return '{}(lang:{}): {} {} -> {} ({:.2f})'.format(id, lang, text, entry.reference, entry.current, float(entry.current) / entry.reference)

                if entry.current < entry.reference * 0.75:
                    ctx.ok(msg('FASTER'))
                elif entry.current < entry.reference * 1.07:
                    ctx.info(msg('OK'))
                elif entry.current < entry.reference * 1.15:
                    ctx.warning(msg('A BIT SLOWER'))
                else:
                    ctx.error(msg('SLOWER'))

    if ctx.num_errors:
        ctx.error('{} errors!'.format(ctx.num_errors))

    return 1 if ctx.num_errors else 0


if __name__ == '__main__':
    exit(main())
