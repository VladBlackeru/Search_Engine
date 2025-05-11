#ifndef APP_FILEREPOSITORY_H
#define APP_FILEREPOSITORY_H

#include "Database.h"
#include "SearchObserver.h"
#include <string>
#include <vector>

class FileRepository {
public:
    explicit FileRepository(Database& db);

    bool insertFile(const std::string& filePath);

    bool deleteFileByPath(const std::string& filePath);

    void deleteTableContents(const std::string& tableName);

    void searchFiles(const std::string& searchPhrase);

    void insertRecursively(const std::string& directoryPath);

    void searchFilesAdvanced(const std::string& queryStr);

    void registerSearchObserver(SearchObserver* observer);
    void removeSearchObserver(SearchObserver* observer);

private:
    Database& db;
    std::vector<SearchObserver*> observers;

    void notifySearchObservers(const std::string& query);

};

#endif //APP_FILEREPOSITORY_H
