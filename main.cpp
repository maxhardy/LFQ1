#include <iostream>
#include <vector>
#include <thread>
#include "lockfreequeue.cpp"
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

const int NUM = 100000000;

void test_lock_free_queue(vector<Node<int>*> data_list){
    cout <<  "1 个生产者与 1 个消费者，测试数据："
         << NUM << endl;
    LockFreeQueue<int> q;
    thread t1([&data_list, &q](){
        for (int i = 0; i < data_list.size(); i++){
            q.enqueue(data_list[i]);
        }
    });

    thread t2([&q](){
        for (int x = 0; x < NUM; x++){
            q.dequeue();
        }
    });
    t1.join();
    t2.join();
}

void test_lock_free_queue2(vector<Node<int>*> &data_list){
    cout <<  "2 个生产者与 1 个消费者，测试数据："
         << NUM << endl;
    LockFreeQueue<int> q;
    thread t1([&data_list, &q](){
        for (int i = 0; i < data_list.size(); i+=2){
            q.enqueue(data_list[i]);
        }
    });
    thread t3([&data_list, &q](){
        for (int i = 1; i < data_list.size(); i+=2){
            q.enqueue(data_list[i]);
        }
    });

    thread t2([&q](){
        for (int x = 0; x < NUM; x++){
            q.dequeue();
        }
    });
    t1.join();
    t2.join();
    t3.join();
}

void test_lock_free_queue3(vector<Node<int>*> &data_list){
    cout <<  "1 个生产者与 2 个消费者，测试数据："
         << NUM << endl;
    LockFreeQueue<int> q;
    thread t1([&data_list, &q](){
        for (int i = 0; i < data_list.size(); i++){
            q.enqueue(data_list[i]);
        }
    });
    thread t3([&q](){
        for (int x = 0; x < NUM/2; x++){
            q.dequeue();
        }
    });

    thread t2([&q](){
        for (int x = 0; x < NUM/2; x++){
            q.dequeue();
        }
    });
    t1.join();
    t2.join();
    t3.join();
}

void test_lock_free_queue4(vector<Node<int>*> data_list){
    cout <<  "4 个生产者与 4 个消费者，测试数据："
         << NUM << endl;
    LockFreeQueue<int> q;
    thread t1([&data_list, &q](){
        for (int i = 0; i < data_list.size(); i+=4){
            q.enqueue(data_list[i]);
        }
    });
    thread t2([&data_list, &q](){
        for (int i = 1; i < data_list.size(); i+=4){
            q.enqueue(data_list[i]);
        }
    });
    thread t3([&data_list, &q](){
        for (int i = 2; i < data_list.size(); i+=4){
            q.enqueue(data_list[i]);
        }
    });
    thread t4([&data_list, &q](){
        for (int i = 3; i < data_list.size(); i+=4){
            q.enqueue(data_list[i]);
        }
    });
    thread t5([&q](){
        for (int x = 0; x < NUM/4; x++){
            q.dequeue();
        }
    });

    thread t6([&q](){
        for (int x = 0; x < NUM/4; x++){
            q.dequeue();
        }
    });

    thread t7([&q](){
        for (int x = 0; x < NUM/4; x++){
            q.dequeue();
        }
    });

    thread t8([&q](){
        for (int x = 0; x < NUM/4; x++){
            q.dequeue();
        }
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();
}

vector<Node<int>*> gen_data(){
    vector<Node<int>*> data_list;
    default_random_engine e;
    uniform_int_distribution<unsigned> u(0, NUM);
    for(int i = 0; i < NUM; i++){
        Node<int> *node = new Node<int>(u(e));
        data_list.push_back(node);
    }
    return data_list;
}


int main() {
    cout << "生成随机数据...init" << endl;
    vector<Node<int>*> data_list = gen_data();
    cout << "生成随机数据...done" << endl;

    auto start = system_clock::now();
    test_lock_free_queue(data_list);
    auto end   = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << double(duration.count()) * microseconds::period::num / microseconds::period::den
         << "s" << endl <<endl;

    auto start2 = system_clock::now();
    test_lock_free_queue2(data_list);
    auto end2   = system_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2);
    cout << double(duration2.count()) * microseconds::period::num / microseconds::period::den
         << "s" << endl <<endl;

    auto start3 = system_clock::now();
    test_lock_free_queue3(data_list);
    auto end3   = system_clock::now();
    auto duration3 = duration_cast<microseconds>(end3 - start3);
    cout << double(duration3.count()) * microseconds::period::num / microseconds::period::den
         << "s" << endl <<endl;

    auto start4 = system_clock::now();
    test_lock_free_queue4(data_list);
    auto end4   = system_clock::now();
    auto duration4 = duration_cast<microseconds>(end4 - start4);
    cout << double(duration4.count()) * microseconds::period::num / microseconds::period::den
         << "s" << endl <<endl;
}