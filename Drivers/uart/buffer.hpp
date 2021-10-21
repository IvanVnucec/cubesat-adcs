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
#include "stm32l4xx.h"

template <class T = std::uint8_t> class Buffer {
  private:
    std::unique_ptr<T[]> m_buffer;
    unsigned m_head;
    unsigned m_tail;
    unsigned m_size;

  public:
    Buffer<T>(int size)
    {
        __disable_irq();
        m_buffer = { std::unique_ptr<T[]>(new T[size]) };
        m_head = 0;
        m_tail = 0;
        m_size = 0;
        __enable_irq();
    }

    ~Buffer<T>()
    {
    }

    void push(T item)
    {
        __disable_irq();
        m_buffer[m_tail] = item;
        m_tail           = (m_tail + 1) % m_size;
        __enable_irq();
    }

    T pop()
    {
        __disable_irq();
        T item = m_buffer[m_head];
        m_head = (m_head + 1) % m_size;
        __enable_irq();
        return item;
    }

    bool is_empty()
    {
        __disable_irq();
        bool retval = m_head == m_tail;
        __enable_irq();

        return retval;
    }

    bool is_full()
    {
        __disable_irq();
        bool retval = m_tail == (m_head - 1) % m_size;
        __enable_irq();

        return retval;
    }
};

#endif /* PARSER_BUFFER_HPP_ */
