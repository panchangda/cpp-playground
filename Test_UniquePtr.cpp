#include <cstdio>
#include <vector>
#include "UniquePtr.hpp"

struct MyClass {
    int a, b, c;
};

struct Animal {
    virtual void speak() = 0;
    virtual ~Animal() = default;
};

struct Dog : Animal {
    int age;

    Dog(int age_) : age(age_) {
    }

    virtual void speak() {
        printf("Bark! I'm %d Year Old!\n", age);
    }
};

struct Cat : Animal {
    int &age;

    Cat(int &age_) : age(age_) {
    }

    virtual void speak() {
        printf("Meow! I'm %d Year Old!\n", age);
    }
};

int main() {
    
    std::vector<UniquePtr<Animal>> zoo;
    int age = 3;
    zoo.push_back(MakeUnique<Cat>(age));
    zoo.push_back(MakeUnique<Dog>(age));
    for (auto const &a: zoo) {
        a->speak();
    }
    age++;
    for (auto const &a: zoo) {
        a->speak();
    }
    



    // 1. Why explicit?
    // int i;
    // UniquePtr<int> a = &i;     

    // 2. Why = delete?
    // auto a = UniquePtr<int>(new int(4));
    // UniquePtr<int>b;
    // b = a;
    // auto c = UniquePtr<int>(a);
    
    // 4. Why != ?
    // auto a = UniquePtr<int>(new int(4));
    // std::cout << a.get() << '\n';
    // a = std::move(a);
    // std::cout << a.get() << '\n';

    // 5. Why destruct in move assignment
    // auto a = UniquePtr<int>(new int(1));
    // a = UniquePtr<int>(new int(2));

    return 0;
}