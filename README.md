# C++ Development Environment for CS161

This repository provides a standardized development environment for introductory C++ programming courses. It includes:
- A pre-configured development container
- Automated build and test tasks
- Simple I/O testing infrastructure
- GitHub Codespaces support
- GitHub Classroom integration
- Status bar buttons (and associated tasks) for building and running code, and running tests
- A script to assist with Git configuration (email and name)

This repository does not contain:
- Normal Unit testing (IO only)
- Cmake

## Quick Start

### Using GitHub Codespaces (Recommended)
1. Click the green "Code" button above
2. Select "Create codespace on main"
3. Wait for the environment to build (this takes a few minutes the first time)
4. Start coding! The environment comes with everything pre-configured

Note: By default, only the `src` directory is visible. To show hidden files:
1. Click View in the menu bar
2. Select "Show Hidden Files"

### Running Locally with VS Code
1. Install [VS Code](https://code.visualstudio.com/)
2. Install the [Dev Containers extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)
3. Install [Docker Desktop](https://www.docker.com/products/docker-desktop/)
4. Clone this repository
5. Open the repository in VS Code
6. When prompted "Reopen in Container", click yes
   - Or press F1, type "Reopen in Container", and select that option

## Building and Running Code

### Using Task Buttons
Look for these buttons in your VS Code status bar (bottom of screen):
- ▶️ Build & Run: Compiles and runs the current program
- 🧪 Run Tests: Runs the test suite

### Using VS Code Tasks
1. Press `Cmd+Shift+B` (Mac) or `Ctrl+Shift+B` (Windows/Linux) to build and run
2. Or open the Command Palette (`Cmd+Shift+P` / `Ctrl+Shift+P`) and type "Run Task"

## Testing Infrastructure

The `test` directory contains a simple I/O testing framework designed for introductory C++:
- Tests input/output behavior of programs
- Supports exact matching or partial matching
- Can ignore case and whitespace differences
- Provides detailed test output

### Test Structure
- `test_cases.h`: Define your test cases here
- `run_io_tests.cpp`: The test runner
- Tests run against program output, comparing with expected results

## Development Environment Details

### What is a Dev Container?
A dev container is a Docker container specifically configured for development. It ensures that:
- All students have the same compiler version and tools
- No need to install C++ tools locally
- Works the same way on Windows, Mac, and Linux
- Matches the environment used for grading

### Git Configuration
This environment is configured to make Git version control as transparent as possible for students new to programming:

- `setup-git.sh` runs automatically when the container starts, prompting for name and email to configure Git
- Git operations are automated:
  - Changes are automatically staged
  - Commits automatically trigger a push
  - No need to manually sync or push changes
  - Single-click commit from the Source Control tab

These settings help students focus on programming while maintaining a good commit history for their work.

### File Visibility
By default, only the `src` directory is visible to reduce clutter. Hidden files include:
- `.devcontainer/`
- `.vscode/`
- `.github/`
- `build/`
- `test/`
- `.gitignore`

To show these files, modify the workspace settings in `.vscode/settings.json`. Find the `files.exclude` section and remove the patterns for files you want to show. For example, to show all development files:

```json
"files.exclude": {
    "**/.git": true  // Only hide .git directory
}
```

Or to show only test files while keeping other files hidden:

```json
"files.exclude": {
    "**/.git": true,
    "**/.devcontainer": true,
    "**/.vscode": true,
    "**/.github": true,
    "**/build": true,
    "**/.gitignore": true
}
```

Note: VS Code applies settings in this order:
1. Default VS Code settings
2. User settings (your personal settings)
3. Workspace settings (from `.vscode/settings.json`)

This means workspace settings will override both default and user settings. If you need to see hidden files, it's recommended to modify the workspace settings so the change applies to all users.

## GitHub Classroom Integration

This repository is designed to work seamlessly with GitHub Classroom:

### For Instructors
1. Create a GitHub Classroom
2. Use this repository as your template/starter code
3. Create an assignment:
   - Select "Create an individual assignment"
   - Choose this repository as the template
   - Configure your assignment settings
   - Enable Codespaces if your organization supports it

### For Students
1. Accept the assignment using the provided GitHub Classroom link
2. A new repository will be created with this template
3. Open in Codespaces (recommended) or clone locally
4. Start coding in the `src` directory

### GitHub Workflows
The `.github` directory contains workflow configurations for GitHub Classroom:
- Automated testing setup
- Codespaces configuration
- Assignment grading workflows

If you're not using GitHub Classroom, you can remove the `.github` directory:
```bash
rm -rf .github
```

## Configuration Files

### `.devcontainer/devcontainer.json`
- Defines the development container configuration
- Specifies VS Code extensions to install
- Sets up task buttons and other VS Code settings

### `.vscode/tasks.json`
- Defines build and test commands
- Configures problem matchers for compiler errors
- Sets up the Build & Run and Test tasks

### `.vscode/settings.json`
- Configures VS Code behavior
- Sets up code runner and task buttons
- Defines debugging settings

## Directory Structure

```
.
├── src/
│   └── main.cpp       # Main program file
├── test/
│   ├── test_cases.h   # Test case definitions
│   └── run_io_tests.cpp # Test runner
├── .devcontainer/     # Container configuration
├── .vscode/          # VS Code configuration
└── build/           # Compiled outputs
```

## Common Issues

### Docker Not Running
If you get a "Docker not running" error when trying to open in container:
1. Start Docker Desktop
2. Try reopening in container again

### Container Build Failed
If the container fails to build:
1. Check your internet connection
2. Try rebuilding the container
3. Make sure Docker has enough resources allocated

## Contributing

To customize this environment for your course:
1. Fork this repository
2. Modify the test cases in `test_cases.h`
3. Update the README and documentation as needed
4. Test thoroughly in both Codespaces and local containers 