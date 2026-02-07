#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

namespace hgl
{
    class ByteWriter
    {
        std::vector<uint8_t> &out;

    public:
        explicit ByteWriter(std::vector<uint8_t> &buffer)
            : out(buffer)
        {
        }

        void reset()
        {
            out.clear();
        }

        void u8(uint8_t value)
        {
            out.push_back(value);
        }

        void u32(uint32_t value)
        {
            out.push_back(static_cast<uint8_t>(value & 0xFF));
            out.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
            out.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
            out.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
        }

        void i32(int32_t value)
        {
            u32(static_cast<uint32_t>(value));
        }

        void bytes(const uint8_t *data, size_t size)
        {
            if(size == 0 || !data)
                return;

            out.insert(out.end(), data, data + size);
        }

        int string(const std::string &value)
        {
            if(value.size() > 0xFF)
                return -1;

            const uint8_t length = static_cast<uint8_t>(value.size());
            u8(length);
            bytes(reinterpret_cast<const uint8_t *>(value.data()), value.size());
            return static_cast<int>(length);
        }
    };

    class ByteReader
    {
        const std::vector<uint8_t> &in;
        size_t offset;

    public:
        explicit ByteReader(const std::vector<uint8_t> &buffer)
            : in(buffer), offset(0)
        {
        }

        size_t left() const
        {
            return (offset <= in.size()) ? (in.size() - offset) : 0;
        }

        bool u8(uint8_t &value)
        {
            if(offset + 1 > in.size())
                return false;

            value = in[offset];
            offset += 1;
            return true;
        }

        bool u32(uint32_t &value)
        {
            if(offset + 4 > in.size())
                return false;

            value = static_cast<uint32_t>(in[offset])
                  | (static_cast<uint32_t>(in[offset + 1]) << 8)
                  | (static_cast<uint32_t>(in[offset + 2]) << 16)
                  | (static_cast<uint32_t>(in[offset + 3]) << 24);
            offset += 4;
            return true;
        }

        bool i32(int32_t &value)
        {
            uint32_t temp = 0;
            if(!u32(temp))
                return false;

            value = static_cast<int32_t>(temp);
            return true;
        }

        int string(std::string &value)
        {
            uint8_t length = 0;
            if(!u8(length))
                return -1;

            if(length == 0)
            {
                value.clear();
                return 0;
            }

            if(offset + length > in.size())
                return -1;

            value.assign(reinterpret_cast<const char *>(&in[offset]), length);
            offset += length;
            return static_cast<int>(length);
        }
    };
}//namespace hgl
