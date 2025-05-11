//
// Created by Vlod on 05/11/2025.
//

#ifndef APP_FILEREPOSITORY_H
#define APP_FILEREPOSITORY_H


#ifndef FILEREPOSITORY_H
#define FILEREPOSITORY_H

#include "Database.h"
#include <string>

class FileRepository {
public:
    explicit FileRepository(Database& db);

    bool insertFile(const std::string& filePath);

    bool deleteFileByPath(const std::string& filePath);

    void deleteTableContents(const std::string& tableName);

    void searchFiles(const std::string& searchPhrase);

    void insertRecursively(const std::string& directoryPath);

private:
    Database& db;
};

#endif // FILEREPOSITORY_H


#endif //APP_FILEREPOSITORY_H
