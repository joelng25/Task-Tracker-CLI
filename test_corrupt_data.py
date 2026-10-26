import os, subprocess, tempfile, unittest
from pathlib import Path
BIN=os.environ.get('TASK_CLI_BIN',str(Path(__file__).parent/'task-cli'))
class Persistence(unittest.TestCase):
 def test_corrupt_data_preserved(self):
  for contents in ['{broken','{}','[{"id":999999999999999999999999}]']:
   with tempfile.TemporaryDirectory() as d:
    p=Path(d,'tasks.json');p.write_text(contents)
    r=subprocess.run([BIN,'add','no overwrite'],cwd=d,capture_output=True)
    self.assertNotEqual(r.returncode,0);self.assertEqual(p.read_text(),contents)
 def test_failed_save_preserves_original(self):
  with tempfile.TemporaryDirectory() as d:
   p=Path(d,'tasks.json');p.write_text('[]');Path(d,'tasks.json.tmp').mkdir()
   r=subprocess.run([BIN,'add','not saved'],cwd=d,capture_output=True)
   self.assertNotEqual(r.returncode,0);self.assertEqual(p.read_text(),'[]')
 def test_recovery(self):
  with tempfile.TemporaryDirectory() as d:
   p=Path(d,'tasks.json');p.write_text('[]')
   self.assertEqual(subprocess.run([BIN,'add','recovered'],cwd=d,capture_output=True).returncode,0)
   self.assertIn('recovered',p.read_text())
if __name__=='__main__':unittest.main()
