#include "task_manager.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <set>
#include <climits>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;
json Task::toJson() const {return {{"id",id},{"desc",desc},{"status",status},{"createdAt",createdAt},{"updatedAt",updatedAt}};}
Task Task::fromJson(const json& j) {Task t;t.id=j["id"];t.desc=j["desc"];t.status=j["status"];t.createdAt=j["createdAt"];t.updatedAt=j["updatedAt"];return t;}
TaskManager::TaskManager(const string& filename):filename(filename),nextId(1){loadTasks();}
TaskManager::~TaskManager()=default;
string TaskManager::getCurrentTimestamp()const{time_t now=time(0);string dt=ctime(&now);dt.resize(dt.size()-1);return dt;}
int TaskManager::getMaxId()const{int maxId=0;for(const auto& task:tasks)if(task.id>maxId)maxId=task.id;return maxId;}
void TaskManager::loadTasks(){
 ifstream in(filename);
 if(!in){if(filesystem::exists(filename))throw runtime_error("Cannot read task data; original file preserved");return;}
 try{
  json loaded;in>>loaded;if(!loaded.is_array())throw runtime_error("Task data must be an array");
  vector<Task> parsed;set<int> ids;
  for(const auto& row:loaded){
   if(!row["id"].is_number_integer()||row["id"].get<long long>()>INT_MAX)throw runtime_error("Invalid task ID");
   Task task=Task::fromJson(row);
   if(task.id<=0||!ids.insert(task.id).second||(task.status!="todo"&&task.status!="done"&&task.status!="in-progress"))throw runtime_error("Invalid task data");
   parsed.push_back(task);
  }
  tasks=parsed;nextId=getMaxId()==INT_MAX?0:getMaxId()+1;
 }catch(const exception& e){throw runtime_error(string("Cannot load task data; original file preserved: ")+e.what());}
}
bool TaskManager::saveTasks(){const string temporary=filename+".tmp";
 try{json data=json::array();for(const auto& task:tasks)data.push_back(task.toJson());
  ofstream out(temporary,ios::trunc);out<<data.dump(2)<<endl;out.close();if(!out)throw runtime_error("temporary file write failed");
#ifdef _WIN32
  if(!MoveFileExW(filesystem::path(temporary).c_str(),filesystem::path(filename).c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH))throw runtime_error("atomic replacement failed");
#else
  filesystem::rename(temporary,filename);
#endif
  return true;
 }catch(const exception& e){error_code ignored;filesystem::remove(temporary,ignored);cerr<<"Error saving tasks: "<<e.what()<<endl;return false;}
}
bool TaskManager::addTask(const string& desc){if(nextId<=0)return false;const auto previousTasks=tasks;const int previousNextId=nextId;
 try{Task t;t.id=nextId;nextId=nextId==INT_MAX?0:nextId+1;t.desc=desc;t.status="todo";t.createdAt=getCurrentTimestamp();t.updatedAt=t.createdAt;tasks.push_back(t);if(saveTasks())return true;}catch(const exception& e){cerr<<"Error adding task: "<<e.what()<<endl;}
 tasks=previousTasks;nextId=previousNextId;return false;
}
bool TaskManager::updateTask(int id,const string& desc){Task* t=findTaskById(id);if(!t)return false;const auto old=tasks;t->desc=desc;t->updatedAt=getCurrentTimestamp();if(saveTasks())return true;tasks=old;return false;}
bool TaskManager::deleteTask(int id){auto it=find_if(tasks.begin(),tasks.end(),[id](const Task& t){return t.id==id;});if(it==tasks.end())return false;const auto old=tasks;tasks.erase(it);if(saveTasks())return true;tasks=old;return false;}
bool TaskManager::markInProgress(int id){Task* t=findTaskById(id);if(!t)return false;const auto old=tasks;t->status="in-progress";t->updatedAt=getCurrentTimestamp();if(saveTasks())return true;tasks=old;return false;}
bool TaskManager::markDone(int id){Task* t=findTaskById(id);if(!t)return false;const auto old=tasks;t->status="done";t->updatedAt=getCurrentTimestamp();if(saveTasks())return true;tasks=old;return false;}
bool TaskManager::markTodo(int id){Task* t=findTaskById(id);if(!t)return false;const auto old=tasks;t->status="todo";t->updatedAt=getCurrentTimestamp();if(saveTasks())return true;tasks=old;return false;}
vector<Task> TaskManager::getAllTasks()const{return tasks;}
vector<Task> TaskManager::getTodoTasks()const{vector<Task> r;copy_if(tasks.begin(),tasks.end(),back_inserter(r),[](const Task& t){return t.status=="todo";});return r;}
vector<Task> TaskManager::getInProgressTasks()const{vector<Task> r;copy_if(tasks.begin(),tasks.end(),back_inserter(r),[](const Task& t){return t.status=="in-progress";});return r;}
vector<Task> TaskManager::getDoneTasks()const{vector<Task> r;copy_if(tasks.begin(),tasks.end(),back_inserter(r),[](const Task& t){return t.status=="done";});return r;}
Task* TaskManager::findTaskById(int id){auto it=find_if(tasks.begin(),tasks.end(),[id](Task& t){return t.id==id;});return it!=tasks.end()?&(*it):nullptr;}
const Task* TaskManager::findTaskById(int id)const{auto it=find_if(tasks.begin(),tasks.end(),[id](const Task& t){return t.id==id;});return it!=tasks.end()?&(*it):nullptr;}
