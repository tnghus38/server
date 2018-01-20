#include "WorkQueue.h"

void test() {
    WorkQueue work;

    // �Һ��� ������ ����
    std::vector<std::thread> ths;
    for (int i = 0; i < 4; i++) {
        ths.push_back(std::thread([&work] { work.run(); }));
    }

    std::vector<int> res;
    res.resize(10000);

    // �۾���⿭�� �߰�
    for (int i = 0; i < static_cast<int>(res.size()); i++) {
        work.push([i, &res] { res[i]++; });
    }

    // �Һ��ڸ� ���߰� �����尡 �����⸦ ��ٸ���.
    work.stop();
    for (auto&& t : ths) t.join();

    // ��� �۾� �׸��� ����Ǿ����� �׽�Ʈ�մϴ�.
    for (int i = 0; i < static_cast<int>(res.size()); i++) {
        if (res[i] != 1)
            printf("ERROR: Index %d set to %d\n", i, res[i]);
    }
}