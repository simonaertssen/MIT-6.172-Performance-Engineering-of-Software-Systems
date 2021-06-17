#!/usr/bin/python

"""Test runner for 6.172, fall 2010.

TODO(rnk): Fill out the docstring comment with usage.
"""

from __future__ import with_statement

__author__ = 'Reid Kleckner <rnk@mit.edu>'

import difflib
import os
import re
import subprocess
import sys
import time


GREEN = '\033[92;1m'
RED = '\033[91;1m'
END = '\033[0m'


QUIET = False

def print_result(result):
    """If result is True, print a green PASSED or red FAILED line otherwise."""
    if result:
        print GREEN + 'PASSED' + END
    else:
        print RED + 'FAILED' + END


def wait_for_test_process(proc, timeout):
    """Waits for a test process with a timeout, and reads stderr.

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
        print 'Test process timed out after 30s...'

    # Read the rest of stderr.
    chunk = True
    while chunk:
        chunk = os.read(proc.stderr.fileno(), 4096)
        err_chunks.append(chunk)
    lines = ''.join(err_chunks).split('\n')
    return (timed_out, lines)


def test_project1(binary):
    """Test runner for project1 problems.

    Apologies for the complicatedness of the testing protocol, and also of how
    hard it is to properly run a process with a timeout.
    """
    testdir = os.path.join(os.path.dirname(binary), 'tests')
    test_files = [os.path.join(testdir, file) for file in os.listdir(testdir)]

    num_passed = 0
    num_failed = 0
    test_index_total = 0
    for filename in test_files:
      testfileNameBegin = len(filename) - filename[::-1].index('/')
      if filename[testfileNameBegin] == '.':
        print "Skipping file beginning with '.'"
        continue
      done_testing = False
      test_index = 0
      while not done_testing:
        with open(os.devnull) as null:
            proc = subprocess.Popen([binary, '-t', filename],
                                    stdout=null, stderr=subprocess.PIPE)
            (timed_out, lines) = wait_for_test_process(proc, 30.0)

        # Interpret each line.
        for lineIndex in range(0, len(lines)):
            line = lines[lineIndex]
            lineIndex += 1;
            match = re.match('Running test #(\d+)\.\.\.', line)
            if match:
                test_index = int(match.group(1))
            match = re.match(' --> (.*): (PASS|FAIL)', line)
            if match:
                passed = match.group(2) == 'PASS'
                if passed:
                    num_passed += 1
                else:
                    num_failed += 1
                # NOTE(TFK): Added 'or' to print failed tests.
                if not QUIET or (QUIET and not passed):
                    testname = os.path.basename(binary) + ' ' + match.group(1)
                    print testname.ljust(64),
                    print_result(passed)
                if not passed:
                    print lines[lineIndex];
                    lineIndex += 1;
                    print lines[lineIndex];
                    lineIndex += 1;
                    print lines[lineIndex];
                    lineIndex += 1;
            if line.startswith('Done testing'):
                test_index_total += (test_index + 1)
                done_testing = True

        # If there was a timeout, skip the last test.
        if timed_out:
            test_index += 1
            num_failed += 1
        elif proc.returncode != 0:
            print 'Nonzero return code.'
            done_testing = True
            test_index += 1
            num_failed += 1

    # NOTE(TFK): No need for this when we're printing failed tests above.
    if QUIET and False:
        testname = os.path.basename(binary)
        print testname.ljust(64),
        print_result(num_failed == 0)

    return (test_index_total, num_passed, num_failed)


def main(argv):
    if len(argv) < 2:
        print 'Usage: test.py [--quiet] <binary> ...'
        sys.exit(1)
    args = argv[1:]
    if '--quiet' in args:
        global QUIET
        args.remove('--quiet')
        QUIET = True
    binaries = [os.path.join(os.getcwd(), arg) for arg in args]
    total_tests = 0
    total_passes = 0
    total_failed = 0
    for binary in binaries:
        (num_tests, num_passes, num_failed) = test_project1(binary)
        total_tests += num_tests
        total_passes += num_passes
        total_failed += num_failed
    print ('Ran %d test functions, %d individual tests passed, '
           '%d individual tests failed.' % (total_tests-1, total_passes,
                                            total_failed))
    print_result(total_failed == 0)


if __name__ == '__main__':
    main(sys.argv)
