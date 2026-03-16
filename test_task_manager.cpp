#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "task_manager.h"

using namespace std;

class TaskManagerTest : public ::testing::Test {
 protected:
  const string test_file = "test_tasks.json";

  void SetUp() override {
    // Limpiar archivo de test antes de cada test
    filesystem::remove(test_file);
  }

  void TearDown() override {
    // Limpiar archivo de test después de cada test
    filesystem::remove(test_file);
  }
};

// Tests para agregar tareas
TEST_F(TaskManagerTest, AddFirstTask) {
  TaskManager tm(test_file);
  EXPECT_TRUE(tm.addTask("Tarea 1"));

  auto all_tasks = tm.getAllTasks();
  EXPECT_EQ(all_tasks.size(), 1);
  EXPECT_EQ(all_tasks[1], "Tarea 1");
}

TEST_F(TaskManagerTest, AddMultipleTasks) {
  TaskManager tm(test_file);
  EXPECT_TRUE(tm.addTask("Tarea 1"));
  EXPECT_TRUE(tm.addTask("Tarea 2"));
  EXPECT_TRUE(tm.addTask("Tarea 3"));

  auto all_tasks = tm.getAllTasks();
  EXPECT_EQ(all_tasks.size(), 3);
}

// Tests para actualizar tareas
TEST_F(TaskManagerTest, UpdateTask) {
  TaskManager tm(test_file);
  tm.addTask("Descripción original");

  EXPECT_TRUE(tm.updateTask(1, "Nueva descripción"));
  auto all_tasks = tm.getAllTasks();
  EXPECT_EQ(all_tasks[1], "Nueva descripción");
}

TEST_F(TaskManagerTest, UpdateNonExistentTask) {
  TaskManager tm(test_file);
  EXPECT_FALSE(tm.updateTask(999, "Nueva descripción"));
}

// Tests para eliminar tareas
TEST_F(TaskManagerTest, DeleteTask) {
  TaskManager tm(test_file);
  tm.addTask("Tarea a eliminar");

  EXPECT_TRUE(tm.deleteTask(1));
  auto all_tasks = tm.getAllTasks();
  EXPECT_EQ(all_tasks.size(), 0);
}

TEST_F(TaskManagerTest, DeleteNonExistentTask) {
  TaskManager tm(test_file);
  EXPECT_FALSE(tm.deleteTask(999));
}

// Tests para cambiar estado a in-progress
TEST_F(TaskManagerTest, MarkTaskInProgress) {
  TaskManager tm(test_file);
  tm.addTask("Tarea en progreso");

  EXPECT_TRUE(tm.markInProgress(1));
  auto in_progress = tm.getInProgressTasks();
  EXPECT_EQ(in_progress.size(), 1);
  EXPECT_EQ(in_progress[1], "Tarea en progreso");
}

// Tests para cambiar estado a done
TEST_F(TaskManagerTest, MarkTaskDone) {
  TaskManager tm(test_file);
  tm.addTask("Tarea completada");

  EXPECT_TRUE(tm.markDone(1));
  auto done = tm.getDoneTasks();
  EXPECT_EQ(done.size(), 1);
}

// Tests para listar por estado
TEST_F(TaskManagerTest, ListTodoTasks) {
  TaskManager tm(test_file);
  tm.addTask("Tarea 1");
  tm.addTask("Tarea 2");
  tm.markDone(1);

  auto todo = tm.getTodoTasks();
  EXPECT_EQ(todo.size(), 1);
}

// Test de persistencia
TEST_F(TaskManagerTest, PersistenceBetweenInstances) {
  {
    TaskManager tm1(test_file);
    tm1.addTask("Tarea persistente");
    tm1.addTask("Otra tarea");
  }

  // Nueva instancia carga del archivo
  TaskManager tm2(test_file);
  auto all_tasks = tm2.getAllTasks();
  EXPECT_EQ(all_tasks.size(), 2);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}