#include <iostream>
#include <string>
#include "task_manager.h"
#include "task_id.h"
using namespace std;
const string FILE_NAME = "tasks.json";
void displayHelp() {
  cout << "\nTASK CLI\n\nUSAGE: ./task-cli <command> [arguments]\n\n"
       << "  add \"<desc>\"          Add a new task\n"
       << "  update <id> \"<desc>\"  Update a task description\n"
       << "  delete <id>           Remove a task\n"
       << "  mark-in-progress <id> Set a task to in-progress\n"
       << "  mark-done <id>        Set a task to done\n"
       << "  mark-todo <id>        Set a task back to todo\n"
       << "  list [todo|in-progress|done]\n\n"
       << "EXAMPLES:\n  ./task-cli add \"Finish project report\"\n  ./task-cli mark-done 1\n";
}
void displayTask(const Task& task) {
  cout << "ID: " << task.id << " | Status: " << task.status << " | " << task.desc << endl;
}
int main(int argc, char* argv[]) {
  if (argc == 1) {
    cerr << "USAGE: task-cli <command> [arguments]\nFOR HELP: task-cli help\n";
    return 1;
  }
  string command = argv[1];
  TaskManager tm(FILE_NAME);
  if (command == "help") { displayHelp(); return 0; }
  if (command == "add") {
    if (argc != 3) { cerr << "Usage: task-cli add \"<description>\"\n"; return 1; }
    if (!tm.addTask(argv[2])) { cerr << "Error adding task\n"; return 1; }
    cout << "Task added successfully!\n";
    return 0;
  }
  if (command == "update" || command == "delete" || command == "mark-in-progress" || command == "mark-done" || command == "mark-todo") {
    if (argc != (command == "update" ? 4 : 3)) {
      cerr << "Usage: task-cli " << command << " <id>" << (command == "update" ? " \"<description>\"" : "") << endl;
      return 1;
    }
    try {
      int id = parseTaskId(argv[2]);
      bool success = command == "update" ? tm.updateTask(id, argv[3])
        : command == "delete" ? tm.deleteTask(id)
        : command == "mark-in-progress" ? tm.markInProgress(id)
        : command == "mark-done" ? tm.markDone(id) : tm.markTodo(id);
      if (!success) { cerr << "Task with ID " << id << " could not be changed (not found or save failed)\n"; return 1; }
      if (command == "update") cout << "Task updated successfully!\n";
      else if (command == "delete") cout << "Task deleted successfully!\n";
      else cout << "Task marked as " << (command == "mark-done" ? "done" : command == "mark-todo" ? "todo" : "in-progress") << "!\n";
    } catch (const invalid_argument&) { cerr << "Error: ID must be a positive integer\n"; return 1; }
    return 0;
  }
  if (command == "list") {
    vector<Task> taskList;
    if (argc == 3) {
      string filter = argv[2];
      if (filter == "todo") taskList = tm.getTodoTasks();
      else if (filter == "in-progress") taskList = tm.getInProgressTasks();
      else if (filter == "done") taskList = tm.getDoneTasks();
      else { cerr << "Unknown filter: " << filter << endl; return 1; }
    } else if (argc == 2) taskList = tm.getAllTasks();
    else { cerr << "Usage: task-cli list [todo|in-progress|done]\n"; return 1; }
    if (taskList.empty()) cout << "No tasks found.\n";
    else for (const auto& task : taskList) displayTask(task);
    return 0;
  }
  cerr << "Unknown command: " << command << "\nFOR HELP: task-cli help\n";
  return 1;
}
