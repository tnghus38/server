
//�ڵ鷯 ������ ����ȭ �մϴ�.
//�ڵ鷯�� ���ÿ� ������� �ʽ��ϴ�.
//ó����� ������ ���μ��������� ����˴ϴ�.
//�ڵ鷯 ��������� ������� �ʽ��ϴ�.

//������ ���μ����� ���� �������̽��� �����ؾ� �մϴ�.
//template <typename F> void Processor::push(F w);
//���μ����� �� �۾� �׸��� �߰��մϴ�. F�� ȣ�� �Ҽ� �ִ� �����ͺ��Դϴ�.

// std :: function <void ()>
// bool Processor :: canDispatch ();
// ���� �������� ���μ����� �� ��Ī �Լ��� ������ true�� ��ȯ�ؾ��մϴ�.

#pragma once
#include "Callstack.h"
#include "Monitor.h"
#include <assert.h>
#include <queue>
#include <functional>

template <typename Processor>
class Strand {
  public:
    Strand(Processor& proc) : m_proc(proc) {}

    Strand(const Strand&) = delete;
    Strand& operator=(const Strand&) = delete;


    //��� strand ������ �����Ǹ� ��� ó���⸦ �����ϰų� ��ȣ�� ���ο��� ������ �������� ������ ���߿� �����ϱ� ���� ó���⸦ �Խ��մϴ�.
    template <typename F>
    void dispatch(F handler) {

        //���� ���μ��� ����ġ �Լ� (�� �����忡��)�� ���ٸ� ���⿡�� �ڵ鷯�� ������ �� �����Ƿ� ť�� �׾ƵӴϴ�.
        if (!m_proc.canDispatch()) {
            post(std::move(handler));
            return;
        }

        // ���� : �� �������� �츮�� �۾��� �����忡 ������ �� �� �ֽ��ϴ� (���� �˻� ������)

        //�� thread�� strand �� �����ϰ��ִ� ���,
        //���� ��, �ٸ� thread�� strand �� ������ �� ���� ������
        //�ٸ� üũ�� �ǽ����� �ʰ� ��� �ڵ鷯�� ������ �� �ֽ��ϴ�.
        if (runningInThisThread()) {
            handler();
            return;
        }

        auto trigger = m_data([&](Data& data) {
            if (data.running) {
                data.q.push(std::move(handler));
                return false;
            } else {
                data.running = true;
                return true;
            }
        });

        if (trigger) {
            Callstack<Strand>::Context ctx(this);
            handler();
            run();
        }
    }

    template <typename F>
    void post(F handler) {
        bool trigger = m_data([&](Data& data) {
            data.q.push(std::move(handler));
            if (data.running) {
                return false;
            } else {
                data.running = true;
                return true;
            }
        });

        if (trigger) {
            m_proc.push([this] { run(); });
        }
    }

    bool runningInThisThread() {
        return Callstack<Strand>::contains(this) != nullptr;
    }

  private:
    void run() {
        Callstack<Strand>::Context ctx(this);
        while (true) {
            std::function<void()> handler;

            m_data([&](Data& data) {
                assert(data.running);
                if (data.q.size()) {
                    handler = std::move(data.q.front());
                    data.q.pop();
                } else {
                    data.running = false;
                }
            });

            if (handler)
                handler();
            else
                return;
        }
    }

    struct Data {
        bool running = false;
        std::queue<std::function<void()>> q;
    };
    Monitor<Data> m_data;
    Processor& m_proc;
};