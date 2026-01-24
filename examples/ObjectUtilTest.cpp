/**
 * ObjectUtil 使用范例
 *
 * 本文件展示 ObjectUtil.h 中所有函数的正确用法。
 * ObjectUtil 提供了非平凡类型的底层内存管理工具，包括：
 * - 原始内存分配/释放（不调用构造/析构）
 * - 显式构造/析构函数调用
 * - 批量构造/析构操作
 * - 内存重新分配与对象移动
 *
 * 注意：这些是危险的底层操作！必须严格遵守以下规则：
 * 1. 分配的内存必须在使用前显式构造对象
 * 2. 销毁对象后必须释放内存
 * 3. 不能对同一内存位置重复构造或析构
 * 4. 移动后的对象仍需显式析构
 */

#include <hgl/type/ObjectUtil.h>
#include <iostream>
#include <string>

// 测试用的非平凡类型
class TestObject
{
private:
    int id_;
    std::string name_;

    static int construction_count_;
    static int destruction_count_;

public:
    // 默认构造函数
    TestObject() : id_(0), name_("default")
    {
        ++construction_count_;
        std::cout << "  [构造] TestObject() - id=" << id_ << ", name=\"" << name_ << "\"\n";
    }

    // 带参数的构造函数
    TestObject(int id, const std::string& name) : id_(id), name_(name)
    {
        ++construction_count_;
        std::cout << "  [构造] TestObject(" << id << ", \"" << name << "\")\n";
    }

    // 复制构造函数
    TestObject(const TestObject& other) : id_(other.id_), name_(other.name_)
    {
        ++construction_count_;
        std::cout << "  [复制构造] TestObject from id=" << other.id_ << "\n";
    }

    // 移动构造函数
    TestObject(TestObject&& other) noexcept : id_(other.id_), name_(std::move(other.name_))
    {
        ++construction_count_;
        std::cout << "  [移动构造] TestObject from id=" << other.id_ << "\n";
        other.id_ = -1; // 标记已移动
    }

    // 析构函数
    ~TestObject()
    {
        ++destruction_count_;
        std::cout << "  [析构] ~TestObject() - id=" << id_ << ", name=\"" << name_ << "\"\n";
    }

    // 获取信息
    int GetId() const { return id_; }
    const std::string& GetName() const { return name_; }

    // 静态方法：获取统计信息
    static int GetConstructionCount() { return construction_count_; }
    static int GetDestructionCount() { return destruction_count_; }
    static void ResetCounters() { construction_count_ = 0; destruction_count_ = 0; }
};

int TestObject::construction_count_ = 0;
int TestObject::destruction_count_ = 0;

//==================================================================================================
// 示例 1: 原始内存分配与释放
//==================================================================================================
void Example1_RawMemoryAllocation()
{
    std::cout << "\n========== 示例 1: 原始内存分配与释放 ==========\n";
    std::cout << "allocate_raw_memory() 分配未初始化的内存，不调用构造函数\n";
    std::cout << "deallocate_raw_memory() 释放内存，不调用析构函数\n\n";

    TestObject::ResetCounters();

    // 分配能容纳 3 个 TestObject 的原始内存
    std::cout << "1. 分配原始内存（容纳 3 个对象）...\n";
    TestObject* raw_memory = hgl::allocate_raw_memory<TestObject>(3);
    std::cout << "   内存地址: " << raw_memory << "\n";
    std::cout << "   构造计数: " << TestObject::GetConstructionCount() << " (应为 0，因为未构造)\n";

    // 注意：此时 raw_memory 指向的内存是未初始化的，不能直接使用！
    // 必须先调用构造函数才能安全使用

    // 释放原始内存
    std::cout << "\n2. 释放原始内存...\n";
    hgl::deallocate_raw_memory(raw_memory);
    std::cout << "   析构计数: " << TestObject::GetDestructionCount() << " (应为 0，因为未调用析构)\n";

    std::cout << "\n⚠️  关键点：\n";
    std::cout << "   - allocate_raw_memory 仅分配内存，不构造对象\n";
    std::cout << "   - 必须配合 construct_at 系列函数使用\n";
    std::cout << "   - deallocate_raw_memory 前必须先手动析构所有对象\n";
}

//==================================================================================================
// 示例 2: 单个对象的显式构造与析构
//==================================================================================================
void Example2_ExplicitConstruction()
{
    std::cout << "\n========== 示例 2: 单个对象的显式构造与析构 ==========\n";
    std::cout << "construct_at() - 默认构造\n";
    std::cout << "construct_at_copy() - 复制构造\n";
    std::cout << "construct_at_move() - 移动构造\n";
    std::cout << "destroy_at() - 显式调用析构函数\n\n";

    TestObject::ResetCounters();

    // 分配原始内存（容纳 3 个对象）
    TestObject* memory = hgl::allocate_raw_memory<TestObject>(3);

    // 使用不同的构造方式
    std::cout << "1. 在 memory[0] 位置默认构造:\n";
    hgl::construct_at(&memory[0]);

    std::cout << "\n2. 在 memory[1] 位置复制构造:\n";
    TestObject source(42, "source_object");
    hgl::construct_at_copy(&memory[1], source);

    std::cout << "\n3. 在 memory[2] 位置移动构造:\n";
    TestObject movable(99, "movable_object");
    hgl::construct_at_move(&memory[2], std::move(movable));
    std::cout << "   移动后源对象 id: " << movable.GetId() << " (已标记为 -1)\n";

    // 现在可以安全使用这些对象
    std::cout << "\n4. 使用已构造的对象:\n";
    std::cout << "   memory[0]: id=" << memory[0].GetId() << ", name=\"" << memory[0].GetName() << "\"\n";
    std::cout << "   memory[1]: id=" << memory[1].GetId() << ", name=\"" << memory[1].GetName() << "\"\n";
    std::cout << "   memory[2]: id=" << memory[2].GetId() << ", name=\"" << memory[2].GetName() << "\"\n";

    // 显式析构每个对象
    std::cout << "\n5. 显式析构所有对象:\n";
    hgl::destroy_at(&memory[0]);
    hgl::destroy_at(&memory[1]);
    hgl::destroy_at(&memory[2]);

    // 释放内存
    std::cout << "\n6. 释放内存:\n";
    hgl::deallocate_raw_memory(memory);

    std::cout << "\n统计: 构造=" << TestObject::GetConstructionCount()
              << ", 析构=" << TestObject::GetDestructionCount() << "\n";
}

//==================================================================================================
// 示例 3: 批量构造与析构
//==================================================================================================
void Example3_BatchConstruction()
{
    std::cout << "\n========== 示例 3: 批量构造与析构 ==========\n";
    std::cout << "default_construct_range() - 批量默认构造\n";
    std::cout << "copy_construct_range() - 批量复制构造\n";
    std::cout << "move_construct_range() - 批量移动构造\n";
    std::cout << "destroy_range() - 批量析构\n\n";

    TestObject::ResetCounters();

    // 测试默认构造范围
    std::cout << "1. 批量默认构造 5 个对象:\n";
    TestObject* array1 = hgl::allocate_raw_memory<TestObject>(5);
    hgl::default_construct_range(array1, 5);
    std::cout << "   构造计数: " << TestObject::GetConstructionCount() << "\n";

    // 测试复制构造范围
    std::cout << "\n2. 批量复制构造 5 个对象:\n";
    TestObject* array2 = hgl::allocate_raw_memory<TestObject>(5);
    hgl::copy_construct_range(array2, array1, 5);
    std::cout << "   构造计数: " << TestObject::GetConstructionCount() << "\n";

    // 测试移动构造范围
    std::cout << "\n3. 批量移动构造 5 个对象:\n";
    TestObject* array3 = hgl::allocate_raw_memory<TestObject>(5);
    hgl::move_construct_range(array3, array1, 5);
    std::cout << "   构造计数: " << TestObject::GetConstructionCount() << "\n";

    // 测试范围析构（使用指针范围）
    std::cout << "\n4. 使用 destroy_range(first, last) 析构 array1:\n";
    hgl::destroy_range(array1, array1 + 5);

    // 测试范围析构（使用计数）
    std::cout << "\n5. 使用 destroy_range(data, count) 析构 array2:\n";
    hgl::destroy_range(array2, 5);

    std::cout << "\n6. 析构 array3 并释放所有内存:\n";
    hgl::destroy_range(array3, 5);
    hgl::deallocate_raw_memory(array1);
    hgl::deallocate_raw_memory(array2);
    hgl::deallocate_raw_memory(array3);

    std::cout << "\n统计: 构造=" << TestObject::GetConstructionCount()
              << ", 析构=" << TestObject::GetDestructionCount() << "\n";
}

//==================================================================================================
// 示例 4: 内存重新分配与移动
//==================================================================================================
void Example4_Reallocation()
{
    std::cout << "\n========== 示例 4: 内存重新分配与移动 ==========\n";
    std::cout << "reallocate_and_move() - 重新分配内存并移动现有对象\n";
    std::cout << "这是容器扩容时的典型操作，会：\n";
    std::cout << "  1. 分配新的更大内存\n";
    std::cout << "  2. 移动构造所有现有对象到新内存\n";
    std::cout << "  3. 析构旧内存中的对象\n";
    std::cout << "  4. 释放旧内存\n\n";

    TestObject::ResetCounters();

    // 创建初始数组（直接在原始内存中构造）
    std::cout << "1. 创建初始数组（容量 3）:\n";
    TestObject* old_array = hgl::allocate_raw_memory<TestObject>(3);
    hgl::construct_at_copy(&old_array[0], TestObject(1, "first"));
    hgl::construct_at_copy(&old_array[1], TestObject(2, "second"));
    hgl::construct_at_copy(&old_array[2], TestObject(3, "third"));

    std::cout << "\n2. 扩容到容量 6 (reallocate_and_move):\n";
    TestObject* new_array = hgl::reallocate_and_move(old_array, 3, 6);

    std::cout << "\n3. 验证对象已正确移动:\n";
    for(int i = 0; i < 3; ++i)
    {
        std::cout << "   new_array[" << i << "]: id=" << new_array[i].GetId()
                  << ", name=\"" << new_array[i].GetName() << "\"\n";
    }

    std::cout << "\n4. 在新的空间构造更多对象:\n";
    hgl::construct_at_copy(&new_array[3], TestObject(4, "fourth"));
    hgl::construct_at_copy(&new_array[4], TestObject(5, "fifth"));
    hgl::construct_at_copy(&new_array[5], TestObject(6, "sixth"));

    std::cout << "\n5. 清理:\n";
    hgl::destroy_range(new_array, 6);
    hgl::deallocate_raw_memory(new_array);

    std::cout << "\n统计: 构造=" << TestObject::GetConstructionCount()
              << ", 析构=" << TestObject::GetDestructionCount() << "\n";

    std::cout << "\n⚠️  关键点：\n";
    std::cout << "   - reallocate_and_move 会自动处理移动和清理\n";
    std::cout << "   - 旧指针 (old_array) 在调用后失效，不能再使用\n";
    std::cout << "   - 这是 PtrArray 扩容的核心机制\n";
}

//==================================================================================================
// 示例 5: 常见错误与陷阱
//==================================================================================================
void Example5_CommonPitfalls()
{
    std::cout << "\n========== 示例 5: 常见错误与陷阱 ==========\n";
    std::cout << "展示使用 ObjectUtil 时的常见错误和正确做法\n\n";

    TestObject::ResetCounters();

    // 错误 1: 忘记构造就使用
    std::cout << "❌ 错误 1: 分配内存后忘记构造就直接使用\n";
    std::cout << "   TestObject* p = allocate_raw_memory<TestObject>(1);\n";
    std::cout << "   p->GetId(); // 💥 未定义行为！访问未初始化的内存\n";
    std::cout << "\n✅ 正确做法:\n";
    std::cout << "   TestObject* p = allocate_raw_memory<TestObject>(1);\n";
    std::cout << "   construct_at(p); // 必须先构造\n";
    std::cout << "   p->GetId(); // 现在可以安全使用\n";

    // 错误 2: 忘记析构就释放
    std::cout << "\n❌ 错误 2: 释放内存前忘记析构对象\n";
    std::cout << "   TestObject* p = allocate_raw_memory<TestObject>(1);\n";
    std::cout << "   construct_at_copy(p, TestObject(1, \"leak\"));\n";
    std::cout << "   deallocate_raw_memory(p); // 💥 内存泄漏！std::string 未释放\n";
    std::cout << "\n✅ 正确做法:\n";
    std::cout << "   TestObject* p = allocate_raw_memory<TestObject>(1);\n";
    std::cout << "   construct_at_copy(p, TestObject(1, \"ok\"));\n";
    std::cout << "   destroy_at(p); // 必须先析构\n";
    std::cout << "   deallocate_raw_memory(p); // 再释放内存\n";

    // 错误 3: 重复析构
    std::cout << "\n❌ 错误 3: 对同一对象重复析构\n";
    std::cout << "   destroy_at(p);\n";
    std::cout << "   destroy_at(p); // 💥 双重释放！\n";
    std::cout << "\n✅ 正确做法: 每个对象只析构一次\n";

    // 错误 4: 移动后忘记析构源对象
    std::cout << "\n❌ 错误 4: 移动构造后忘记析构源对象\n";
    TestObject* src = hgl::allocate_raw_memory<TestObject>(1);
    TestObject* dst = hgl::allocate_raw_memory<TestObject>(1);
    hgl::construct_at_copy(src, TestObject(1, "source"));
    std::cout << "   construct_at_move(dst, std::move(*src));\n";
    std::cout << "   deallocate_raw_memory(src); // 💥 src 虽然被移动，但未析构！\n";
    std::cout << "\n✅ 正确做法: 移动后仍需析构源对象\n";
    hgl::construct_at_move(dst, std::move(*src));
    hgl::destroy_at(src); // 必须析构
    hgl::destroy_at(dst);
    hgl::deallocate_raw_memory(src);
    hgl::deallocate_raw_memory(dst);

    // 错误 5: 计数错误导致越界
    std::cout << "\n❌ 错误 5: 析构数量超过构造数量\n";
    std::cout << "   TestObject* p = allocate_raw_memory<TestObject>(5);\n";
    std::cout << "   default_construct_range(p, 3); // 只构造 3 个\n";
    std::cout << "   destroy_range(p, 5); // 💥 析构 5 个！后两个未构造\n";
    std::cout << "\n✅ 正确做法: 析构数量必须匹配构造数量\n";

    std::cout << "\n总结: 使用 ObjectUtil 的黄金法则:\n";
    std::cout << "  1. 分配 → 构造 → 使用 → 析构 → 释放 (严格顺序)\n";
    std::cout << "  2. 每个对象恰好构造一次、析构一次\n";
    std::cout << "  3. 移动后的对象仍需析构\n";
    std::cout << "  4. 记录活动对象的数量，避免越界操作\n";
}

//==================================================================================================
// 主函数
//==================================================================================================
int main()
{
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║         ObjectUtil.h 完整使用范例                         ║\n";
    std::cout << "║    非平凡类型的底层内存管理工具演示                       ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";

    try
    {
        Example1_RawMemoryAllocation();
        Example2_ExplicitConstruction();
        Example3_BatchConstruction();
        Example4_Reallocation();
        Example5_CommonPitfalls();

        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                  所有示例执行完毕                          ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "\n异常: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
