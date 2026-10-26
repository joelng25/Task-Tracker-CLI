#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H
#include <string>
#include <vector>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;
struct Task {
  int id;
  string desc;
  string status;
  string createdAt;
  string updatedAt;
  json toJson() const;
  static Task fromJson(const json& j);
};
class TaskManager {
 public:
  TaskManager(const string& filename);
  ~TaskManager();
  bool addTask(const string& desc);
  bool updateTask(int id, const string& new_desc);
  bool deleteTask(int id);
  bool markInProgress(int id);
  bool markDone(int id);
  bool markTodo(int id);
  vector<Task> getAllTasks() const;
  vector<Task> getTodoTasks() const;
  vector<Task> getInProgressTasks() const;
  vector<Task> getDoneTasks() const;
  Task* findTaskById(int id);
  const Task* findTaskById(int id) const;
 private:
  string filename;
  vector<Task> tasks;
  int nextId;
  void loadTasks();
  bool saveTasks();
  string getCurrentTimestamp() const;
  int getMaxId() const;
};
#endif
