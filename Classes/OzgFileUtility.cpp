
#include "OzgFileUtility.h"

//私有方法

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//win32 or win8 or winrt
wstring OzgFileUtility::s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
#endif

bool OzgFileUtility::isSpecialDir(const char *path)
{
    return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

void OzgFileUtility::getFilePath(const char *path, const char *fileName, char *filePath)
{
    strcpy(filePath, path);
    if(filePath[strlen(path) - 1] != '/')
        strcat(filePath, "/");
    strcat(filePath, fileName);
}

//获取路径下的所有文件夹和文件
vector<string> OzgFileUtility::getFoldersAndFile(const string &path, bool showAbsolutePath)
{
    vector<string> items;
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WIN8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//mac or ios or android or linux
    
    struct dirent* ent = NULL;
    DIR* pDir;
    pDir = opendir(path.c_str());
    while(NULL != (ent = readdir(pDir)))
    {
        string fullpath = path + "/" + ent->d_name;
        if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
        {
            string filePath;
            
            //绝对路径
            if(showAbsolutePath)
                filePath.append(path);
            
            //所在目录的结尾是否有/
            if (path.c_str()[path.size() - 1] != '/')
                filePath.append("/");
                            
            filePath.append(ent->d_name);
            items.push_back(filePath);
        }
    }
    closedir(pDir);
#else
//win32 or win8 or winrt

#endif
    
    return items;
}

//获取路径下的所有文件夹
vector<string> OzgFileUtility::getFolders(const string &path, bool showAbsolutePath)
{
    vector<string> folders;
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WIN8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//mac or ios or android or linux
    struct dirent* ent = NULL;
    DIR* pDir;
    pDir = opendir(path.c_str());
    while(NULL != (ent = readdir(pDir)))
    {
        string fullpath = path + "/" + ent->d_name;
        //if(4 == ent->d_type) //在nfs或xfs下，有的目录d_type也是0
        if(OzgFileUtility::isFolder(fullpath))
        {
            if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
            {
                string filePath;
                
                //绝对路径
                if(showAbsolutePath)
                    filePath.append(path);
                
                //所在目录的结尾是否有/
                if (path.c_str()[path.size() - 1] != '/')
                    filePath.append("/");
                
                filePath.append(ent->d_name);
                folders.push_back(filePath);
            }
        }
    }
    closedir(pDir);
#else
//win32 or win8 or winrt
    
#endif
    
    return folders;
}

//获取路径下的所有文件，可指定文件后缀名
vector<string> OzgFileUtility::getFiles(const string &path, bool showAbsolutePath, const string &postfix)
{
    vector<string> files;
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WIN8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//mac or ios or android or linux
    struct dirent* ent = NULL;
    DIR* pDir;
    pDir = opendir(path.c_str());
    while(NULL != (ent = readdir(pDir)))
    {
        string fullpath = path + "/" + ent->d_name;
        //if(8 == ent->d_type) //在nfs或xfs下，有的文件d_type也是0
        if(OzgFileUtility::isFile(fullpath))
        {
            if(postfix == "" || strstr(ent->d_name, postfix.c_str())!=NULL)
            {
                string filePath;
                
                //绝对路径
                if(showAbsolutePath)
                    filePath.append(path);
                
                //所在目录的结尾是否有/
                if (path.c_str()[path.size() - 1] != '/')
                    filePath.append("/");
                
                filePath.append(ent->d_name);
                files.push_back(filePath);
            }
        }
    }
    closedir(pDir);
#else
//win32 or win8 or winrt
    
#endif
    
    return files;
}

//检查文件或目录是否准备好，一定程度上防止数据正在拷贝的时候去读写。
//方法是检查文件或目录的access time是否在5分钟以前
bool OzgFileUtility::isPrepared(const string &path)
{
    struct stat st;
    stat(path.c_str(), &st);
    return time(0) - st.st_ctime >= MAX_DUR;
}

//判断是否为文件夹，用stat的标志来判断
bool OzgFileUtility::isFolder(const string &path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WIN8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//mac or ios or android or linux
    struct stat st;
    int ret = stat(path.c_str(), &st);
    return ret >= 0 && S_ISDIR(st.st_mode);
#else

	return false;
#endif

}

//判断是否为文件，用stat的标志来判断
bool OzgFileUtility::isFile(const string &path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WIN8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//mac or ios or android or linux
    struct stat st;
    int ret = stat(path.c_str(), &st);
    return ret >= 0 && S_ISREG(st.st_mode);
#else

	return false;
#endif

}

//判断文件或文件夹是否存在
bool OzgFileUtility::fileExists(const string &path)
{
    fstream fs;
    fs.open(path.c_str(), ios::in);
    if(!fs)
        return false;
    else
        return true;
    
}

void OzgFileUtility::deleteFile(const string &path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WIN8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//mac or ios or android or linux
    DIR *dir;
    dirent *dirInfo;
    char filePath[PATH_MAX];
    if(OzgFileUtility::isFile(path))
    {
        remove(path.c_str());
        return;
    }
    
    if(OzgFileUtility::isFolder(path))
    {
        if((dir = opendir(path.c_str())) == NULL)
            return;
        while((dirInfo = readdir(dir)) != NULL)
        {
            OzgFileUtility::getFilePath(path.c_str(), dirInfo->d_name, filePath);
            if(OzgFileUtility::isSpecialDir(dirInfo->d_name))
                continue;
            OzgFileUtility::deleteFile(filePath);
            rmdir(filePath);
        }
        
        rmdir(path.c_str());
    }
#else
//win32 or win8 or winrt
    
#endif
    
}

bool OzgFileUtility::copyFile(const char *src, const char *des)
{
    FILE * fSrc = fopen(src, "rb");
    if(!fSrc)
    {
        printf("打开文件`%s`失败", src);
        return false;
    }
    FILE * fDes = fopen(des, "wb");
    if(!fDes)
    {
        printf("创建文件`%s`失败", des);
        return false;
    }
    
    unsigned char * buf;
    unsigned int length;
    fseek(fSrc, 0, SEEK_END);
    length = ftell(fSrc);
    buf = new unsigned char[length+1];
    memset(buf, 0, length+1);
    fseek(fSrc, 0, SEEK_SET);
    fread(buf, length, 1, fSrc);
    
    fwrite(buf, length, 1, fDes);
    
    fclose(fSrc);
    fclose(fDes);
    delete [] buf;
    return true;
}

void OzgFileUtility::copyFolder(const char *src, const char *des)
{
    if(!OzgFileUtility::fileExists(src))
    {
        cout << "源文件夹不存在" << endl;
        return;
    }
    
    if(OzgFileUtility::fileExists(des))
        //删除旧的版本
        OzgFileUtility::deleteFile(des);
    
	 //建立空文件夹
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WIN8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//mac or ios or android or linux
   
    mkdir(des, S_IRWXU);
#else
	_wmkdir((wchar_t*)des);
#endif
    
    vector<string> fileList = OzgFileUtility::getFoldersAndFile(src);
    vector<string>::iterator fileListIterator = fileList.begin();
    while (fileListIterator != fileList.end())
    {
        string targetFileName = *fileListIterator;
        targetFileName.replace(0, strlen(src), des);
        
        if(OzgFileUtility::isFolder(*fileListIterator))
        {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WIN8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//mac or ios or android or linux
            //建立文件夹
            mkdir(targetFileName.c_str(), S_IRWXU);
#else
			_wmkdir((wchar_t*)targetFileName.c_str());
#endif

            vector<string> fileList2 = OzgFileUtility::getFoldersAndFile(*fileListIterator);
            vector<string>::iterator fileListIterator2 = fileList2.begin();
            while (fileListIterator2 != fileList2.end())
            {
                targetFileName = *fileListIterator2;
                targetFileName.replace(0, strlen(src), des);
                
                if(OzgFileUtility::isFolder(*fileListIterator2))
                {
                    OzgFileUtility::copyFolder((*fileListIterator2).c_str(), targetFileName.c_str());
                    //cout << *fileListIterator2 << endl;
                }
                else
                {
                    //复制文件
                    
                    OzgFileUtility::copyFile((*fileListIterator2).c_str(), targetFileName.c_str());
                    //cout << targetFileName << endl;
                }
                
                fileListIterator2++;
            }
            
        }
        else
        {
            //复制文件
            OzgFileUtility::copyFile((*fileListIterator).c_str(), targetFileName.c_str());
            //cout << targetFileName << endl;
        }
        
        fileListIterator++;
    }
    
}
