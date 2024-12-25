#!/bin/bash

# Git configurations
git config --global pull.rebase false
git config --global core.editor "code --wait"
git config --global init.defaultBranch main
git config --global credential.helper cache
git config --global color.ui auto
git config --global --add safe.directory "/workspaces/${PWD##*/}"

# In Codespaces or GitHub Actions, use default values
if [ -n "$CODESPACES" ] || [ -n "$GITHUB_ACTIONS" ]; then
    # Only set if not already configured
    if [ -z "$(git config --global user.email)" ]; then
        git config --global user.email "student@example.com"
    fi
    if [ -z "$(git config --global user.name)" ]; then
        git config --global user.name "CS 161 Student"
    fi
else
    # Only prompt for input in local development
    if [ -z "$(git config --global user.email)" ]; then
        echo "We need to set up some configurations."
        read -p "Enter your college email address: " email
        git config --global user.email "$email"
    fi

    if [ -z "$(git config --global user.name)" ]; then
        read -p "Enter your name: " name
        git config --global user.name "$name"
    fi 
fi