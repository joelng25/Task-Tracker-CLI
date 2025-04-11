# Task Tracker CLI - README.txt

Thanks to https://roadmap.sh/projects/task-tracker

A simple command-line task management application written in C++. This application allows 
you to keep track of your tasks by adding, updating, marking status, deleting, and listing them.

------------------------------------------------------------------------------------------
FEATURES
------------------------------------------------------------------------------------------

- Add new tasks with descriptions
- Update task descriptions
- Mark tasks as "todo", "in-progress", or "done"
- Delete tasks
- List all tasks or filter by status

------------------------------------------------------------------------------------------
INSTALLATION
------------------------------------------------------------------------------------------

Clone the repository and build the application:

    git clone https://github.com/yourusername/Task-Tracker-CLI.git
    cd Task-Tracker-CLI
    make

------------------------------------------------------------------------------------------
USAGE
------------------------------------------------------------------------------------------

    USAGE: task-cli <command> [arguments]

------------------------------------------------------------------------------------------
COMMANDS
------------------------------------------------------------------------------------------

Add a new task:
    ./task-cli add "Task description"

Update a task:
    ./task-cli update <id> "New task description"

Mark a task as in-progress:
    ./task-cli mark-in-progress <id>

Mark a task as done:
    ./task-cli mark-done <id>

Delete a task:
    ./task-cli delete <id>

List all tasks:
    ./task-cli list

List tasks by status:
    ./task-cli list todo
    ./task-cli list in-progress
    ./task-cli list done

------------------------------------------------------------------------------------------
EXAMPLES
------------------------------------------------------------------------------------------

Add a task:
    ./task-cli add "Finish project report"

Update a task:
    ./task-cli update 1 "Finish project report by Friday"

Mark a task as in-progress:
    ./task-cli mark-in-progress 1

Mark a task as done:
    ./task-cli mark-done 1

Delete a task:
    ./task-cli delete 1

List all todo tasks:
    ./task-cli list todo

------------------------------------------------------------------------------------------
DATA STORAGE
------------------------------------------------------------------------------------------

Tasks are stored in a JSON file (tasks.json) in the following format:

[
  {"id":1, "desc":"Example task", "status":"todo", "createdAt":"time", "updatedAt":"time"},
  {"id":2, "desc":"Another task", "status":"in-progress", "createdAt":"time", "updatedAt":"time"}
]

------------------------------------------------------------------------------------------
CONTRIBUTING
------------------------------------------------------------------------------------------

Contributions are welcome! Please feel free to submit a Pull Request.

------------------------------------------------------------------------------------------
LICENSE
------------------------------------------------------------------------------------------

This project is licensed under the MIT License - see the LICENSE file for details.
