#include <hgl/type/ArrayRearrangeHelper.h>
#include <hgl/type/MemoryUtil.h>
#include <array>
#include <vector>
#include <iostream>
#include <new>
#include <stdexcept>
#include <chrono>
#include <random>
#include <sstream>

#define CHECK(cond, msg) do { if(!(cond)) { \
    std::ostringstream oss; \
    oss << msg << " (at line " << __LINE__ << ")"; \
    throw std::runtime_error(oss.str()); \
} } while(0)

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

struct Tracker
{
    static int constructed;
    static int destroyed;
    static int moved;

    int value;
    bool moved_from; // 标记是否已被移动

    explicit Tracker(int v = 0) : value(v), moved_from(false) { ++constructed; }
    Tracker(Tracker&& other) noexcept : value(other.value), moved_from(false)
    {
        other.moved_from = true; // 标记源对象已被移动，但不修改value
        ++constructed;
        ++moved;
    }
    Tracker(const Tracker&) = delete;
    Tracker& operator=(const Tracker&) = delete;
    Tracker& operator=(Tracker&& other) noexcept
    {
        value = other.value;
        moved_from = false;
        other.moved_from = true;
        ++moved;
        return *this;
    }
    ~Tracker() { ++destroyed; }

    static void Reset() { constructed = destroyed = moved = 0; }
    static int Alive() { return constructed - destroyed; }
};

int Tracker::constructed = 0;
int Tracker::destroyed = 0;
int Tracker::moved = 0;

void TestTrivialRearrange()
{
    std::cout << "[TestTrivialRearrange] Running..." << std::endl;

    // 基础小规模测试
    {
        constexpr int count = 5;
        const int src[count] = {1, 2, 3, 4, 5};
        int* dest = hgl::alloc_raw<int>(count);

        const bool ok = hgl::ArrayRearrange(dest, src, count, {2, 3}, {1, 0});
        CHECK(ok, "trivial rearrange failed");

        const int expected[count] = {3, 4, 5, 1, 2};
        for(int i = 0; i < count; ++i)
            CHECK(dest[i] == expected[i], "trivial rearrange result mismatch");

        hgl::free_raw(dest);
    }

    // 大规模重排测试
    for(int scale : {100, 1000, 10000})
    {
        std::vector<int> src(scale);
        for(int i = 0; i < scale; ++i)
            src[i] = i;

        int* dest = hgl::alloc_raw<int>(scale);

        // 测试多种重排模式
        // 模式1: 将数组分成两半并交换
        {
            int half = scale / 2;
            Timer timer;
            bool ok = hgl::ArrayRearrange(dest, src.data(), scale,
                                         {half, scale - half}, {1, 0});
            double elapsed = timer.ElapsedMs();

            CHECK(ok, "Large scale rearrange (swap halves) failed");

            // 验证：前半部分应该是原来的后半部分
            for(int i = 0; i < half; ++i)
                CHECK(dest[i] == src[half + i], "Swap halves verification failed");

            // 验证：后半部分应该是原来的前半部分
            for(int i = half; i < scale; ++i)
                CHECK(dest[i] == src[i - half], "Swap halves verification failed");

            std::cout << "  [Scale " << scale << "] Swap halves: " << elapsed << " ms" << std::endl;
        }

        // 模式2: 逆序排列
        {
            // 创建逆序的字段列表和索引
            std::vector<hgl::int64> fields;
            std::vector<hgl::int64> indices;
            for(int i = 0; i < scale; ++i)
            {
                fields.push_back(1);
                indices.push_back(scale - 1 - i);
            }

            Timer timer;
            hgl::ArrayRearrangeHelper helper(scale, scale);
            for(auto f : fields)
                helper.AddField(f);
            bool ok = helper.Rearrange(dest, src.data(), indices.data());
            double elapsed = timer.ElapsedMs();

            CHECK(ok, "Large scale reverse rearrange failed");

            // 验证逆序
            for(int i = 0; i < scale; ++i)
                CHECK(dest[i] == src[scale - 1 - i], "Reverse order verification failed");

            std::cout << "  [Scale " << scale << "] Reverse order: " << elapsed << " ms" << std::endl;
        }

        hgl::free_raw(dest);
    }

    std::cout << "[TestTrivialRearrange] Passed" << std::endl;
}

void TestNonTrivialRearrange()
{
    std::cout << "[TestNonTrivialRearrange] Running..." << std::endl;

    // 基础小规模测试
    {
        Tracker::Reset();
        constexpr int count = 5;
        Tracker src[count] = {Tracker{0}, Tracker{1}, Tracker{2}, Tracker{3}, Tracker{4}};

        Tracker* dest = hgl::alloc_raw<Tracker>(count);

        const bool ok = hgl::ArrayRearrange(dest, src, count, {1, 2, 2}, {0, 2, 1});
        CHECK(ok, "non-trivial rearrange failed");

        const int expected[count] = {0, 3, 4, 1, 2};
        for(int i = 0; i < count; ++i)
            CHECK(dest[i].value == expected[i], "non-trivial rearrange result mismatch");

        // After constructing dest, alive objects include both src and dest
        CHECK(Tracker::Alive() == 10, "alive count after rearrange unexpected");

        hgl::destroy_range(dest, count);
        hgl::free_raw(dest);
        CHECK(Tracker::Alive() == 5, "alive count after destroying dest unexpected");
    }

    // 大规模非平凡类型重排测试
    for(int scale : {100, 1000, 10000})
    {
        Tracker::Reset();

        // 创建源数组
        std::vector<Tracker> src_vec;
        src_vec.reserve(scale);
        for(int i = 0; i < scale; ++i)
            src_vec.emplace_back(i);

        int initial_constructed = Tracker::constructed;
        CHECK(Tracker::Alive() == scale, "Source array construction check failed");

        Tracker* dest = hgl::alloc_raw<Tracker>(scale);

        // 测试：将数组分成4段并打乱顺序
        int quarter = scale / 4;
        Timer timer;
        bool ok = hgl::ArrayRearrange(dest, src_vec.data(), scale,
                                     {quarter, quarter, quarter, scale - quarter * 3},
                                     {2, 0, 3, 1});
        double elapsed = timer.ElapsedMs();

        CHECK(ok, "Large scale non-trivial rearrange failed");

        // 验证重排结果
        // 索引映射：{2, 0, 3, 1} 意味着：
        // dest的第0段 = src的第2段
        // dest的第1段 = src的第0段
        // dest的第2段 = src的第3段
        // dest的第3段 = src的第1段

        int error_count = 0;
        int dest_pos = 0;
        std::vector<int> src_segment_order = {2, 0, 3, 1};
        std::vector<int> segment_sizes = {quarter, quarter, quarter, scale - quarter * 3};

        for(size_t seg = 0; seg < src_segment_order.size(); ++seg) {
            int src_seg_idx = src_segment_order[seg];
            int seg_size = segment_sizes[seg];

            // 计算源段的起始位置
            int src_start = 0;
            for(int i = 0; i < src_seg_idx; ++i)
                src_start += segment_sizes[i];

            // 验证这一段
            for(int i = 0; i < seg_size; ++i) {
                if(dest[dest_pos + i].value != src_vec[src_start + i].value)
                    error_count++;
            }
            dest_pos += seg_size;
        }

        CHECK(error_count == 0, "Non-trivial rearrange verification failed");

        // 验证对象计数：src + dest
        int expected_alive = scale * 2;
        if(Tracker::Alive() != expected_alive) {
            std::ostringstream oss;
            oss << "Alive count mismatch: expected " << expected_alive
                << ", got " << Tracker::Alive();
            throw std::runtime_error(oss.str());
        }

        std::cout << "  [Scale " << scale << "] Non-trivial rearrange: " << elapsed
                  << " ms (" << Tracker::moved << " moves, "
                  << Tracker::Alive() << " objects alive)" << std::endl;

        // 清理
        hgl::destroy_range(dest, scale);
        hgl::free_raw(dest);

        // 验证只剩下src对象
        CHECK(Tracker::Alive() == scale, "Memory leak detected after cleanup");
    }

    std::cout << "[TestNonTrivialRearrange] Passed" << std::endl;
}

void TestInvalidIndexEarlyExit()
{
    std::cout << "[TestInvalidIndexEarlyExit] Running..." << std::endl;

    Tracker::Reset();
    constexpr int count = 3;
    Tracker src[count] = {Tracker{10}, Tracker{11}, Tracker{12}};

    Tracker* dest = hgl::alloc_raw<Tracker>(count);
    const bool ok = hgl::ArrayRearrange(dest, src, count, {1, 1}, {-1});
    CHECK(!ok, "invalid index should fail");

    // No objects were constructed into dest, so only src are alive
    CHECK(Tracker::Alive() == 3, "alive count should reflect only source objects");

    hgl::free_raw(dest);

    std::cout << "[TestInvalidIndexEarlyExit] Passed" << std::endl;
}

void TestBoundaryConditions()
{
    std::cout << "[TestBoundaryConditions] Running..." << std::endl;

    // 空数组测试
    {
        int* src = nullptr;
        int* dest = nullptr;
        hgl::ArrayRearrangeHelper helper(0, 0);
        bool ok = helper.Finish();
        CHECK(ok, "Empty array should succeed");
    }

    // 单元素数组
    {
        int src[1] = {42};
        int* dest = hgl::alloc_raw<int>(1);
        bool ok = hgl::ArrayRearrange(dest, src, 1, {1}, {0});
        CHECK(ok, "Single element rearrange failed");
        CHECK(dest[0] == 42, "Single element value check failed");
        hgl::free_raw(dest);
    }

    // 两元素交换
    {
        int src[2] = {1, 2};
        int* dest = hgl::alloc_raw<int>(2);
        bool ok = hgl::ArrayRearrange(dest, src, 2, {1, 1}, {1, 0});
        CHECK(ok, "Two element swap failed");
        CHECK(dest[0] == 2 && dest[1] == 1, "Swap verification failed");
        hgl::free_raw(dest);
    }

    // 测试字段数量不匹配
    {
        int src[5] = {1, 2, 3, 4, 5};
        int* dest = hgl::alloc_raw<int>(5);
        // 故意使用错误的字段数量
        hgl::ArrayRearrangeHelper helper(5, 2);
        helper.AddField(2);
        // 第二个字段会自动包含剩余的3个元素
        bool ok = helper.Finish();
        CHECK(ok, "Auto-finish remaining elements failed");
        hgl::free_raw(dest);
    }

    std::cout << "[TestBoundaryConditions] Passed" << std::endl;
}

void TestStressRearrange()
{
    std::cout << "[TestStressRearrange] Running..." << std::endl;

    // 压力测试：随机重排模式
    std::random_device rd;
    std::mt19937 gen(rd());

    for(int scale : {100, 1000})
    {
        std::vector<int> src(scale);
        for(int i = 0; i < scale; ++i)
            src[i] = i;

        int* dest = hgl::alloc_raw<int>(scale);

        // 生成随机字段划分
        int num_fields = 5 + (gen() % 10); // 5-14个字段
        std::vector<hgl::int64> fields;
        std::vector<hgl::int64> indices;

        int remaining = scale;
        for(int i = 0; i < num_fields - 1; ++i) {
            int field_size = 1 + (gen() % (remaining / (num_fields - i)));
            fields.push_back(field_size);
            remaining -= field_size;
        }
        fields.push_back(remaining); // 最后一个字段包含所有剩余元素

        // 生成随机索引序列（0到num_fields-1的排列）
        for(int i = 0; i < num_fields; ++i)
            indices.push_back(i);
        std::shuffle(indices.begin(), indices.end(), gen);

        Timer timer;
        hgl::ArrayRearrangeHelper helper(scale, num_fields);
        for(auto f : fields)
            helper.AddField(f);
        bool ok = helper.Rearrange(dest, src.data(), indices.data());
        double elapsed = timer.ElapsedMs();

        CHECK(ok, "Random stress rearrange failed");

        // 验证：统计每个值出现的次数（应该都是1）
        std::vector<int> counts(scale, 0);
        for(int i = 0; i < scale; ++i) {
            if(dest[i] >= 0 && dest[i] < scale)
                counts[dest[i]]++;
        }

        for(int i = 0; i < scale; ++i)
            CHECK(counts[i] == 1, "Element duplication or loss detected");

        std::cout << "  [Scale " << scale << "] Random " << num_fields
                  << "-way rearrange: " << elapsed << " ms" << std::endl;

        hgl::free_raw(dest);
    }

    std::cout << "[TestStressRearrange] Passed" << std::endl;
}

int main()
{
    std::cout << "=========================================" << std::endl;
    std::cout << "ArrayRearrangeHelper Comprehensive Tests" << std::endl;
    std::cout << "Testing scales: 100, 1000, 10000" << std::endl;
    std::cout << "=========================================" << std::endl;

    try
    {
        TestTrivialRearrange();
        std::cout << std::endl;

        TestNonTrivialRearrange();
        std::cout << std::endl;

        TestInvalidIndexEarlyExit();
        std::cout << std::endl;

        TestBoundaryConditions();
        std::cout << std::endl;

        TestStressRearrange();
        std::cout << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "=========================================" << std::endl;
    std::cout << "All ArrayRearrangeHelper tests PASSED!" << std::endl;
    std::cout << "=========================================" << std::endl;
    return 0;
}
