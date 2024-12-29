#!/bin/bash

# Git configurations
git config --global pull.rebase false
git config --global core.editor "code --wait"
git config --global init.defaultBranch main
git config --global credential.helper cache
git config --global color.ui auto
git config --global --add safe.directory "/workspaces/${PWD##*/}"

# Configure git to automatically stage changes and simplify workflows
git config --global add.all true
git config --global push.default current
git config --global push.autoSetupRemote true   # Automatically sets up remote tracking
git config --global push.followTags true        # Push tags automatically with commits
git config --global core.autocrlf input         # Handles line endings automatically

# Configure VS Code to auto-push on commit
git config --global postCommit.push true        # Enables auto-push after commit

# In Codespaces or GitHub Actions, use default values
if [ -n "$CODESPACES" ] || [ -n "$GITHUB_ACTIONS" ]; then
    # Use GitHub user info in Codespaces
    if [ -n "$GITHUB_USER" ]; then
        git config --global user.name "$GITHUB_USER"
    else
        git config --global user.name "CS 161 Student"
    fi
    
    if [ -n "$GITHUB_TOKEN" ]; then
        # Extract email from GitHub API
        EMAIL=$(curl -s -H "Authorization: token $GITHUB_TOKEN" https://api.github.com/user | grep -Po '"email": *"\K[^"]*')
        if [ -n "$EMAIL" ]; then
            git config --global user.email "$EMAIL"
        else
            git config --global user.email "student@example.com"
        fi
    else
        git config --global user.email "student@example.com"
    fi
else
    # Try to get GitHub info if gh CLI is available and authenticated
    if command -v gh &> /dev/null && gh auth status &> /dev/null; then
        # Get user info from GitHub CLI
        GH_USER=$(gh api user -q '.login')
        GH_EMAIL=$(gh api user -q '.email')
        
        if [ -n "$GH_USER" ]; then
            git config --global user.name "$GH_USER"
        else
            # Fall back to interactive prompt
            read -p "Enter your name: " name
            git config --global user.name "$name"
        fi
        
        if [ -n "$GH_EMAIL" ]; then
            git config --global user.email "$GH_EMAIL"
        else
            # Fall back to interactive prompt
            read -p "Enter your college email address: " email
            git config --global user.email "$email"
        fi
    else
        # Fall back to interactive prompt if gh CLI not available/authenticated
        echo "We need to set up some configurations."
        read -p "Enter your college email address: " email
        git config --global user.email "$email"
        
        read -p "Enter your name: " name
        git config --global user.name "$name"
    fi
fi