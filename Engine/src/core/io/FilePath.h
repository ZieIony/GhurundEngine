#pragma once

#include "DirectoryPath.h"
#include <Shlwapi.h>

namespace Ghurund {

    class FilePath:public Path {
    public:
        FilePath(const DirectoryPath& dirPath, const UnicodeString& fileName):Path(dirPath.get()) {
            path.add(fileName);
        }

        FilePath(const char* path):FilePath(UnicodeString(path)) {}

        FilePath(const wchar_t* path):FilePath(UnicodeString(path)) {}

        FilePath(const UnicodeString& path):Path(path) {
            DWORD attributes = GetFileAttributesW(path);

            if (attributes != INVALID_FILE_ATTRIBUTES && attributes & FILE_ATTRIBUTE_DIRECTORY)
                Logger::log(LogType::ERR0R, _T("invalid file path {}\n"), String(path.getData()));
        }

        FilePath(const FilePath & path):Path(path.get()) {}

        DirectoryPath getDirectory() const {
            size_t index = path.findLast(L"\\");
            if (index != path.Size)
                return path.subString(0, index);
            return path;
        }

        __declspec(property(get = getDirectory)) DirectoryPath Directory;

        UnicodeString getFileName() const {
            size_t index = path.findLast(L"\\");
            if (index != path.Size)
                return path.subString(index + 1);
            return path;
        }

        __declspec(property(get = getFileName)) UnicodeString FileName;

        FilePath getRelativePath(const DirectoryPath& dir) const {
            wchar_t relativePathStr[MAX_PATH];
            BOOL success = PathRelativePathToW(relativePathStr, dir.get().getData(), FILE_ATTRIBUTE_DIRECTORY, path.getData(), FILE_ATTRIBUTE_NORMAL);
            if (success)
                return FilePath(relativePathStr);
            return *this;
        }

        FilePath getAbsolutePath() const {
            wchar_t fullPath[MAX_PATH];
            GetFullPathNameW(path, MAX_PATH, fullPath, nullptr);
            return FilePath(fullPath);
        }

        __declspec(property(get = getAbsolutePath)) FilePath AbsolutePath;
    };
}