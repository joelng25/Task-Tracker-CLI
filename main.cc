#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string FILE_NAME = "tasks.json";

int main(int argc, char* argv[]) {
  string desc;
  vector<string> status = {"todo", "in-progress", "done"};
  string createdAt;
  string updatedAt;

  if (argc == 1) return 0;

  string command = argv[1];
  desc = argv[2];
  time_t now = time(0);
  string dt = ctime(&now);
  dt.resize(dt.size() - 1);

  createdAt = updatedAt = dt;

  int pos = 0;
  string line;
  string content;

  // crea si no existe y escribe al final
  ifstream task_file(FILE_NAME);
  if (task_file.fail()) {
    ofstream task_file(FILE_NAME);
  } else {
    while (getline(task_file, line)) {
      content += line + "\n";
      ++pos;
    }
    task_file.close();
  }

  if (command == "add" && argc > 2) {
    if (pos == 0) {
      ofstream first_task(FILE_NAME, ios::app);
      first_task << "[" << endl;

      first_task << "{\"id\":" << pos + 1 << ", \"desc\":\"" << desc
                 << "\", \"status\":\"" << status[0] << "\", \"createdAt\":\""
                 << createdAt << "\", \"updatedAt\":\"" << updatedAt << "\"}"
                 << endl;

      first_task << "]" << endl;
    } else {
      size_t end_bracket = content.rfind("]");
      ofstream new_task(FILE_NAME, ios::trunc);
      new_task << content.substr(0, end_bracket - 1);

      new_task << "," << endl;

      new_task << "{\"id\":" << pos - 1 << ", \"desc\":\"" << desc
               << "\", \"status\":\"" << status[0] << "\", \"createdAt\":\""
               << createdAt << "\", \"updatedAt\":\"" << updatedAt << "\"}"
               << endl;

      new_task << "]" << endl;

      new_task.close();
    }
  }

  return 0;
}