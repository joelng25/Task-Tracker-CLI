import json
from pathlib import Path
import subprocess
import tempfile
import unittest
BINARY = Path(__file__).resolve().parent / "task-cli"
class CliRegressionTest(unittest.TestCase):
    def test_invalid_ids_never_change_existing_task(self):
        with tempfile.TemporaryDirectory() as directory:
            def run(*args):
                return subprocess.run([str(BINARY), *args], cwd=directory, capture_output=True, text=True)
            self.assertEqual(run("add", "Keep me").returncode, 0)
            original = (Path(directory) / "tasks.json").read_bytes()
            for command in ["update", "delete", "mark-done", "mark-in-progress", "mark-todo"]:
                for task_id in ["1xyz", "999999999999999999999", "0", "-1"]:
                    args = [command, task_id] + (["changed"] if command == "update" else [])
                    with self.subTest(command=command, task_id=task_id):
                        self.assertEqual(run(*args).returncode, 1)
                        self.assertEqual((Path(directory) / "tasks.json").read_bytes(), original)
            self.assertEqual(run("mark-done", "1").returncode, 0)
            self.assertEqual(json.loads((Path(directory) / "tasks.json").read_text())[0]["status"], "done")
    def test_failed_save_does_not_report_success(self):
        with tempfile.TemporaryDirectory() as directory:
            (Path(directory) / "tasks.json").mkdir()
            result = subprocess.run([str(BINARY), "add", "Cannot save"], cwd=directory, capture_output=True, text=True)
            self.assertEqual(result.returncode, 1)
            self.assertIn("Error saving tasks", result.stderr)
if __name__ == "__main__":
    unittest.main()
