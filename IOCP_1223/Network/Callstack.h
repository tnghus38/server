#pragma once

//쓰레드 상에서 실행되는 함수의 콜스택을 남길수 있다.??
template <typename Key, typename Value = unsigned char>
class Callstack {
  public:
    class Iterator;

    class Context {
      public:
        Context(const Context&) = delete;
        Context& operator=(const Context&) = delete;
        explicit Context(Key* k)
            : m_key(k), m_next(Callstack<Key, Value>::ms_top) {
            m_val = reinterpret_cast<unsigned char*>(this);
            Callstack<Key, Value>::ms_top = this;
        }

        Context(Key* k, Value& v)
            : m_key(k), m_val(&v), m_next(Callstack<Key, Value>::ms_top) {
            Callstack<Key, Value>::ms_top = this;
        }

        ~Context() {
            Callstack<Key, Value>::ms_top = m_next;
        }

        Key* getKey() {
            return m_key;
        }

        Value* getValue() {
            return m_val;
        }

      private:
        friend class Callstack<Key, Value>;
        friend class Callstack<Key, Value>::Iterator;
        Key* m_key;
        Value* m_val;
        Context* m_next;
    };

    class Iterator {
      public:
        Iterator(Context* ctx) : m_ctx(ctx) {}
        Iterator& operator++() {
            if (m_ctx)
                m_ctx = m_ctx->m_next;
            return *this;
        }

        bool operator!=(const Iterator& other) {
            return m_ctx != other.m_ctx;
        }

        Context* operator*() {
            return m_ctx;
        }

      private:
        Context* m_ctx;
    };

    static Value* contains(const Key* k) {
        Context* elem = ms_top;
        while (elem) {
            if (elem->m_key == k)
                return elem->m_val;
            elem = elem->m_next;
        }
        return nullptr;
    }

    static Iterator begin() {
        return Iterator(ms_top);
    }

    static Iterator end() {
        return Iterator(nullptr);
    }

  private:
    static thread_local Context* ms_top;
};

template <typename Key, typename Value>
typename thread_local Callstack<Key, Value>::Context*
Callstack<Key, Value>::ms_top = nullptr;

