#!/usr/bin/python

"""Test runner for screen saver executables."""

__author__ = 'Simon Aertssen'

import re
import os
import sys
import time
import subprocess


BINARY = './screensaver'


def wait_for_test_process(proc, timeout):
    """Wait for a test process with a timeout, and reads stderr.

    Reading stderr while polling is important, or a deadlock can occur if the
    pipe's internal buffer fills up.
    """
    endtime = time.time() + timeout
    err_chunks = []
    while proc.returncode is None and time.time() < endtime:
        time.sleep(0.1)
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

    # Check if there are any errors registered
    if len(err_chunks) > 1 and err_chunks[0] != b'':
        lines = b''.join(err_chunks)

    # If not, then read from the stdout
    lines = b''.join(proc.stdout.readlines())
    return (timed_out, str(lines))


def run_binary(binary, frames, input_file):
    timeout = 60*60.0
    print(f'Timeout = {timeout}')
    done_testing = False
    # Run the binary in a subprocess
    while not done_testing:
        proc = subprocess.Popen([binary, str(frames), input_file],
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        (timed_out, lines) = wait_for_test_process(proc, timeout)

        # If there was a timeout, try it again
        if timed_out:
            raise TimeoutError(f'Animation {input_file} timed out {num_timeouts} times.')
        elif proc.returncode != 0:
            raise ValueError(f'{input_file} returned {proc.returncode} error code.')

        # Process the output lines
        numbers = re.findall(r"[-+]?\d*\.?\d+", lines)
        if len(numbers) > 4:
            print("Found more than 4 integers in this output.")
            numbers = numbers[0:4]
        done_testing = True
    return numbers[1:]  # Drop the frame number


def produce_test_table(frames=10, verbose=False):
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
        current_output = f'| {filename} | {exc_time} s | {l_w_coll} | {l_l_coll} |\n'
        output += current_output
        if verbose:
            print(current_output)
    output_file.write(output)
    output_file.close()


def test_correctness(frames):
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
    verbose = False if '--quiet' in args else True
    frames = 1000
    if '--table' in args:
        index = args.index('--table') + 1
        frames = args[index]
        produce_test_table(frames, verbose)
    elif '--test' in args:
        test_correctness(frames, verbose)


if __name__ == '__main__':
    main(sys.argv)
