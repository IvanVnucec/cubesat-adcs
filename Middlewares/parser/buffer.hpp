/*
 * buffer.hpp
 *
 *  Created on: 20. lis 2021.
 *      Author: ivnucec
 */

#ifndef PARSER_BUFFER_HPP_
#define PARSER_BUFFER_HPP_

#include <cstdint>
#include <memory>

// TODO [ivan vnucec]: guard variables by disabling interrupts or use some other solution. See functions below
template <class T = std::uint8_t> class Buffer {
  private:
    std::unique_ptr<T[]> m_buffer;
    unsigned m_head;
    unsigned m_tail;
    unsigned m_size;

  public:
    Buffer<T>(int size)
        : m_buffer(std::unique_ptr<T[]>(new T[size])), m_head(0), m_tail(0), m_size(size)
    {
    }

    ~Buffer<T>()
    {
    }

    void push(T item)
    {
        m_buffer[m_tail] = item;
        m_tail           = (m_tail + 1) % m_size;
    }

    T pop()
    {
        T item = m_buffer[m_head];
        m_head = (m_head + 1) % m_size;
        return item;
    }

    bool is_empty()
    {
        return m_head == m_tail;
    }

    bool is_full()
    {
        return (m_tail == (m_head - 1) % m_size);
    }
};

#endif /* PARSER_BUFFER_HPP_ */
