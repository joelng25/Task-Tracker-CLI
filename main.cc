#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
// Constants
const string FILE_NAME = "tasks.json";
const string ID_KEY = "\"id\":";
const string DESC_KEY = "\"desc\":";
const string STATUS_KEY = "\"status\":";
const string UPDATED_KEY = "\"updatedAt\":";

void Usage() {
  cout << "USAGE: task-cli <command> [arguments]" << endl;
  exit(1);
}

int main(int argc, char* argv[]) {
  vector<string> status = {"todo", "in-progress", "done"};
  string createdAt;
  string updatedAt;

  if (argc == 1) Usage();

  bool to_modify = false;  // comprobar modificacion con comando

  string command = argv[1];

  time_t now = time(0);
  string dt = ctime(&now);
  dt.resize(dt.size() - 1);

  createdAt = updatedAt = dt;

  int pos = 0;
  int last_id = 0;
  int counter_task = 0;

  string line;
  string content;

  map<int, string> all_tasks;
  map<int, string> todo_tasks;
  map<int, string> inprogress_tasks;
  map<int, string> done_tasks;

  // crea si no existe y escribe al final
  ifstream task_file(FILE_NAME);
  if (task_file.fail()) {
    ofstream task_file(FILE_NAME);
  } else {
    while (getline(task_file, line)) {
      size_t start_id = line.find(ID_KEY);       // busca el id en json
      size_t end_id = line.find(",", start_id);  // busca el fin id

      size_t start_desc = line.find(DESC_KEY) + 8;     // busca inicio desc
      size_t end_desc = line.find("\",", start_desc);  // busca final tarea

      if (start_id != string::npos) {
        int id_num = stoi(line.substr(start_id + 5, end_id - (start_id + 5)));

        string desc_task = line.substr(start_desc, end_desc - start_desc);

        all_tasks[id_num] = desc_task;

        if (line.find("todo") != string::npos) {
          // añadir tarea a todo list
          todo_tasks[id_num] = desc_task;
          ;
        } else if (line.find("in-progress") != string::npos) {
          // añadir tarea a inprogress list
          inprogress_tasks[id_num] = desc_task;
          ;
        } else if (line.find("done") != string::npos) {
          // añadir tarea a done list
          done_tasks[id_num] = desc_task;
          ;
        }
      }

      content += line + "\n";
      ++pos;
    }
    counter_task = pos - 2;
    task_file.close();
  }

  // encontrar último ID
  if (counter_task != 0) {
    size_t lastId = content.rfind(ID_KEY);
    lastId += 5;
    last_id = stoi(content.substr(lastId, lastId + 1 - lastId));
  }

  // comando añadir tarea
  if (command == "add" && argc > 2) {
    string desc = argv[2];

    if (pos == 0) {
      ofstream first_task(FILE_NAME, ios::app);
      first_task << "[" << endl;

      first_task << "{\"id\":" << 1 << ", \"desc\":\"" << desc << "\", \"status\":\"" << status[0]
                 << "\", \"createdAt\":\"" << createdAt << "\", \"updatedAt\":\"" << updatedAt
                 << "\"}" << endl;

      first_task << "]" << endl;
      counter_task = 1;
    } else {
      size_t end_bracket = content.rfind("]");
      ofstream new_task(FILE_NAME, ios::trunc);
      new_task << content.substr(0, end_bracket - 1);

      if (counter_task != 0) {
        new_task << ",";
      }
      new_task << endl;

      new_task << "{\"id\":" << last_id + 1 << ", \"desc\":\"" << desc << "\", \"status\":\""
               << status[0] << "\", \"createdAt\":\"" << createdAt << "\", \"updatedAt\":\""
               << updatedAt << "\"}" << endl;

      new_task << "]" << endl;

      new_task.close();

      counter_task = pos - 1;
    }
  }

  // comando update tarea con nueva descripción
  else if (command == "update" and argc == 4) {
    string id = argv[2];
    string new_desc = argv[3];

    size_t id_pos = content.find(ID_KEY + id);  // busca el id en json
    if (id_pos == string::npos) {
      cerr << "ID " << id << " no encontrado." << endl;
      return 1;
    }
    size_t start_desc = content.find(DESC_KEY, id_pos) + 8;  // busca inicio desc
    size_t end_desc = content.find("\",", start_desc);       // busca final tarea

    size_t start_update = content.find(UPDATED_KEY, end_desc) + 13;  // busca inicio update
    size_t end_update = content.find("\"", start_update);            // busca final update

    content.replace(start_desc, end_desc - start_desc, new_desc);
    content.replace(start_update, end_update - start_update, updatedAt);

    to_modify = true;
  }

  // comando delete tarea
  else if (command == "delete" and argc == 3) {
    string id = argv[2];
    size_t id_pos = content.find(ID_KEY + id);  // busca el ID en el json
    if (id_pos == string::npos) {
      cerr << "ID " << id << " no encontrado." << endl;
      return 1;
    }

    size_t start_task = content.rfind("{", id_pos);  // encuentra el inicio del objeto
    size_t end_task = content.find("}", id_pos);     // encuentra el final del objeto

    // ajusta el rango para eliminar correctamente
    if (content[end_task + 1] == ',') {
      // si no es el último elemento, incluye la coma
      end_task += 2;
    } else if (counter_task != 1) {
      // si es el último elemento (no único), elimina la coma anterior
      start_task -= 2;
    } else {
      // único elemento
      ++end_task;
    }

    // elimina el rango correspondiente
    content.erase(start_task, end_task - start_task + 1);

    // escribe el contenido actualizado al archivo
    to_modify = true;
  }

  else if (command == "mark-in-progress" or command == "mark-done") {
    string id = argv[2];
    size_t id_pos = content.find(ID_KEY + id);  // busca el ID en el json
    if (id_pos == string::npos) {
      cerr << "ID " << id << " no encontrado." << endl;
      return 1;
    }
    size_t start_status = content.find(STATUS_KEY, id_pos) + 10;
    size_t end_status = content.find("\",", start_status);

    size_t start_update = content.find(UPDATED_KEY, end_status) + 13;  // busca inicio update
    size_t end_update = content.find("\"", start_update);              // busca final update

    content.replace(start_update, end_update - start_update, updatedAt);

    if (command == "mark-in-progress") {
      content.replace(start_status, end_status - start_status, "in-progress");
    } else {
      content.replace(start_status, end_status - start_status, "done");
    }

    to_modify = true;
  }

  // comando list task
  else if (command == "list") {
    if (argc == 3) {
      string status_list = argv[2];
      if (status_list == "todo") {  // list todo
        for (const auto& pair : todo_tasks) {
          cout << "ID: " << pair.first << " - " << pair.second << endl;
        }
      } else if (status_list == "in-progress") {  // list in-progress
        for (const auto& pair : inprogress_tasks) {
          cout << "ID: " << pair.first << " - " << pair.second << endl;
        }
      } else if (status_list == "done") {  // list done
        for (const auto& pair : done_tasks) {
          cout << "ID: " << pair.first << " - " << pair.second << endl;
        }
      }
    }
    // imprime todas las tasks
    else if (argc == 2) {
      for (const auto& pair : all_tasks) {
        cout << "ID: " << pair.first << " - " << pair.second << endl;
      }
    }
  }

  else {
    Usage();
  }

  // si se ha modifica algo lo cambia
  if (to_modify) {
    ofstream new_task(FILE_NAME, ios::trunc);
    new_task << content;
    new_task.close();
  }

  return 0;
}