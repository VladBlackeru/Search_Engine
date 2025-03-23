# Architecture

## Overview
This document outlines the architecture of the search engine using Simon Brown's C4 model. The architecture is structured into four levels:

1. **System Context** - Defines the overall system and its interactions.
2. **Containers** - Identifies the main deployable units.
3. **Components** - Breaks down containers into logical building blocks.
4. **Code** - Describes classes and functions within components.

---

## 1. System Context
The search engine is a system that allows users to store and search file contents efficiently using a PostgreSQL database. It interacts with the following external systems:

- **User (CLI Interface)**: Users provide file paths and search queries.
- **PostgreSQL Database**: Stores metadata and indexed file contents.

---

## 2. Containers
The system consists of the following containers:

- **CLI Application (C++)**:
  - Reads file contents and inserts them into the database.
  - Provides a command-line interface for searching.
- **PostgreSQL Database**:
  - Stores file metadata (name, path, extension, size).
  - Stores file contents and enables full-text search.

---

## 3. Components
Each container consists of components:

### **CLI Application Components**
1. **Database Connector**
   - Establishes a connection to PostgreSQL.
   - Executes SQL queries securely.
   
2. **File Processor**
   - Reads file contents line-by-line.
   - Extracts metadata (name, path, size, extension).
   
3. **Search Engine**
   - Implements full-text search using PostgreSQL's `tsvector`.
   - Supports partial word matching.

4. **Error Logging**
   - Logs database and file processing errors.

### **PostgreSQL Database Components**
1. **Files Table (`search_engine.files`)**
   - Stores file metadata.
2. **File Contents Table (`search_engine.file_contents`)**
   - Stores indexed file contents with references to the files table.
3. **Full-Text Search Index**
   - Uses PostgreSQL’s full-text search (`tsvector`) for efficient searches.

---

## 4. Code-Level Details
The main classes and functions include:

- `establish_connection()`: Connects to the PostgreSQL database.
- `insertFileToDatabase()`: Inserts a file and its content into the database.
- `printSearchResults()`: Retrieves and displays search results.
- `deleteRowFromDatabase()`: Deletes a file from the database.
- `insertRec()`: Recursively inserts files from a directory.
