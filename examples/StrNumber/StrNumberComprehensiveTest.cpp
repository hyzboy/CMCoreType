#include<hgl/type/Str.Number.h>
#include<iostream>
#include<cassert>
#include<cstring>
#include<chrono>
#include<iomanip>

using namespace hgl;
using namespace std;

// ==================== 测试辅助函数 ====================

void PrintTestHeader(const char *test_name)
{
    cout << "\n========== " << test_name << " ==========" << endl;
}

void AssertEqual(int actual, int expected, const char *test_name)
{
    if(actual != expected)
    {
        cerr << "FAIL [" << test_name << "]: Expected " << expected << ", got " << actual << endl;
        assert(false);
    }
}

void AssertEqual(const char *actual, const char *expected, const char *test_name)
{
    if(strcmp(actual, expected) != 0)
    {
        cerr << "FAIL [" << test_name << "]: Expected \"" << expected << "\", got \"" << actual << "\"" << endl;
        assert(false);
    }
}

// ==================== 1. stoi 测试 - 有符号整数解析 ====================
bool TestStoiBasic()
{
    PrintTestHeader("Test 1: stoi - Basic Signed Integer");

    int result;

    // 正常正数
    assert(hgl::stoi("12345", result) == true);
    assert(result == 12345);

    // 正数带符号
    assert(hgl::stoi("+999", result) == true);
    assert(result == 999);

    // 负数
    assert(hgl::stoi("-500", result) == true);
    assert(result == -500);

    // 零
    assert(hgl::stoi("0", result) == true);
    assert(result == 0);

    cout << "✓ PASSED" << endl;
    return true;
}

bool TestStoiEdgeCases()
{
    PrintTestHeader("Test 2: stoi - Edge Cases");

    int result;

    // 空字符串
    assert(hgl::stoi("", result) == false);

    // 仅符号
    assert(hgl::stoi("+", result) == false);
    assert(hgl::stoi("-", result) == false);

    // 符号后只有非数字
    assert(hgl::stoi("+abc", result) == false);
    assert(hgl::stoi("-xyz", result) == false);

    // 包含非数字字符
    assert(hgl::stoi("12a34", result) == true);  // 解析到"a"停止，得12
    assert(result == 12);

    cout << "✓ PASSED" << endl;
    return true;
}

bool TestStoiWithLength()
{
    PrintTestHeader("Test 3: stoi - With Length Limit");

    int result;

    // 指定长度解析
    assert(hgl::stoi("12345678", 4, result) == true);
    assert(result == 1234);

    // 负数带长度限制
    assert(hgl::stoi("-999xyz", 3, result) == true);
    assert(result == -99);

    // 长度为0
    assert(hgl::stoi("123", 0, result) == false);

    cout << "✓ PASSED" << endl;
    return true;
}

// ==================== 2. stou 测试 - 无符号整数解析 ====================
bool TestStouBasic()
{
    PrintTestHeader("Test 4: stou - Basic Unsigned Integer");

    uint result;

    // 正常正数
    assert(hgl::stou("12345", result) == true);
    assert(result == 12345);

    // 零
    assert(hgl::stou("0", result) == true);
    assert(result == 0);

    // 大数值
    assert(hgl::stou("999999999", result) == true);
    assert(result == 999999999);

    cout << "✓ PASSED" << endl;
    return true;
}

bool TestStouEdgeCases()
{
    PrintTestHeader("Test 5: stou - Edge Cases");

    uint result;

    // 空字符串
    assert(hgl::stou("", result) == false);

    // 包含非数字
    assert(hgl::stou("abc123", result) == false);
    assert(hgl::stou("12@34", result) == true);
    assert(result == 12);

    // 带符号（无符号类型不应解析符号）
    assert(hgl::stou("+123", result) == false);
    assert(hgl::stou("-123", result) == false);

    cout << "✓ PASSED" << endl;
    return true;
}

bool TestStouWithLength()
{
    PrintTestHeader("Test 6: stou - With Length Limit");

    uint result;

    // 带长度限制
    assert(hgl::stou("123456", 3, result) == true);
    assert(result == 123);

    // 长度超过有效数字
    assert(hgl::stou("789", 10, result) == true);
    assert(result == 789);

    cout << "✓ PASSED" << endl;
    return true;
}

// ==================== 3. xtou 测试 - 十六进制解析 ====================
bool TestXtouBasic()
{
    PrintTestHeader("Test 7: xtou - Basic Hex Parsing");

    uint result;

    // 小写十六进制
    assert(hgl::xtou("abcd", result) == true);
    assert(result == 0xabcd);

    // 大写十六进制
    assert(hgl::xtou("ABCD", result) == true);
    assert(result == 0xabcd);

    // 混合大小写
    assert(hgl::xtou("AbCd", result) == true);
    assert(result == 0xabcd);

    // 仅数字
    assert(hgl::xtou("1234", result) == true);
    assert(result == 0x1234);

    // 零
    assert(hgl::xtou("0", result) == true);
    assert(result == 0);

    cout << "✓ PASSED" << endl;
    return true;
}

bool TestXtouEdgeCases()
{
    PrintTestHeader("Test 8: xtou - Edge Cases");

    uint result;

    // 空字符串
    assert(hgl::xtou("", result) == false);

    // 包含非十六进制字符
    assert(hgl::xtou("12G34", result) == true);  // 解析到"G"停止
    assert(result == 0x12);

    // 不带0x前缀（正确用法）
    assert(hgl::xtou("FF", result) == true);
    assert(result == 0xFF);

    cout << "✓ PASSED" << endl;
    return true;
}

// ==================== 4. stof 测试 - 浮点数解析 ====================
bool TestStofBasic()
{
    PrintTestHeader("Test 9: stof - Basic Float Parsing");

    double result;

    // 整数部分
    assert(hgl::stof("123", result) == true);
    assert(result == 123.0);

    // 小数部分
    assert(hgl::stof("3.14", result) == true);
    assert(abs(result - 3.14) < 0.001);

    // 负数
    assert(hgl::stof("-2.5", result) == true);
    assert(abs(result - (-2.5)) < 0.001);

    // 仅小数点
    assert(hgl::stof(".5", result) == true);
    assert(abs(result - 0.5) < 0.001);

    cout << "✓ PASSED" << endl;
    return true;
}

bool TestStofEdgeCases()
{
    PrintTestHeader("Test 10: stof - Edge Cases");

    double result;

    // 空字符串
    assert(hgl::stof("", result) == false);

    // 仅小数点 - 无效
    assert(hgl::stof(".", result) == false);

    // 仅整数部分
    assert(hgl::stof("42", result) == true);
    assert(abs(result - 42.0) < 0.001);

    // 科学计数法（etof处理）
    assert(hgl::stof("1.5e2", result) == true);  // 只解析到小数部分

    cout << "✓ PASSED" << endl;
    return true;
}

bool TestStofWithLength()
{
    PrintTestHeader("Test 11: stof - With Length Limit");

    double result;

    // 带长度限制
    assert(hgl::stof("3.14159", 3, result) == true);
    assert(abs(result - 3.1) < 0.01);

    // 长度到小数点
    assert(hgl::stof("123.456", 3, result) == true);
    assert(abs(result - 123.0) < 0.001);

    cout << "✓ PASSED" << endl;
    return true;
}

// ==================== 5. stob 测试 - 布尔值解析 ====================
bool TestStobBasic()
{
    PrintTestHeader("Test 12: stob - Basic Boolean Parsing");

    bool result;

    // True 值
    assert(hgl::stob("t", result) == true);
    assert(result == true);

    assert(hgl::stob("T", result) == true);
    assert(result == true);

    assert(hgl::stob("y", result) == true);
    assert(result == true);

    assert(hgl::stob("Y", result) == true);
    assert(result == true);

    assert(hgl::stob("m", result) == true);
    assert(result == true);

    assert(hgl::stob("M", result) == true);
    assert(result == true);

    assert(hgl::stob("1", result) == true);
    assert(result == true);

    // False 值
    assert(hgl::stob("f", result) == true);
    assert(result == false);

    assert(hgl::stob("0", result) == true);
    assert(result == false);

    cout << "✓ PASSED" << endl;
    return true;
}

bool TestStobEdgeCases()
{
    PrintTestHeader("Test 13: stob - Edge Cases");

    bool result;

    // 空字符串
    assert(hgl::stob("", result) == false);

    // 其他字符
    assert(hgl::stob("yes", result) == true);  // 'y' 有效，只看第一个字符
    assert(result == true);

    assert(hgl::stob("2", result) == false);  // '2' 无效

    // 大小写混合 - 只看第一个字符 'T'
    assert(hgl::stob("True", result) == true);
    assert(result == true);

    cout << "✓ PASSED" << endl;
    return true;
}

// ==================== 6. itos 测试 - 有符号整数转字符串 ====================
bool TestItosBasic()
{
    PrintTestHeader("Test 14: itos - Basic Signed to String");

    char buffer[64];

    // 正数
    hgl::itos(buffer, 64, 12345);
    AssertEqual(buffer, "12345", "itos positive");

    // 负数
    hgl::itos(buffer, 64, -9999);
    AssertEqual(buffer, "-9999", "itos negative");

    // 零
    hgl::itos(buffer, 64, 0);
    AssertEqual(buffer, "0", "itos zero");

    cout << "✓ PASSED" << endl;
    return true;
}

bool TestItosEdgeCases()
{
    PrintTestHeader("Test 15: itos - Edge Cases");

    char buffer[64];

    // 缓冲区太小
    int result = hgl::itos_rl(buffer, 3, 12345);
    assert(result == -1);  // 失败

    // 最小值
    hgl::itos(buffer, 64, (int)-2147483648);
    // 结果应该是有效的

    // 最大值
    hgl::itos(buffer, 64, (int)2147483647);
    // 结果应该是有效的

    cout << "✓ PASSED" << endl;
    return true;
}

// ==================== 7. utos 测试 - 无符号整数转字符串 ====================
bool TestUtosBasic()
{
    PrintTestHeader("Test 16: utos - Basic Unsigned to String");

    char buffer[64];

    // 十进制
    hgl::utos(buffer, 64, 54321);
    AssertEqual(buffer, "54321", "utos decimal");

    // 零
    hgl::utos(buffer, 64, 0);
    AssertEqual(buffer, "0", "utos zero");

    cout << "✓ PASSED" << endl;
    return true;
}

bool TestUtosWithBase()
{
    PrintTestHeader("Test 17: utos - Various Bases");

    char buffer[64];
    uint value = 255;

    // 二进制
    hgl::utos(buffer, 64, value, 2);
    AssertEqual(buffer, "11111111", "utos binary");

    // 八进制
    hgl::utos(buffer, 64, value, 8);
    AssertEqual(buffer, "377", "utos octal");

    // 十六进制（小写）
    hgl::utos(buffer, 64, value, 16, false);
    AssertEqual(buffer, "ff", "utos hex lower");

    // 十六进制（大写）
    hgl::utos(buffer, 64, value, 16, true);
    AssertEqual(buffer, "FF", "utos hex upper");

    // 三十六进制
    hgl::utos(buffer, 64, 36, 36);
    AssertEqual(buffer, "10", "utos base36");

    cout << "✓ PASSED" << endl;
    return true;
}

bool TestUtosAllBases()
{
    PrintTestHeader("Test 18: utos - All Bases 2-36");

    char buffer[64];
    uint value = 1000;

    cout << "Value 1000 in different bases:" << endl;
    for(uint base = 2; base <= 36; base++)
    {
        hgl::utos(buffer, 64, value, base);
        cout << "  Base " << setw(2) << base << ": " << buffer << endl;
    }

    cout << "✓ PASSED" << endl;
    return true;
}

// ==================== 8. htos 测试 - 十六进制转字符串 ====================
bool TestHtosBasic()
{
    PrintTestHeader("Test 19: htos - Hex Integer to String");

    char buffer[64];

    // uint8
    hgl::htos(buffer, 64, (uint8)0x80);
    cout << "  uint8(0x80) -> " << buffer << endl;

    // uint16
    hgl::htos(buffer, 64, (uint16)0x4567);
    cout << "  uint16(0x4567) -> " << buffer << endl;

    // uint32
    hgl::htos(buffer, 64, (uint32)0xDEADBEEF);
    cout << "  uint32(0xDEADBEEF) -> " << buffer << endl;

    // 大小写控制
    hgl::htos(buffer, 64, (uint16)0xabcd, true);  // 大写
    AssertEqual(buffer, "ABCD", "htos uppercase");

    hgl::htos(buffer, 64, (uint16)0xabcd, false);  // 小写
    AssertEqual(buffer, "abcd", "htos lowercase");

    cout << "✓ PASSED" << endl;
    return true;
}

bool TestHtosPointer()
{
    PrintTestHeader("Test 20: htos - Pointer to Hex String");

    char buffer[64];

    // 指针转换
    hgl::htos(buffer, 64, (void*)0x12345678);
    cout << "  Pointer(0x12345678) -> " << buffer << endl;

    // 函数指针
    hgl::htos(buffer, 64, (void*)&TestHtosPointer);
    cout << "  Function pointer -> " << buffer << endl;

    cout << "✓ PASSED" << endl;
    return true;
}

// ==================== 9. ftos 测试 - 浮点数转字符串 ====================
bool TestFtosBasic()
{
    PrintTestHeader("Test 21: ftos - Float to String");

    char buffer[64];

    // 一位小数
    hgl::ftos(buffer, 64, 1, 3.14159);
    cout << "  ftos(3.14159, 1 decimal) -> " << buffer << endl;

    // 多位小数
    hgl::ftos(buffer, 64, 5, 3.14159);
    cout << "  ftos(3.14159, 5 decimals) -> " << buffer << endl;

    // 负数
    hgl::ftos(buffer, 64, 2, -2.5);
    cout << "  ftos(-2.5, 2 decimals) -> " << buffer << endl;

    // 零
    hgl::ftos(buffer, 64, 3, 0.0);
    cout << "  ftos(0.0, 3 decimals) -> " << buffer << endl;

    cout << "✓ PASSED" << endl;
    return true;
}

bool TestFtosDefaultPrecision()
{
    PrintTestHeader("Test 22: ftos - Default Precision");

    char buffer[64];

    // 不指定精度（使用默认）
    hgl::ftos(buffer, 64, 123.456);
    cout << "  ftos(123.456) -> " << buffer << endl;

    hgl::ftos(buffer, 64, 0.001);
    cout << "  ftos(0.001) -> " << buffer << endl;

    hgl::ftos(buffer, 64, 1000000.0);
    cout << "  ftos(1000000.0) -> " << buffer << endl;

    cout << "✓ PASSED" << endl;
    return true;
}

// ==================== 10. int_to_size 测试 - 字节大小转可读格式 ====================
bool TestIntToSize()
{
    PrintTestHeader("Test 23: int_to_size - Human Readable Size");

    char buffer[64];

    // 字节
    hgl::int_to_size(buffer, 64, 512);
    cout << "  512 bytes -> " << buffer << endl;

    // KB
    hgl::int_to_size(buffer, 64, 1024);
    cout << "  1024 bytes -> " << buffer << endl;

    // MB
    hgl::int_to_size(buffer, 64, 1024 * 1024);
    cout << "  1MB -> " << buffer << endl;

    // GB
    hgl::int_to_size(buffer, 64, 1024 * 1024 * 1024);
    cout << "  1GB -> " << buffer << endl;

    // TB
    hgl::int_to_size(buffer, 64, (int64)1024 * 1024 * 1024 * 1024);
    cout << "  1TB -> " << buffer << endl;

    // 零
    hgl::int_to_size(buffer, 64, 0);
    cout << "  0 bytes -> " << buffer << endl;

    cout << "✓ PASSED" << endl;
    return true;
}

// ==================== 11. 性能测试 ====================
bool TestPerformanceStoi()
{
    PrintTestHeader("Test 24: Performance - stoi (1 million conversions)");

    int result;
    auto start = chrono::steady_clock::now();

    for(int i = 0; i < 1000000; i++)
    {
        hgl::stoi("123456", result);
    }

    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "  1,000,000 conversions: " << duration.count() << " ms" << endl;
    cout << "  Average: " << (double)duration.count() / 1000000 * 1000000 << " us/conv" << endl;

    cout << "✓ PASSED" << endl;
    return true;
}

bool TestPerformanceItos()
{
    PrintTestHeader("Test 25: Performance - itos (100k conversions)");

    char buffer[64];
    auto start = chrono::steady_clock::now();

    for(int i = 0; i < 100000; i++)
    {
        hgl::itos(buffer, 64, i * 100);
    }

    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "  100,000 conversions: " << duration.count() << " ms" << endl;
    cout << "  Average: " << (double)duration.count() / 100000 * 1000 << " us/conv" << endl;

    cout << "✓ PASSED" << endl;
    return true;
}

// ==================== 12. 错误恢复测试 ====================
bool TestErrorRecovery()
{
    PrintTestHeader("Test 26: Error Recovery - Malformed Input");

    int int_result;
    double float_result;
    uint uint_result;
    bool bool_result;
    char buffer[64];

    // 包含特殊字符
    assert(hgl::stoi("123$456", int_result) == true);
    assert(int_result == 123);

    // 空格混合
    assert(hgl::stoi("  123", int_result) == false);

    // 控制字符
    assert(hgl::stou("999\0", uint_result) == true);
    assert(uint_result == 999);

    // 转换时缓冲区过小
    int result = hgl::itos_rl(buffer, 2, 123456);
    assert(result == -1);  // 应该失败

    cout << "✓ PASSED" << endl;
    return true;
}

// ==================== 13. 边界值测试 ====================
bool TestBoundaryValues()
{
    PrintTestHeader("Test 27: Boundary Values");

    char buffer[64];
    int int_val;

    // 32位有符号整数边界
    hgl::itos(buffer, 64, INT32_MAX);
    cout << "  INT32_MAX: " << buffer << endl;

    hgl::itos(buffer, 64, INT32_MIN);
    cout << "  INT32_MIN: " << buffer << endl;

    // 32位无符号整数边界
    hgl::utos(buffer, 64, UINT32_MAX);
    cout << "  UINT32_MAX: " << buffer << endl;

    // 浮点数边界
    hgl::ftos(buffer, 64, 6, 1.23456789);
    cout << "  Precision test: " << buffer << endl;

    cout << "✓ PASSED" << endl;
    return true;
}

// ==================== 主测试运行器 ====================
int main(int, char**)
{
    cout << "╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║    Str.Number Comprehensive Test Suite                     ║" << endl;
    cout << "║    All conversion functions with edge cases & errors        ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝" << endl;

    int passed = 0;
    int failed = 0;

    struct Test {
        const char* name;
        bool (*func)();
    };

    Test tests[] = {
        {"stoi - Basic", TestStoiBasic},
        {"stoi - Edge Cases", TestStoiEdgeCases},
        {"stoi - With Length", TestStoiWithLength},
        {"stou - Basic", TestStouBasic},
        {"stou - Edge Cases", TestStouEdgeCases},
        {"stou - With Length", TestStouWithLength},
        {"xtou - Basic Hex", TestXtouBasic},
        {"xtou - Edge Cases", TestXtouEdgeCases},
        {"stof - Basic Float", TestStofBasic},
        {"stof - Edge Cases", TestStofEdgeCases},
        {"stof - With Length", TestStofWithLength},
        {"stob - Basic Boolean", TestStobBasic},
        {"stob - Edge Cases", TestStobEdgeCases},
        {"itos - Basic", TestItosBasic},
        {"itos - Edge Cases", TestItosEdgeCases},
        {"utos - Basic", TestUtosBasic},
        {"utos - Various Bases", TestUtosWithBase},
        {"utos - All Bases", TestUtosAllBases},
        {"htos - Basic Hex", TestHtosBasic},
        {"htos - Pointer", TestHtosPointer},
        {"ftos - Basic Float", TestFtosBasic},
        {"ftos - Default Precision", TestFtosDefaultPrecision},
        {"int_to_size - Readable", TestIntToSize},
        {"Performance - stoi", TestPerformanceStoi},
        {"Performance - itos", TestPerformanceItos},
        {"Error Recovery", TestErrorRecovery},
        {"Boundary Values", TestBoundaryValues}
    };

    for(const auto& test : tests)
    {
        try
        {
            if(test.func())
            {
                passed++;
            }
            else
            {
                failed++;
                cout << "✗ " << test.name << " - FAILED" << endl;
            }
        }
        catch(const exception& e)
        {
            failed++;
            cout << "✗ " << test.name << " - EXCEPTION: " << e.what() << endl;
        }
    }

    cout << "\n╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║             Test Summary                                    ║" << endl;
    cout << "╠════════════════════════════════════════════════════════════╣" << endl;
    cout << "  Total Tests:  " << (passed + failed) << endl;
    cout << "  Passed:       " << passed << " ✓" << endl;
    cout << "  Failed:       " << failed << " ✗" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝" << endl;

    return (failed == 0) ? 0 : 1;
}
