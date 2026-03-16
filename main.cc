#include <iostream>
#include <string>

#include "task_manager.h"

using namespace std;

const string FILE_NAME = "tasks.json";

void displayHelp() {
  cout << "\n======================================\n";
  cout << "        TASK CLI - HELP MENU          \n";
  cout << "======================================\n\n";

  cout << "USAGE:\n";
  cout << "  ./task-cli <command> [arguments]\n\n";

  cout << "COMMANDS:\n";
  cout << "  add \"<desc>\"          Add a new task\n";
  cout << "  update <id> \"<desc>\"  Update an existing task description\n";
  cout << "  delete <id>           Remove a task\n";
  cout << "  mark-in-progress <id> Set a task to 'in-progress'\n";
  cout << "  mark-done <id>        Set a task to 'done'\n\n";

  cout << "LISTING:\n";
  cout << "  list                  List all tasks\n";
  cout << "  list todo             List only tasks to be done\n";
  cout << "  list in-progress      List only tasks currently in progress\n";
  cout << "  list done             List only completed tasks\n\n";

  cout << "EXAMPLES:\n";
  cout << "  ./task-cli add \"Finish project report\"\n";
  cout << "  ./task-cli mark-done 1\n";
  cout << "======================================\n" << endl;
}

void displayTask(const Task& task) {
  cout << "ID: " << task.id << " | Status: " << task.status << " | " << task.desc << endl;
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    cerr << "USAGE: task-cli <command> [arguments]" << endl;
    cerr << "FOR HELP: task-cli help" << endl;
    return 1;
  }

  string command = argv[1];
  TaskManager tm(FILE_NAME);

  // Help command
  if (command == "help") {
    displayHelp();
    return 0;
  }

  // Add command
  if (command == "add") {
    if (argc != 3) {
      cerr << "Usage: task-cli add \"<description>\"" << endl;
      return 1;
    }

    if (!tm.addTask(argv[2])) {
      cerr << "Error adding task" << endl;
      return 1;
    }

    cout << "Task added successfully!" << endl;
    return 0;
  }

  // Update command
  if (command == "update") {
    if (argc != 4) {
      cerr << "Usage: task-cli update <id> \"<description>\"" << endl;
      return 1;
    }

    try {
      int id = stoi(argv[2]);
      if (!tm.updateTask(id, argv[3])) {
        cerr << "Task with ID " << id << " not found" << endl;
        return 1;
      }
      cout << "Task updated successfully!" << endl;
    } catch (const invalid_argument&) {
      cerr << "Error: ID must be a number" << endl;
      return 1;
    }
    return 0;
  }

  // Delete command
  if (command == "delete") {
    if (argc != 3) {
      cerr << "Usage: task-cli delete <id>" << endl;
      return 1;
    }

    try {
      int id = stoi(argv[2]);
      if (!tm.deleteTask(id)) {
        cerr << "Task with ID " << id << " not found" << endl;
        return 1;
      }
      cout << "Task deleted successfully!" << endl;
    } catch (const invalid_argument&) {
      cerr << "Error: ID must be a number" << endl;
      return 1;
    }
    return 0;
  }

  // Mark in-progress command
  if (command == "mark-in-progress") {
    if (argc != 3) {
      cerr << "Usage: task-cli mark-in-progress <id>" << endl;
      return 1;
    }

    try {
      int id = stoi(argv[2]);
      if (!tm.markInProgress(id)) {
        cerr << "Task with ID " << id << " not found" << endl;
        return 1;
      }
      cout << "Task marked as in-progress!" << endl;
    } catch (const invalid_argument&) {
      cerr << "Error: ID must be a number" << endl;
      return 1;
    }
    return 0;
  }

  // Mark done command
  if (command == "mark-done") {
    if (argc != 3) {
      cerr << "Usage: task-cli mark-done <id>" << endl;
      return 1;
    }

    try {
      int id = stoi(argv[2]);
      if (!tm.markDone(id)) {
        cerr << "Task with ID " << id << " not found" << endl;
        return 1;
      }
      cout << "Task marked as done!" << endl;
    } catch (const invalid_argument&) {
      cerr << "Error: ID must be a number" << endl;
      return 1;
    }
    return 0;
  }

  // List commands
  if (command == "list") {
    vector<Task> taskList;

    if (argc == 3) {
      string filter = argv[2];
      if (filter == "todo") {
        taskList = tm.getTodoTasks();
      } else if (filter == "in-progress") {
        taskList = tm.getInProgressTasks();
      } else if (filter == "done") {
        taskList = tm.getDoneTasks();
      } else {
        cerr << "Unknown filter: " << filter << endl;
        return 1;
      }
    } else if (argc == 2) {
      taskList = tm.getAllTasks();
    } else {
      cerr << "Usage: task-cli list [todo|in-progress|done]" << endl;
      return 1;
    }

    if (taskList.empty()) {
      cout << "No tasks found." << endl;
    } else {
      for (const auto& task : taskList) {
        displayTask(task);
      }
    }
    return 0;
  }

  // Unknown command
  cerr << "Unknown command: " << command << endl;
  cerr << "FOR HELP: task-cli help" << endl;
  return 1;
}