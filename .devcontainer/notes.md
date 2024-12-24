Add the following to the devcontainer settings.json file for production builds:

```json
"files.exclude": {
                    "**/.devcontainer": true,
                    "**/.vscode": true,
                    "**/build": true,
                    "**/test": true,
                    "**/.gitignore": true
                },
```