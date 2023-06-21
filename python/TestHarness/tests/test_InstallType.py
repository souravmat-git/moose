#* This file is part of the MOOSE framework
#* https://www.mooseframework.org
#*
#* All rights reserved, see COPYRIGHT for full restrictions
#* https://github.com/idaholab/moose/blob/master/COPYRIGHT
#*
#* Licensed under LGPL 2.1, please see LICENSE for details
#* https://www.gnu.org/licenses/lgpl-2.1.html


from TestHarnessTestCase import TestHarnessTestCase
import os, io
import mock
import TestHarness
from contextlib import redirect_stdout

class TestHarnessTester(TestHarnessTestCase):
    @mock.patch.object(TestHarness.util, 'checkInstalled')
    def mocked_output(self, mocked, expect_fail, mocked_return):
        MOOSE_DIR = os.getenv('MOOSE_DIR')
        os.chdir(f'{MOOSE_DIR}/test')
        out = io.StringIO()
        with redirect_stdout(out):
            mocked_return.return_value=mocked
            harness = TestHarness.TestHarness(['', '-i', 'install_type', '-c'], MOOSE_DIR)
            if expect_fail:
                with self.assertRaises(SystemExit):
                    harness.findAndRunTests()
            else:
                harness.findAndRunTests()
        return out.getvalue()

    def testInstalled(self):
        """
        Test which only runs if binary is installed
        """
        out = self.mocked_output(set(['ALL', 'INSTALLED']), False)
        # Mock objects do not inherit MOOSE_TERM_FORMAT options set forth in TestHarnesTestCase.py.
        # In order to pass Civet, and on folks machines, do separate searches
        self.assertRegex(out, r'^(?=.*in_tree_type)(?=.*SKIP)(?=.*\[test requires "IN_TREE" binary\])')
        self.assertRegex(out, r'(?=.*installed_type)(?=.*OK)')
        self.assertRegex(out, r'(?=.*all_type)(?=.*OK)')

    def testInTree(self):
        """
        Test which only runs if binary is in_tree
        """
        out = self.mocked_output(set(['ALL', 'IN_TREE']), False)
        # Mock objects do not inherit MOOSE_TERM_FORMAT options set forth in TestHarnesTestCase.py.
        # In order to pass Civet, and on folks machines, do separate searches
        self.assertRegex(out, r'^(?=.*installed_type)(?=.*SKIP)(?=.*\[test requires "INSTALLED" binary\])')
        self.assertRegex(out, r'(?=.*in_tree_type)(?=.*OK)')
        self.assertRegex(out, r'(?=.*all_type)(?=.*OK)')
