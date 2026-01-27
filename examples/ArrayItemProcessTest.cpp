#include <hgl/type/ArrayItemProcess.h>
#include <array>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <random>
#include <algorithm>
#include <sstream>

#define CHECK(cond, msg) do { if(!(cond)) { \
    std::ostringstream oss; \
    oss << msg << " (at line " << __LINE__ << ")"; \
    throw std::runtime_error(oss.str()); \
} } while(0)

#define VERIFY_RANGE(vec1, vec2, start, end, msg) do { \
    for(int i = start; i < end; ++i) { \
        if(vec1[i] != vec2[i]) { \
            std::ostringstream oss; \
            oss << msg << " at index " << i << ": expected " << vec2[i] << ", got " << vec1[i]; \
            throw std::runtime_error(oss.str()); \
        } \
    } \
} while(0)

using namespace hgl;

struct Pod
{
    int a{};
    int b{};
    bool operator==(const Pod &other) const { return a == other.a && b == other.b; }
    bool operator<(const Pod &other) const
    {
        if(a != other.a) return a < other.a;
        return b < other.b;
    }
};

// 性能计时辅助类
class Timer
{
    std::chrono::high_resolution_clock::time_point start;
public:
    Timer() : start(std::chrono::high_resolution_clock::now()) {}
    double ElapsedMs() const
    {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }
};

void TestRawTypeCallback()
{
    std::cout << "[TestRawTypeCallback] Running..." << std::endl;
    RawTypeArrayItemProcessCallback<int> cb;

    // 基础单元素测试
    int dst_single = 0;
    cb.Equal(dst_single, 42);
    CHECK(dst_single == 42, "RawType Equal single failed");

    // 小规模批量测试
    std::array<int, 4> src = {1, 2, 3, 4};
    std::array<int, 4> dst = {0, 0, 0, 0};
    cb.Equal(dst.data(), src.data(), static_cast<int>(src.size()));
    for(size_t i = 0; i < src.size(); ++i)
        CHECK(dst[i] == src[i], "RawType Equal bulk failed");

    CHECK(cb.Comp(5, 3) > 0, "RawType Comp greater failed");
    CHECK(cb.Comp(3, 5) < 0, "RawType Comp less failed");
    CHECK(cb.Comp(7, 7) == 0, "RawType Comp equal failed");

    // 大规模批量复制测试：100、1000、10000
    for(int scale : {100, 1000, 10000})
    {
        std::vector<int> large_src(scale);
        std::vector<int> large_dst(scale, -1); // 使用-1初始化，确保真正复制了数据

        // 初始化源数据（使用多样化的模式）
        for(int i = 0; i < scale; ++i)
            large_src[i] = i * 7 + 13;

        // 验证目标数组确实是-1
        for(int i = 0; i < scale; ++i)
            CHECK(large_dst[i] == -1, "Initial dst corruption");

        Timer timer;
        cb.Equal(large_dst.data(), large_src.data(), scale);
        double elapsed = timer.ElapsedMs();

        // 完整验证：逐个检查所有元素
        int mismatch_count = 0;
        for(int i = 0; i < scale; ++i) {
            if(large_dst[i] != large_src[i]) {
                if(mismatch_count == 0) {
                    std::cerr << "  First mismatch at index " << i
                              << ": expected " << large_src[i]
                              << ", got " << large_dst[i] << std::endl;
                }
                mismatch_count++;
            }
        }
        CHECK(mismatch_count == 0, "RawType large scale copy validation failed");

        // 边界检查：验证首尾元素
        CHECK(large_dst[0] == large_src[0], "First element mismatch");
        CHECK(large_dst[scale-1] == large_src[scale-1], "Last element mismatch");

        std::cout << "  [Scale " << scale << "] Bulk copy: " << elapsed << " ms (all " << scale << " elements verified)" << std::endl;
    }

    // 边界测试：空数组、单元素
    std::vector<int> empty_src;
    std::vector<int> empty_dst;
    cb.Equal(empty_dst.data(), empty_src.data(), 0); // 不应崩溃

    std::vector<int> single_src = {999};
    std::vector<int> single_dst = {0};
    cb.Equal(single_dst.data(), single_src.data(), 1);
    CHECK(single_dst[0] == 999, "Single element copy failed");
    std::cout << "  Boundary tests (empty/single) passed" << std::endl;

    std::cout << "[TestRawTypeCallback] Passed" << std::endl;
}

void TestMemoryCallback()
{
    std::cout << "[TestMemoryCallback] Running..." << std::endl;
    MemoryArrayItemProcessCallback<Pod> cb;

    // 基础单元素测试
    Pod src_single{10, 20};
    Pod dst_single{};
    cb.Equal(dst_single, src_single);
    CHECK(dst_single == src_single, "Memory Equal single failed");

    // 小规模批量测试
    std::array<Pod, 3> src = {{{1, 2}, {3, 4}, {5, 6}}};
    std::array<Pod, 3> dst = {{{0, 0}, {0, 0}, {0, 0}}};
    cb.Equal(dst.data(), src.data(), static_cast<int>(src.size()));
    for(size_t i = 0; i < src.size(); ++i)
        CHECK(dst[i] == src[i], "Memory Equal bulk failed");

    Pod a{1, 1};
    Pod b{1, 2};
    CHECK(cb.Comp(a, a) == 0, "Memory Comp equal failed");
    CHECK(cb.Comp(a, b) < 0, "Memory Comp less failed");
    CHECK(cb.Comp(b, a) > 0, "Memory Comp greater failed");

    // 大规模POD复制测试（增强验证）
    for(int scale : {100, 1000, 10000})
    {
        std::vector<Pod> large_src(scale);
        std::vector<Pod> large_dst(scale, Pod{-999, -999}); // 用特殊值初始化

        // 初始化源数据（使用不同的模式）
        for(int i = 0; i < scale; ++i)
            large_src[i] = Pod{i, i * 2};

        // 验证目标数组初始状态
        CHECK(large_dst[0].a == -999 && large_dst[0].b == -999, "Dst init failed");

        Timer timer;
        cb.Equal(large_dst.data(), large_src.data(), scale);
        double elapsed = timer.ElapsedMs();

        // 逐个验证所有元素的两个字段
        int error_count = 0;
        for(int i = 0; i < scale; ++i) {
            if(large_dst[i].a != large_src[i].a || large_dst[i].b != large_src[i].b) {
                if(error_count == 0) {
                    std::cerr << "  First error at " << i
                              << ": expected {" << large_src[i].a << "," << large_src[i].b
                              << "}, got {" << large_dst[i].a << "," << large_dst[i].b << "}" << std::endl;
                }
                error_count++;
            }
        }
        CHECK(error_count == 0, "Memory large scale copy validation failed");

        // 随机抽查10个元素
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, scale - 1);
        for(int check = 0; check < 10; ++check) {
            int idx = dis(gen);
            CHECK(large_dst[idx] == large_src[idx], "Random spot check failed");
        }

        std::cout << "  [Scale " << scale << "] Pod bulk copy: " << elapsed
                  << " ms (verified + 10 random checks)" << std::endl;
    }

    std::cout << "[TestMemoryCallback] Passed" << std::endl;
}

void TestFindDataPositionInArray()
{
    std::cout << "[TestFindDataPositionInArray] Running..." << std::endl;

    // 基础小规模测试
    const int arr[] = {5, 10, 15, 20};
    CHECK(FindDataPositionInArray(arr, static_cast<int64>(4), 5) == 0, "FindDataPosition first failed");
    CHECK(FindDataPositionInArray(arr, static_cast<int64>(4), 15) == 2, "FindDataPosition middle failed");
    CHECK(FindDataPositionInArray(arr, static_cast<int64>(4), 30) == -1, "FindDataPosition not found failed");
    CHECK(FindDataPositionInArray<int>(nullptr, 4, 1) == -1, "FindDataPosition null failed");
    CHECK(FindDataPositionInArray(arr, static_cast<int64>(0), 5) == -1, "FindDataPosition zero count failed");

    // 大规模无序数组查找测试（增强验证）
    for(int scale : {100, 1000, 10000})
    {
        std::vector<int> data(scale);
        for(int i = 0; i < scale; ++i)
            data[i] = i * 3 + 7;

        Timer timer;
        // 查找存在的元素（首、中、尾）
        int64 pos_first = FindDataPositionInArray(data.data(), static_cast<int64>(scale), data[0]);
        int64 pos_mid = FindDataPositionInArray(data.data(), static_cast<int64>(scale), data[scale / 2]);
        int64 pos_last = FindDataPositionInArray(data.data(), static_cast<int64>(scale), data[scale - 1]);
        // 查找不存在的元素（最差情况，需要遍历整个数组）
        int64 pos_missing = FindDataPositionInArray(data.data(), static_cast<int64>(scale), -9999);

        // 随机查找20个存在的元素
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, scale - 1);
        for(int test = 0; test < 20; ++test) {
            int idx = dis(gen);
            int64 found_pos = FindDataPositionInArray(data.data(), static_cast<int64>(scale), data[idx]);
            if(found_pos != idx) {
                std::ostringstream oss;
                oss << "Random find failed: looking for data[" << idx << "]=" << data[idx]
                    << ", found at " << found_pos;
                throw std::runtime_error(oss.str());
            }
        }

        double elapsed = timer.ElapsedMs();

        CHECK(pos_first == 0, "Large scale find first failed");
        CHECK(pos_mid == scale / 2, "Large scale find mid failed");
        CHECK(pos_last == scale - 1, "Large scale find last failed");
        CHECK(pos_missing == -1, "Large scale find missing failed");

        std::cout << "  [Scale " << scale << "] Unsorted search: " << elapsed
                  << " ms (+ 20 random verifications)" << std::endl;
    }

    // 重复元素测试
    std::vector<int> dup_data = {5, 5, 5, 10, 10};
    int64 first_5 = FindDataPositionInArray(dup_data.data(), 5, 5);
    CHECK(first_5 == 0, "Duplicate element should return first occurrence");
    std::cout << "  Duplicate element test passed" << std::endl;

    std::cout << "[TestFindDataPositionInArray] Passed" << std::endl;
}

void TestFindDataPositionInSortedArray()
{
    std::cout << "[TestFindDataPositionInSortedArray] Running..." << std::endl;

    // 基础小规模测试
    const int arr[] = {2, 4, 6, 8, 10};
    CHECK(FindDataPositionInSortedArray(arr, static_cast<int64>(5), 2) == 0, "FindDataPositionSorted first failed");
    CHECK(FindDataPositionInSortedArray(arr, static_cast<int64>(5), 6) == 2, "FindDataPositionSorted middle failed");
    CHECK(FindDataPositionInSortedArray(arr, static_cast<int64>(5), 10) == 4, "FindDataPositionSorted last failed");
    CHECK(FindDataPositionInSortedArray(arr, static_cast<int64>(5), 5) == -1, "FindDataPositionSorted missing failed");

    // 大规模有序数组二分查找测试（严格验证）
    for(int scale : {100, 1000, 10000})
    {
        std::vector<int> sorted_data(scale);
        for(int i = 0; i < scale; ++i)
            sorted_data[i] = i * 2; // 偶数序列，保证有序

        // 验证数据确实有序
        for(int i = 1; i < scale; ++i)
            CHECK(sorted_data[i] > sorted_data[i-1], "Data not sorted!");

        Timer timer;
        // 查找存在的元素
        int64 pos_first = FindDataPositionInSortedArray(sorted_data.data(), static_cast<int64>(scale), 0);
        int64 pos_mid = FindDataPositionInSortedArray(sorted_data.data(), static_cast<int64>(scale), (scale / 2) * 2);
        int64 pos_last = FindDataPositionInSortedArray(sorted_data.data(), static_cast<int64>(scale), (scale - 1) * 2);

        // 随机查找30个存在的偶数
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, scale - 1);
        for(int test = 0; test < 30; ++test) {
            int idx = dis(gen);
            int value = sorted_data[idx];
            int64 found = FindDataPositionInSortedArray(sorted_data.data(), static_cast<int64>(scale), value);
            if(found != idx) {
                std::ostringstream oss;
                oss << "Binary search failed: value " << value
                    << " should be at " << idx << ", found at " << found;
                throw std::runtime_error(oss.str());
            }
        }

        // 查找不存在的奇数（应该都返回-1）
        for(int odd : {1, 3, 5, 7, 11, 13}) {
            int64 pos = FindDataPositionInSortedArray(sorted_data.data(), static_cast<int64>(scale), odd);
            CHECK(pos == -1, "Odd number should not be found");
        }

        double elapsed = timer.ElapsedMs();

        CHECK(pos_first == 0, "Large scale sorted find first failed");
        CHECK(pos_mid == scale / 2, "Large scale sorted find mid failed");
        CHECK(pos_last == scale - 1, "Large scale sorted find last failed");

        std::cout << "  [Scale " << scale << "] Binary search: " << elapsed
                  << " ms (+ 30 random + 6 missing checks)" << std::endl;
    }

    std::cout << "[TestFindDataPositionInSortedArray] Passed" << std::endl;
}

void TestFindInsertPositionInSortedArray()
{
    std::cout << "[TestFindInsertPositionInSortedArray] Running..." << std::endl;

    // 基础小规模测试
    const int arr[] = {1, 3, 5, 7};
    int64 pos = -1;

    bool exists = FindInsertPositionInSortedArray(&pos, arr, static_cast<int64>(4), 5);
    CHECK(exists == true && pos == 2, "FindInsert existing failed");

    exists = FindInsertPositionInSortedArray(&pos, arr, static_cast<int64>(4), 0);
    CHECK(exists == false && pos == 0, "FindInsert before first failed");

    exists = FindInsertPositionInSortedArray(&pos, arr, static_cast<int64>(4), 2);
    CHECK(exists == false && pos == 1, "FindInsert between failed");

    exists = FindInsertPositionInSortedArray(&pos, arr, static_cast<int64>(4), 9);
    CHECK(exists == false && pos == 4, "FindInsert after last failed");

    // 大规模插入位置查找测试
    for(int scale : {100, 1000, 10000})
    {
        std::vector<int> sorted_data(scale);
        for(int i = 0; i < scale; ++i)
            sorted_data[i] = i * 10; // 间隔较大，方便测试插入

        Timer timer;
        int64 insert_pos = -1;

        // 测试查找已存在元素
        bool found_exist = FindInsertPositionInSortedArray(&insert_pos, sorted_data.data(),
                                                           static_cast<int64>(scale), (scale / 2) * 10);
        CHECK(found_exist && insert_pos == scale / 2, "Large scale insert find existing failed");

        // 测试查找不存在的元素（需要插入）
        bool found_new = FindInsertPositionInSortedArray(&insert_pos, sorted_data.data(),
                                                         static_cast<int64>(scale), 5); // 5应该插入到位置1
        CHECK(!found_new && insert_pos == 1, "Large scale insert find new position failed");

        // 边界测试：比最小值还小
        found_new = FindInsertPositionInSortedArray(&insert_pos, sorted_data.data(),
                                                    static_cast<int64>(scale), -100);
        CHECK(!found_new && insert_pos == 0, "Large scale insert before first failed");

        // 边界测试：比最大值还大
        found_new = FindInsertPositionInSortedArray(&insert_pos, sorted_data.data(),
                                                    static_cast<int64>(scale), (scale + 100) * 10);
        CHECK(!found_new && insert_pos == scale, "Large scale insert after last failed");

        // 随机测试插入位置的正确性
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(-50, scale * 10 + 50);
        for(int test = 0; test < 20; ++test) {
            int test_value = dis(gen);
            int64 pos = -1;
            bool exists = FindInsertPositionInSortedArray(&pos, sorted_data.data(),
                                                          static_cast<int64>(scale), test_value);

            // 验证：如果exists=true，则sorted_data[pos]应该等于test_value
            if(exists) {
                if(pos < 0 || pos >= scale || sorted_data[pos] != test_value) {
                    std::ostringstream oss;
                    oss << "Insert position error: value=" << test_value
                        << " reported exists at pos=" << pos;
                    if(pos >= 0 && pos < scale)
                        oss << " but sorted_data[pos]=" << sorted_data[pos];
                    throw std::runtime_error(oss.str());
                }
            } else {
                // 如果不存在，验证插入位置的正确性
                if(pos < 0 || pos > scale) {
                    std::ostringstream oss;
                    oss << "Invalid insert position: " << pos << " for value " << test_value;
                    throw std::runtime_error(oss.str());
                }
                // 验证插入位置的相对关系
                if(pos > 0 && sorted_data[pos-1] >= test_value) {
                    std::ostringstream oss;
                    oss << "Insert position error: sorted_data[" << (pos-1) << "]="
                        << sorted_data[pos-1] << " >= test_value=" << test_value;
                    throw std::runtime_error(oss.str());
                }
                if(pos < scale && sorted_data[pos] <= test_value) {
                    std::ostringstream oss;
                    oss << "Insert position error: sorted_data[" << pos << "]="
                        << sorted_data[pos] << " <= test_value=" << test_value;
                    throw std::runtime_error(oss.str());
                }
            }
        }

        double elapsed = timer.ElapsedMs();
        std::cout << "  [Scale " << scale << "] Insert position search: " << elapsed
                  << " ms (+ 20 random position verifications)" << std::endl;
    }

    std::cout << "[TestFindInsertPositionInSortedArray] Passed" << std::endl;
}

int main()
{
    std::cout << "=====================================" << std::endl;
    std::cout << "ArrayItemProcess Comprehensive Tests" << std::endl;
    std::cout << "Testing scales: 100, 1000, 10000" << std::endl;
    std::cout << "=====================================" << std::endl;

    try
    {
        TestRawTypeCallback();
        std::cout << std::endl;

        TestMemoryCallback();
        std::cout << std::endl;

        TestFindDataPositionInArray();
        std::cout << std::endl;

        TestFindDataPositionInSortedArray();
        std::cout << std::endl;

        TestFindInsertPositionInSortedArray();
        std::cout << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "=====================================" << std::endl;
    std::cout << "All ArrayItemProcess tests PASSED!" << std::endl;
    std::cout << "=====================================" << std::endl;
    return 0;
}
