#include "task_manager.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
using namespace std;
json Task::toJson() const {
  return {{"id", id}, {"desc", desc}, {"status", status}, {"createdAt", createdAt}, {"updatedAt", updatedAt}};
}
Task Task::fromJson(const json& j) {
  Task t;
  t.id = j["id"]; t.desc = j["desc"]; t.status = j["status"];
  t.createdAt = j["createdAt"]; t.updatedAt = j["updatedAt"];
  return t;
}
TaskManager::TaskManager(const string& filename) : filename(filename), nextId(1) { loadTasks(); }
TaskManager::~TaskManager() = default;
string TaskManager::getCurrentTimestamp() const {
  time_t now = time(0);
  string dt = ctime(&now);
  dt.resize(dt.size() - 1);
  return dt;
}
int TaskManager::getMaxId() const {
  int maxId = 0;
  for (const auto& task : tasks) if (task.id > maxId) maxId = task.id;
  return maxId;
}
void TaskManager::loadTasks() {
  ifstream task_file(filename);
  if (task_file.fail()) {
    ofstream new_file(filename); new_file << "[]" << endl; new_file.close();
    nextId = 1; return;
  }
  try {
    json loaded; task_file >> loaded;
    if (loaded.is_array()) for (const auto& j : loaded) tasks.push_back(Task::fromJson(j));
    nextId = getMaxId() + 1;
  } catch (const exception& e) {
    cerr << "Error parsing JSON: " << e.what() << endl;
    tasks.clear(); nextId = 1;
  }
}
bool TaskManager::saveTasks() {
  try {
    json j = json::array();
    for (const auto& task : tasks) j.push_back(task.toJson());
    ofstream out(filename); out << j.dump(2) << endl; out.close();
    if (!out) { cerr << "Error saving tasks: write failed" << endl; return false; }
    return true;
  } catch (const exception& e) { cerr << "Error saving tasks: " << e.what() << endl; return false; }
}
bool TaskManager::addTask(const string& desc) {
  const auto previousTasks = tasks;
  const int previousNextId = nextId;
  try {
    Task newTask; newTask.id = nextId++; newTask.desc = desc; newTask.status = "todo";
    newTask.createdAt = getCurrentTimestamp(); newTask.updatedAt = newTask.createdAt;
    tasks.push_back(newTask);
    if (saveTasks()) return true;
  } catch (const exception& e) { cerr << "Error adding task: " << e.what() << endl; }
  tasks = previousTasks; nextId = previousNextId; return false;
}
bool TaskManager::updateTask(int id, const string& new_desc) {
  Task* task = findTaskById(id); if (!task) return false;
  const auto previousTasks = tasks;
  task->desc = new_desc; task->updatedAt = getCurrentTimestamp();
  if (saveTasks()) return true;
  tasks = previousTasks; return false;
}
bool TaskManager::deleteTask(int id) {
  auto it = find_if(tasks.begin(), tasks.end(), [id](const Task& t) { return t.id == id; });
  if (it == tasks.end()) return false;
  const auto previousTasks = tasks; tasks.erase(it);
  if (saveTasks()) return true;
  tasks = previousTasks; return false;
}
bool TaskManager::markInProgress(int id) {
  Task* task = findTaskById(id); if (!task) return false;
  const auto previousTasks = tasks; task->status = "in-progress"; task->updatedAt = getCurrentTimestamp();
  if (saveTasks()) return true;
  tasks = previousTasks; return false;
}
bool TaskManager::markDone(int id) {
  Task* task = findTaskById(id); if (!task) return false;
  const auto previousTasks = tasks; task->status = "done"; task->updatedAt = getCurrentTimestamp();
  if (saveTasks()) return true;
  tasks = previousTasks; return false;
}
bool TaskManager::markTodo(int id) {
  Task* task = findTaskById(id); if (!task) return false;
  const auto previousTasks = tasks; task->status = "todo"; task->updatedAt = getCurrentTimestamp();
  if (saveTasks()) return true;
  tasks = previousTasks; return false;
}
vector<Task> TaskManager::getAllTasks() const { return tasks; }
vector<Task> TaskManager::getTodoTasks() const {
  vector<Task> result; copy_if(tasks.begin(), tasks.end(), back_inserter(result), [](const Task& t) { return t.status == "todo"; }); return result;
}
vector<Task> TaskManager::getInProgressTasks() const {
  vector<Task> result; copy_if(tasks.begin(), tasks.end(), back_inserter(result), [](const Task& t) { return t.status == "in-progress"; }); return result;
}
vector<Task> TaskManager::getDoneTasks() const {
  vector<Task> result; copy_if(tasks.begin(), tasks.end(), back_inserter(result), [](const Task& t) { return t.status == "done"; }); return result;
}
Task* TaskManager::findTaskById(int id) {
  auto it = find_if(tasks.begin(), tasks.end(), [id](Task& t) { return t.id == id; }); return it != tasks.end() ? &(*it) : nullptr;
}
const Task* TaskManager::findTaskById(int id) const {
  auto it = find_if(tasks.begin(), tasks.end(), [id](const Task& t) { return t.id == id; }); return it != tasks.end() ? &(*it) : nullptr;
}
