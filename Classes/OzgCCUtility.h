
#ifndef __ozgcc2dxutils__OzgCCUtility__
#define __ozgcc2dxutils__OzgCCUtility__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//(注意：对工程右键-> 属性 -> 连接器 -> 输入 -> 附加依赖项    栏目->后面有个按钮，点击打开，换一行加入libiconv.lib，或者在最后空一格加上libiconv.lib也行）
#include "iconv\iconv.h" 
#endif  

#define OZG_BASE64 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="

USING_NS_CC;
using namespace std;

static unsigned int OzgCCUtilsRandomSeed = 0;

//string转数字类型
/*
 string str("00801");
 cout << stringToNumber<int>(str) << endl;
 */
template <class Type>
Type stringToNumber(const string& str)
{
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}

class OzgCCUtility
{
        
public:
    
    //draw实心圆,参数为：中心点，半径，线段数（越大锯齿就越少），颜色
    static void ccDrawSolidCircle(const CCPoint& center, float radius, unsigned int segments, ccColor4F color);
    
    static void clearAnimate(const char *plist); //清空plist生成的帧动画
    static CCActionInterval* createAnimate(const char *plist); //利用plist生成帧动画，plist结构在实现文件里面
    
    static void plistPosition(CCNode *node, const char *plist); //使用plist来定位
    
    static unsigned int rangeRand(unsigned int min, unsigned int max); //范围随机数
    
    //随机例子，第一次调用randomFloat的值都是一样的，所以第一次的值可以忽略
//    OzgCCUtils::randomSeed(0);
//    for (int i = 0; i < 10; i++)
//    {
//        CCLog("%f, %f", OzgCCUtils::randomFloat(0.0f, 3.0f), OzgCCUtils::randomFloat(0.0f, 3.0f));
//        sleep(1);
//    }
    static void randomSeed(int seed);
    static float randomFloat(float min, float max); //范围随机float
    
    static int atoi(const char *src); //atoi的加强版，字符转整数
    static char* itoa(int num, char *str); //整数转字符
    static char* formatTime(const char* format); //返回格式化的时间，参数1 demos:"%Y-%m-%d %H:%M:%S"
    static char* formatTime(const char* format, time_t utc_time);

    /*
     base64 demo
     
     const char *t = "this is ozg demo";
     int j = strlen(t);
     char *enc = OzgCCUtils::base64Encode(t, j);
     int len = strlen(enc);
     char *dec = OzgCCUtils::base64Decode(enc, len);
     printf("\noriginal: %s\n", t);
     printf("\nencoded : %s\n", enc);
     printf("\ndecoded : %s\n", dec);
     free(enc);
     free(dec);
     
     */
    static char *base64Encode(const char* data, int data_len);
    static char base64FindPos(char ch);
    static char *base64Decode(const char *data, int data_len);
    
    static float roundf(float num); //四舍五入到整数
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	//windows的乱码解决方案
	static int GBKToUTF8(string &gbkStr,const char* toCode,const char* formCode);
#endif  
	static string generalString(string &str); //通用的乱码解决方案
    static string generalString(const char* str);
    
    static CCActionInterval* createAnimate(CCTexture2D* texture, int xNum, int yNum, vector<int> frameIndces, float delay, bool loops);
    static CCActionInterval* createAnimate(const char* fileimage, int xNum, int yNum, vector<int> frameIndces, float delay, bool loops);
    static CCArray* createSpriteFrames(CCTexture2D* texture, int xNum, int yNum);
    static CCArray* createSpriteFrames(const char* fileimage, int xNum, int yNum);
    
    
};

#endif /* defined(__ozgcc2dxutils__OzgCCUtility__) */
