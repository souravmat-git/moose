#* This file is part of the MOOSE framework
#* https://www.mooseframework.org
#*
#* All rights reserved, see COPYRIGHT for full restrictions
#* https://github.com/idaholab/moose/blob/master/COPYRIGHT
#*
#* Licensed under LGPL 2.1, please see LICENSE for details
#* https://www.gnu.org/licenses/lgpl-2.1.html

import os, sys, io
import unittest
import mock
import TestHarness
from contextlib import redirect_stdout

class TestHarnessTester(unittest.TestCase):
    @mock.patch.object(TestHarness.util, 'getMachine')
    def mocked_output(self, mocked, expect_fail, mocked_return):
        MOOSE_DIR = os.getenv('MOOSE_DIR')
        os.chdir(f'{MOOSE_DIR}/test')
        out = io.StringIO()
        with redirect_stdout(out):
            mocked_return.return_value=mocked
            harness = TestHarness.TestHarness(['', '-i', 'always_ok', '-c'], MOOSE_DIR)
            if expect_fail:
                with self.assertRaises(SystemExit):
                    harness.findAndRunTests()
            else:
                harness.findAndRunTests()
        return out.getvalue()

    def testNotSkipped(self):
        """
        Test should not be skipped, as it is set to run on any arch (ALL)
        """
        out = self.mocked_output(set(['ALL']), False)
        self.assertRegex(out, r'.*?OK.*?always_ok')

    def testSkipped(self):
        """
        Test that a non existing machine type is skipped (remove default of ALL)
        """
        out = self.mocked_output(set(['']), False)
        self.assertRegex(out, r'.*?SKIP.*?always_ok.*?MACHINE!=ALL')
