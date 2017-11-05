#ifndef NLOHMANN_JSON_DETAIL_PARSING_BINARY_WRITER_HPP
#define NLOHMANN_JSON_DETAIL_PARSING_BINARY_WRITER_HPP

#include <algorithm> // reverse
#include <array> // array
#include <cstdint> // uint8_t, uint16_t, uint32_t, uint64_t
#include <cstring> // memcpy
#include <limits> // numeric_limits

#include <json/detail/parsing/binary_reader.hpp>
#include <json/detail/parsing/output_adapters.hpp>

namespace nlohmann
{
namespace detail
{
/*!
@brief serialization to CBOR and MessagePack values
*/
template<typename BasicJsonType, typename CharType>
class binary_writer
{
  public:
    /*!
    @brief create a binary writer

    @param[in] adapter  output adapter to write to
    */
    explicit binary_writer(output_adapter_t<CharType> adapter) : oa(adapter)
    {
        assert(oa);
    }

    /*!
    @brief[in] j  JSON value to serialize
    */
    void write_cbor(const BasicJsonType& j)
    {
        switch (j.type())
        {
            case value_t::null:
            {
                oa->write_character(static_cast<CharType>(0xf6));
                break;
            }

            case value_t::boolean:
            {
                oa->write_character(j.m_value.boolean
                                    ? static_cast<CharType>(0xf5)
                                    : static_cast<CharType>(0xf4));
                break;
            }

            case value_t::number_integer:
            {
                if (j.m_value.number_integer >= 0)
                {
                    // CBOR does not differentiate between positive signed
                    // integers and unsigned integers. Therefore, we used the
                    // code from the value_t::number_unsigned case here.
                    if (j.m_value.number_integer <= 0x17)
                    {
                        write_number(static_cast<uint8_t>(j.m_value.number_integer));
                    }
                    else if (j.m_value.number_integer <= (std::numeric_limits<uint8_t>::max)())
                    {
                        oa->write_character(static_cast<CharType>(0x18));
                        write_number(static_cast<uint8_t>(j.m_value.number_integer));
                    }
                    else if (j.m_value.number_integer <= (std::numeric_limits<uint16_t>::max)())
                    {
                        oa->write_character(static_cast<CharType>(0x19));
                        write_number(static_cast<uint16_t>(j.m_value.number_integer));
                    }
                    else if (j.m_value.number_integer <= (std::numeric_limits<uint32_t>::max)())
                    {
                        oa->write_character(static_cast<CharType>(0x1a));
                        write_number(static_cast<uint32_t>(j.m_value.number_integer));
                    }
                    else
                    {
                        oa->write_character(static_cast<CharType>(0x1b));
                        write_number(static_cast<uint64_t>(j.m_value.number_integer));
                    }
                }
                else
                {
                    // The conversions below encode the sign in the first
                    // byte, and the value is converted to a positive number.
                    const auto positive_number = -1 - j.m_value.number_integer;
                    if (j.m_value.number_integer >= -24)
                    {
                        write_number(static_cast<uint8_t>(0x20 + positive_number));
                    }
                    else if (positive_number <= (std::numeric_limits<uint8_t>::max)())
                    {
                        oa->write_character(static_cast<CharType>(0x38));
                        write_number(static_cast<uint8_t>(positive_number));
                    }
                    else if (positive_number <= (std::numeric_limits<uint16_t>::max)())
                    {
                        oa->write_character(static_cast<CharType>(0x39));
                        write_number(static_cast<uint16_t>(positive_number));
                    }
                    else if (positive_number <= (std::numeric_limits<uint32_t>::max)())
                    {
                        oa->write_character(static_cast<CharType>(0x3a));
                        write_number(static_cast<uint32_t>(positive_number));
                    }
                    else
                    {
                        oa->write_character(static_cast<CharType>(0x3b));
                        write_number(static_cast<uint64_t>(positive_number));
                    }
                }
                break;
            }

            case value_t::number_unsigned:
            {
                if (j.m_value.number_unsigned <= 0x17)
                {
                    write_number(static_cast<uint8_t>(j.m_value.number_unsigned));
                }
                else if (j.m_value.number_unsigned <= (std::numeric_limits<uint8_t>::max)())
                {
                    oa->write_character(static_cast<CharType>(0x18));
                    write_number(static_cast<uint8_t>(j.m_value.number_unsigned));
                }
                else if (j.m_value.number_unsigned <= (std::numeric_limits<uint16_t>::max)())
                {
                    oa->write_character(static_cast<CharType>(0x19));
                    write_number(static_cast<uint16_t>(j.m_value.number_unsigned));
                }
                else if (j.m_value.number_unsigned <= (std::numeric_limits<uint32_t>::max)())
                {
                    oa->write_character(static_cast<CharType>(0x1a));
                    write_number(static_cast<uint32_t>(j.m_value.number_unsigned));
                }
                else
                {
                    oa->write_character(static_cast<CharType>(0x1b));
                    write_number(static_cast<uint64_t>(j.m_value.number_unsigned));
                }
                break;
            }

            case value_t::number_float: // Double-Precision Float
            {
                oa->write_character(static_cast<CharType>(0xfb));
                write_number(j.m_value.number_float);
                break;
            }

            case value_t::string:
            {
                // step 1: write control byte and the string length
                const auto N = j.m_value.string->size();
                if (N <= 0x17)
                {
                    write_number(static_cast<uint8_t>(0x60 + N));
                }
                else if (N <= 0xff)
                {
                    oa->write_character(static_cast<CharType>(0x78));
                    write_number(static_cast<uint8_t>(N));
                }
                else if (N <= 0xffff)
                {
                    oa->write_character(static_cast<CharType>(0x79));
                    write_number(static_cast<uint16_t>(N));
                }
                else if (N <= 0xffffffff)
                {
                    oa->write_character(static_cast<CharType>(0x7a));
                    write_number(static_cast<uint32_t>(N));
                }
                // LCOV_EXCL_START
                else if (N <= 0xffffffffffffffff)
                {
                    oa->write_character(static_cast<CharType>(0x7b));
                    write_number(static_cast<uint64_t>(N));
                }
                // LCOV_EXCL_STOP

                // step 2: write the string
                oa->write_characters(
                    reinterpret_cast<const CharType*>(j.m_value.string->c_str()),
                    j.m_value.string->size());
                break;
            }

            case value_t::array:
            {
                // step 1: write control byte and the array size
                const auto N = j.m_value.array->size();
                if (N <= 0x17)
                {
                    write_number(static_cast<uint8_t>(0x80 + N));
                }
                else if (N <= 0xff)
                {
                    oa->write_character(static_cast<CharType>(0x98));
                    write_number(static_cast<uint8_t>(N));
                }
                else if (N <= 0xffff)
                {
                    oa->write_character(static_cast<CharType>(0x99));
                    write_number(static_cast<uint16_t>(N));
                }
                else if (N <= 0xffffffff)
                {
                    oa->write_character(static_cast<CharType>(0x9a));
                    write_number(static_cast<uint32_t>(N));
                }
                // LCOV_EXCL_START
                else if (N <= 0xffffffffffffffff)
                {
                    oa->write_character(static_cast<CharType>(0x9b));
                    write_number(static_cast<uint64_t>(N));
                }
                // LCOV_EXCL_STOP

                // step 2: write each element
                for (const auto& el : *j.m_value.array)
                {
                    write_cbor(el);
                }
                break;
            }

            case value_t::object:
            {
                // step 1: write control byte and the object size
                const auto N = j.m_value.object->size();
                if (N <= 0x17)
                {
                    write_number(static_cast<uint8_t>(0xa0 + N));
                }
                else if (N <= 0xff)
                {
                    oa->write_character(static_cast<CharType>(0xb8));
                    write_number(static_cast<uint8_t>(N));
                }
                else if (N <= 0xffff)
                {
                    oa->write_character(static_cast<CharType>(0xb9));
                    write_number(static_cast<uint16_t>(N));
                }
                else if (N <= 0xffffffff)
                {
                    oa->write_character(static_cast<CharType>(0xba));
                    write_number(static_cast<uint32_t>(N));
                }
                // LCOV_EXCL_START
                else if (N <= 0xffffffffffffffff)
                {
                    oa->write_character(static_cast<CharType>(0xbb));
                    write_number(static_cast<uint64_t>(N));
                }
                // LCOV_EXCL_STOP

                // step 2: write each element
                for (const auto& el : *j.m_value.object)
                {
                    write_cbor(el.first);
                    write_cbor(el.second);
                }
                break;
            }

            default:
                break;
        }
    }

    /*!
    @brief[in] j  JSON value to serialize
    */
    void write_msgpack(const BasicJsonType& j)
    {
        switch (j.type())
        {
            case value_t::null: // nil
            {
                oa->write_character(static_cast<CharType>(0xc0));
                break;
            }

            case value_t::boolean: // true and false
            {
                oa->write_character(j.m_value.boolean
                                    ? static_cast<CharType>(0xc3)
                                    : static_cast<CharType>(0xc2));
                break;
            }

            case value_t::number_integer:
            {
                if (j.m_value.number_integer >= 0)
                {
                    // MessagePack does not differentiate between positive
                    // signed integers and unsigned integers. Therefore, we used
                    // the code from the value_t::number_unsigned case here.
                    if (j.m_value.number_unsigned < 128)
                    {
                        // positive fixnum
                        write_number(static_cast<uint8_t>(j.m_value.number_integer));
                    }
                    else if (j.m_value.number_unsigned <= (std::numeric_limits<uint8_t>::max)())
                    {
                        // uint 8
                        oa->write_character(static_cast<CharType>(0xcc));
                        write_number(static_cast<uint8_t>(j.m_value.number_integer));
                    }
                    else if (j.m_value.number_unsigned <= (std::numeric_limits<uint16_t>::max)())
                    {
                        // uint 16
                        oa->write_character(static_cast<CharType>(0xcd));
                        write_number(static_cast<uint16_t>(j.m_value.number_integer));
                    }
                    else if (j.m_value.number_unsigned <= (std::numeric_limits<uint32_t>::max)())
                    {
                        // uint 32
                        oa->write_character(static_cast<CharType>(0xce));
                        write_number(static_cast<uint32_t>(j.m_value.number_integer));
                    }
                    else if (j.m_value.number_unsigned <= (std::numeric_limits<uint64_t>::max)())
                    {
                        // uint 64
                        oa->write_character(static_cast<CharType>(0xcf));
                        write_number(static_cast<uint64_t>(j.m_value.number_integer));
                    }
                }
                else
                {
                    if (j.m_value.number_integer >= -32)
                    {
                        // negative fixnum
                        write_number(static_cast<int8_t>(j.m_value.number_integer));
                    }
                    else if (j.m_value.number_integer >= (std::numeric_limits<int8_t>::min)() and
                             j.m_value.number_integer <= (std::numeric_limits<int8_t>::max)())
                    {
                        // int 8
                        oa->write_character(static_cast<CharType>(0xd0));
                        write_number(static_cast<int8_t>(j.m_value.number_integer));
                    }
                    else if (j.m_value.number_integer >= (std::numeric_limits<int16_t>::min)() and
                             j.m_value.number_integer <= (std::numeric_limits<int16_t>::max)())
                    {
                        // int 16
                        oa->write_character(static_cast<CharType>(0xd1));
                        write_number(static_cast<int16_t>(j.m_value.number_integer));
                    }
                    else if (j.m_value.number_integer >= (std::numeric_limits<int32_t>::min)() and
                             j.m_value.number_integer <= (std::numeric_limits<int32_t>::max)())
                    {
                        // int 32
                        oa->write_character(static_cast<CharType>(0xd2));
                        write_number(static_cast<int32_t>(j.m_value.number_integer));
                    }
                    else if (j.m_value.number_integer >= (std::numeric_limits<int64_t>::min)() and
                             j.m_value.number_integer <= (std::numeric_limits<int64_t>::max)())
                    {
                        // int 64
                        oa->write_character(static_cast<CharType>(0xd3));
                        write_number(static_cast<int64_t>(j.m_value.number_integer));
                    }
                }
                break;
            }

            case value_t::number_unsigned:
            {
                if (j.m_value.number_unsigned < 128)
                {
                    // positive fixnum
                    write_number(static_cast<uint8_t>(j.m_value.number_integer));
                }
                else if (j.m_value.number_unsigned <= (std::numeric_limits<uint8_t>::max)())
                {
                    // uint 8
                    oa->write_character(static_cast<CharType>(0xcc));
                    write_number(static_cast<uint8_t>(j.m_value.number_integer));
                }
                else if (j.m_value.number_unsigned <= (std::numeric_limits<uint16_t>::max)())
                {
                    // uint 16
                    oa->write_character(static_cast<CharType>(0xcd));
                    write_number(static_cast<uint16_t>(j.m_value.number_integer));
                }
                else if (j.m_value.number_unsigned <= (std::numeric_limits<uint32_t>::max)())
                {
                    // uint 32
                    oa->write_character(static_cast<CharType>(0xce));
                    write_number(static_cast<uint32_t>(j.m_value.number_integer));
                }
                else if (j.m_value.number_unsigned <= (std::numeric_limits<uint64_t>::max)())
                {
                    // uint 64
                    oa->write_character(static_cast<CharType>(0xcf));
                    write_number(static_cast<uint64_t>(j.m_value.number_integer));
                }
                break;
            }

            case value_t::number_float: // float 64
            {
                oa->write_character(static_cast<CharType>(0xcb));
                write_number(j.m_value.number_float);
                break;
            }

            case value_t::string:
            {
                // step 1: write control byte and the string length
                const auto N = j.m_value.string->size();
                if (N <= 31)
                {
                    // fixstr
                    write_number(static_cast<uint8_t>(0xa0 | N));
                }
                else if (N <= 255)
                {
                    // str 8
                    oa->write_character(static_cast<CharType>(0xd9));
                    write_number(static_cast<uint8_t>(N));
                }
                else if (N <= 65535)
                {
                    // str 16
                    oa->write_character(static_cast<CharType>(0xda));
                    write_number(static_cast<uint16_t>(N));
                }
                else if (N <= 4294967295)
                {
                    // str 32
                    oa->write_character(static_cast<CharType>(0xdb));
                    write_number(static_cast<uint32_t>(N));
                }

                // step 2: write the string
                oa->write_characters(
                    reinterpret_cast<const CharType*>(j.m_value.string->c_str()),
                    j.m_value.string->size());
                break;
            }

            case value_t::array:
            {
                // step 1: write control byte and the array size
                const auto N = j.m_value.array->size();
                if (N <= 15)
                {
                    // fixarray
                    write_number(static_cast<uint8_t>(0x90 | N));
                }
                else if (N <= 0xffff)
                {
                    // array 16
                    oa->write_character(static_cast<CharType>(0xdc));
                    write_number(static_cast<uint16_t>(N));
                }
                else if (N <= 0xffffffff)
                {
                    // array 32
                    oa->write_character(static_cast<CharType>(0xdd));
                    write_number(static_cast<uint32_t>(N));
                }

                // step 2: write each element
                for (const auto& el : *j.m_value.array)
                {
                    write_msgpack(el);
                }
                break;
            }

            case value_t::object:
            {
                // step 1: write control byte and the object size
                const auto N = j.m_value.object->size();
                if (N <= 15)
                {
                    // fixmap
                    write_number(static_cast<uint8_t>(0x80 | (N & 0xf)));
                }
                else if (N <= 65535)
                {
                    // map 16
                    oa->write_character(static_cast<CharType>(0xde));
                    write_number(static_cast<uint16_t>(N));
                }
                else if (N <= 4294967295)
                {
                    // map 32
                    oa->write_character(static_cast<CharType>(0xdf));
                    write_number(static_cast<uint32_t>(N));
                }

                // step 2: write each element
                for (const auto& el : *j.m_value.object)
                {
                    write_msgpack(el.first);
                    write_msgpack(el.second);
                }
                break;
            }

            default:
                break;
        }
    }

  private:
    /*
    @brief write a number to output input

    @param[in] n number of type @a NumberType
    @tparam NumberType the type of the number

    @note This function needs to respect the system's endianess, because bytes
          in CBOR and MessagePack are stored in network order (big endian) and
          therefore need reordering on little endian systems.
    */
    template<typename NumberType> void write_number(NumberType n)
    {
        // step 1: write number to array of length NumberType
        std::array<CharType, sizeof(NumberType)> vec;
        std::memcpy(vec.data(), &n, sizeof(NumberType));

        // step 2: write array to output (with possible reordering)
        if (is_little_endian)
        {
            // reverse byte order prior to conversion if necessary
            std::reverse(vec.begin(), vec.end());
        }

        oa->write_characters(vec.data(), sizeof(NumberType));
    }

  private:
    /// whether we can assume little endianess
    const bool is_little_endian = binary_reader<BasicJsonType>::little_endianess();

    /// the output
    output_adapter_t<CharType> oa = nullptr;
};
}
}

#endif
