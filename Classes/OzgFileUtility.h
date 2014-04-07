
/*
demo
vector<string> files = OzgFileUtility::getFoldersAndFile("/");
vector<string>::iterator filesIterator = files.begin();
while (filesIterator != files.end())
{
cout << *filesIterator << endl;
filesIterator++;
}
*/

#ifndef __ozgcc2dxutils__OzgFileUtility__
#define __ozgcc2dxutils__OzgFileUtility__

#include "cocos2d.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <ctime>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WIN8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//mac or ios or android or linux
#include <dirent.h>
#else
//win32 or win8 or winrt
#include <Windows.h>
#endif

using namespace std;

const int MAX_DUR = 5 * 60;

class OzgFileUtility
{

private:
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	static wstring OzgFileUtility::s2ws(const std::string& s);
#endif

    //判断是否是特殊目录
    static bool isSpecialDir(const char *path);
    
    //生成完整的文件路径
    static void getFilePath(const char *path, const char *fileName, char *filePath);
    
public:
    
    //获取路径下的所有文件夹和文件
    static vector<string> getFoldersAndFile(const string &path, bool showAbsolutePath = true);
    
    //获取路径下的所有文件夹
    static vector<string> getFolders(const string &path, bool showAbsolutePath = true);
    
    //获取路径下的所有文件，可指定文件后缀名
    static vector<string> getFiles(const string &path, bool showAbsolutePath = true, const string &postfix = "");
    
    //检查文件或目录是否准备好，一定程度上防止数据正在拷贝的时候去读写。
    //方法是检查文件或目录的access time是否在5分钟以前
    static bool isPrepared(const string &path);
    
    //判断是否为文件夹
    static bool isFolder(const string &path);
    
    //判断是否为文件
    static bool isFile(const string &path);
    
    //判断文件或文件夹是否存在
    static bool fileExists(const string &path);
    
    //删除非空文件夹
    static void deleteFile(const string &path);
    
    //复制文件
    static bool copyFile(const char *src, const char *des);
    
    //复制文件夹（des的路径如果存在则直接替换成新的版本）
    static void copyFolder(const char *src, const char *des);
    
};


#endif /* defined(__ozgcc2dxutils__OzgFileUtility__) */
