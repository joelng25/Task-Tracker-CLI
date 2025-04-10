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
  int last_id = 0;
  int counter_task = 0;
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
    counter_task = pos - 2;
    task_file.close();
  }

  // encontrar último ID
  if (counter_task != 0) {
    size_t lastId = content.rfind("\"id\":");
    lastId += 5;
    last_id = stoi(content.substr(lastId, lastId + 1 - lastId));
  }

  // comando añadir tarea
  if (command == "add" && argc > 2) {
    if (pos == 0) {
      ofstream first_task(FILE_NAME, ios::app);
      first_task << "[" << endl;

      first_task << "{\"id\":" << 1 << ", \"desc\":\"" << desc
                 << "\", \"status\":\"" << status[0] << "\", \"createdAt\":\""
                 << createdAt << "\", \"updatedAt\":\"" << updatedAt << "\"}"
                 << endl;

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

      new_task << "{\"id\":" << last_id + 1 << ", \"desc\":\"" << desc
               << "\", \"status\":\"" << status[0] << "\", \"createdAt\":\""
               << createdAt << "\", \"updatedAt\":\"" << updatedAt << "\"}"
               << endl;

      new_task << "]" << endl;

      new_task.close();

      counter_task = pos - 1;
    }
  }

  // comando update tarea con nueva descripción
  else if (command == "update" and argc == 4) {
    string id = argv[2];
    string new_desc = argv[3];

    size_t id_pos = content.find("\"id\":" + id);  // busca el id en json
    if (id_pos == string::npos) {
      cerr << "ID " << id << " no encontrado." << endl;
      return 1;
    }
    size_t start_desc = content.find("\"desc\":", id_pos);  // busca inicio desc
    size_t end_desc = content.find("\",", start_desc);      // busca final tarea

    size_t start_update =
        content.find("\"updatedAt\":", end_desc);  // busca inicio update
    size_t end_update =
        content.find("\"", start_update + 13);  // busca final update

    content.replace(start_desc + 8, end_desc - (start_desc + 8), new_desc);
    content.replace(start_update + 13, end_update - (start_update + 13),
                    updatedAt);

    ofstream update_task(FILE_NAME, ios::trunc);
    update_task << content;
    update_task.close();

  }

  // comando delete tarea
  else if (command == "delete" and argc == 3) {
    string id = argv[2];
    int id_num = atoi(argv[2]);

    size_t id_pos = content.find("\"id\":" + id);  // busca el ID en el json
    if (id_pos == string::npos) {
      cerr << "ID " << id << " no encontrado." << endl;
      return 1;
    }

    size_t start_task =
        content.rfind("{", id_pos);  // encuentra el inicio del objeto
    size_t end_task =
        content.find("}", id_pos);  // encuentra el final del objeto

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
    ofstream delete_task(FILE_NAME, ios::trunc);
    delete_task << content;
    delete_task.close();
  }

  return 0;
}