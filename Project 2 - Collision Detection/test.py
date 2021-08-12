#!/usr/bin/python

"""Test runner for screen saver executables."""

__author__ = 'Simon Aertssen'

import re
import os
import sys
import time
import subprocess


BINARY = './screensaver'
QUIET = True


def wait_for_test_process(proc, timeout):
    """Wait for a test process with a timeout, and reads stderr.

    Reading stderr while polling is important, or a deadlock can occur if the
    pipe's internal buffer fills up.
    """
    endtime = time.time() + timeout
    err_chunks = []
    while proc.returncode is None and time.time() < endtime:
        time.sleep(0.1)
        err_chunks.append(os.read(proc.stderr.fileno(), 4096))
        proc.poll()

    # Kill the child if it hasn't stopped yet, and wait for it.
    timed_out = False
    if proc.returncode is None:
        proc.kill()
        proc.wait()
        timed_out = True
        print(f'Test process timed out after {timeout}s...')

    # Read the rest of stderr.
    chunk = True
    while chunk:
        chunk = os.read(proc.stderr.fileno(), 4096)
        err_chunks.append(chunk)
    lines = ''.join(err_chunks).split('\n')
    return (timed_out, lines)


def run_binary(binary, frames, input_file):
    timeout = 30.0
    num_timeouts = 0

    done_testing = False
    # Run the binary in a subprocess
    while not done_testing:
        with open(os.devnull) as null:
            proc = subprocess.Popen([binary, input_file],
                                    stdout=null, stderr=subprocess.PIPE)
            (timed_out, lines) = wait_for_test_process(proc, timeout)

        # If there was a timeout, try it again
        if timed_out:
            if num_timeouts > 3:
                raise TimeoutError(f'{input_file} timed out {num_timeouts} times.')
            timeout += 10
            num_timeouts += 1
        elif proc.returncode != 0:
            raise ValueError(f'{input_file} returned {proc.returncode} error code.')

        # Process the output lines
        numbers = re.findall(r"[-+]?\d*\.?\d+", lines)
        print(numbers)
        if len(numbers) > 4:
            print("Found more than 4 integers in this output.")
            numbers = numbers[0:4]
    return numbers[1:]  # Drop the frame number


def produce_test_table(frames=1000):
    output_filename = f'../test_output/test_table_{frames}.txt'
    output_file = open(output_filename, "w")

    output = '| Animation Name | Execution Time [s]| Line-Wall Collisions | Line-Line Collisions |\n'
    output += '| - | - | - | - |\n'

    input_directory = '../screensaver/input/'
    for filename in os.listdir(input_directory):
        try:
            exc_time, l_w_coll, l_l_coll = run_binary(BINARY, frames, input_directory + filename)
        except Exception:
            exc_time, l_w_coll, l_l_coll = '-', '_', '_'

        output += f'| {filename} | {exc_time} s | {l_w_coll} | {l_l_coll} |\n'
    output_file.write(output)
    output_file.close()


def test_correctness():
    total_tests = 0
    total_passes = 0
    total_failed = 0
    for binary in binaries:
        (num_tests, num_passes, num_failed) = test_project1(binary)
        total_tests += num_tests
        total_passes += num_passes
        total_failed += num_failed
    print(('Ran %d test functions, %d individual tests passed, '
           '%d individual tests failed.' % (total_tests-1, total_passes,
                                            total_failed)))
    print_result(total_failed == 0)


def main(args):
    QUIET = True if '--quiet' in args else False

    if '--table' in args:
        produce_test_table()
    elif '--test' in args:
        test_correctness()

if __name__ == '__main__':
    main(sys.argv)
