// Реализовано две версии циклического буфера (динамический и статический)
// Преимущества статического заключаются в его скорости,
// так как работа с данными происходит с статической памятью,
// но из-за этого данная реализация циклического буфера не позволяет изменять его размер, 
// то есть размер должен быть задан изначально и в дальнейшем не может быть изменен.
// В динамической реализации же при добавлении элемента в уже заполненный полностью циклический буфер,
// произойдет перемещение старых данных, а размер циклического буфера для хранения данных увеличится в два раза.
// Стоит отметить, что в обоих случаях для реализации циклического буфера 
// зарезервировано на один элемент больше, для того чтобы удобнее было определять,
// когда циклический буфер заполнен, а когда пустой.
#pragma once

#include <cstddef>

template<typename T, size_t SZ>
class RingQueueStatic{
private:
    T st = 0;
    T en = 0;
    T data[SZ + 1];
public:
    RingQueueStatic(){};
    ~RingQueueStatic(){
        clear();
    };

    void push_back(const T &value){
        if(size() == SZ){
            std::cout << "Couldn't perform push_back" << std::endl;
            return;
        }
        data[en++] = value;
        en %= SZ + 1;
    };

    void pop_front(){
        if(isEmpty()){
            std::cout << "RingQueueStatic is empty" << std::endl;
            return;
        }
        st++;
        st %= SZ + 1;
    }

    bool isEmpty() const{
        return st == en;
    }

    void clear(){
        st = en = 0;
    }

    size_t reserved() const{
        return SZ;
    }

    size_t size() const{
        return en >= st ? en - st : en + SZ + 1 - st;
    }

    class Iterator{
        private:
            T* cur;
            T* st;
            T* en;
        public:
            Iterator(T* it, T* s, T* e) : cur(it), st(s), en(e){}

            T& operator++(){
                if(cur == en){
                    cur = st;
                }
                else{
                    cur++;
                }
                return *cur;
            }

            T& operator--(){
                if(cur == st){
                    cur = en;
                }
                else{
                    cur--;
                }
                return *cur;
            }
            
            bool operator!=(const Iterator& it){
                return cur != it.cur;
            }
            bool operator==(const Iterator& it){
                return cur == it.cur;
            }
            T& operator*(){
                return *cur;
            }
        
    };

    Iterator begin(){
            return Iterator(data + st, data, data + SZ);
    }

    Iterator end(){
        return Iterator(data + en, data, data + SZ);
    }
};

template<typename T>
class RingQueueDynamic{
private:
    T st = 0;
    T en = 0;
    T* data;
    size_t sz;
public:
    RingQueueDynamic(size_t val){
        sz = val;
        data = new T[sz + 1];
    };

    ~RingQueueDynamic(){
        clear();
    };

    RingQueueDynamic(const RingQueueDynamic& other){
        data = new T[other.sz + 1];
        st = other.st;
        en = other.en;
        sz = other.sz;
        for(size_t i = st; i != en; i++){
            if(i == sz + 1){
                i = 0;
            }
            data[i] = other.data[i];
        }
    }

    RingQueueDynamic& operator=(const RingQueueDynamic& other){
        data = new T[other.sz + 1];
        st = other.st;
        en = other.en;
        sz = other.sz;
        for(size_t i = st; i != en; i++){
            if(i == sz + 1){
                i = 0;
            }
            data[i] = other.data[i];
        }
        return *this;
    }

    RingQueueDynamic(const RingQueueDynamic&& other) noexcept{
        st = other.st;
        en = other.en;
        sz = other.sz;
        data = other.data;
        other.data = nullptr;
    }

    RingQueueDynamic& operator=(const RingQueueDynamic&& other) noexcept{
        st = other.st;
        en = other.en;
        sz = other.sz;
        data = other.data;
        other.data = nullptr;
        return *this;
    }

    void push_back(const T &value){
        if(size() == sz){
            T* newData = new T[sz * 2];
            size_t j = 0;
            for(size_t i = st; i != en; i++, j++){
                if(i == sz + 1){
                    i = 0;
                }
                newData[j] = data[i];
            }
            st = 0;
            en = j;
            delete[] data;
            data = newData;
            sz *= 2;
        }
        data[en++] = value;
        en %= sz + 1;
    };

    void pop_front(){
        if(isEmpty()){
            std::cout << "RingQueueStatic is empty" << std::endl;
            return;
        }
        st++;
        st %= sz + 1;
    }

    bool isEmpty() const{
        return st == en;
    }

    void clear(){
        delete[] data;
        data = nullptr;
        st = en = 0;
    }

    size_t reserved() const{
        return sz;
    }

    size_t size() const{
        return en >= st ? en - st : en + sz + 1 - st;
    }

    class Iterator{
        private:
            T* cur;
            T* st;
            T* en;
        public:
            Iterator(T* it, T* s, T* e) : cur(it), st(s), en(e){}

            T& operator++(){
                if(cur == en){
                    cur = st;
                }
                else{
                    cur++;
                }
                return *cur;
            }

            T& operator--(){
                if(cur == st){
                    cur = en;
                }
                else{
                    cur--;
                }
                return *cur;
            }
            
            bool operator!=(const Iterator& it){
                return cur != it.cur;
            }
            bool operator==(const Iterator& it){
                return cur == it.cur;
            }
            T& operator*(){
                return *cur;
            }
        
    };

    Iterator begin(){
            return Iterator(data + st, data, data + sz);
    }

    Iterator end(){
        return Iterator(data + en, data, data + sz);
    }
};
