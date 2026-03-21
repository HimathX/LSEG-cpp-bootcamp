# Quickstart Guide

This guide will help you set up and run both the **server** (Python) and the **GUI** (React/Vite).

---

## 1. Server Setup (Python)

The server uses `uv`, an extremely fast Python package installer and resolver.

### Commands

1. **Navigate to the server directory**
   ```bash
   cd server
   ```

2. **Initialize a new uv project (if starting from scratch)**
   ```bash
   uv init
   ```
   *What it does:* `uv init` creates a new Python project by generating a `pyproject.toml` file (which defines your project metadata and dependencies) and handles environment setup. In this repo, it is already initialized, but `uv init` is how you would start a new one.

3. **Install dependencies**
   ```bash
   uv sync
   ```
   *What it does:* This command reads the `pyproject.toml` or `uv.lock` file, creates a virtual environment if one doesn't exist, and installs all required dependencies (like FastAPI and Uvicorn).

4. **Run the server**
   ```bash
   uv run python main.py
   ```
   *What it does:* `uv run` ensures the command is executed within the project's virtual environment. This starts your backend server.

---

## 2. GUI Setup (Node.js/React)

The frontend is a React application built with Vite and Tailwind.

### Commands

1. **Navigate to the GUI directory**
   ```bash
   cd gui
   ```

2. **Install dependencies**
   ```bash
   npm install
   ```
   *What it does:* This reads the `package.json` file and downloads all required libraries (like React, Tailwind CSS, Framer Motion, and shadcn components) into the `node_modules` folder so your app can run.

3. **Start the development server**
   ```bash
   npm run dev
   ```
   *What it does:* This starts the Vite development server with Hot Module Replacement (HMR). Any changes you make to the code will instantly reflect in the browser. You can typically view the app at `http://localhost:5173`.

### Additional GUI Commands
- **Build for production:**
  ```bash
  npm run build
  ```
  *(Compiles TypeScript and bundles the app for production)*
- **Preview production build:**
  ```bash
  npm run preview
  ```
  *(Serves the production build locally to test it)*
