~~~c++
void UserDefault::initXMLFilePath()
{
    if (! _isFilePathInitialized)
    {
        _filePath += FileUtils::getInstance()->getWritablePath() + XML_FILE_NAME;
        _isFilePathInitialized = true;
    }    
}
~~~

打印

~~~c++
log("%s", FileUtils::getInstance()->getWritablePath().c_str());  
~~~

输出地址

`C:/Users/LIUYT/AppData/Local/SimpleGame/`