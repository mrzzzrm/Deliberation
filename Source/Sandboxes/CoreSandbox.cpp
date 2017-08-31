#include <iostream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/Sphere.h>
#include <Deliberation/Deliberation.h>

struct Foo {
//    template<typename ValueType>
//    operator ValueType() const {
//        return 0;
//    }

    friend std::ostream &operator<<(std::ostream &o, const Foo &f) {
        o << "Foo";
        return o;
    }
};


int main(int argc, char *argv[]) {
    deliberation::init();

    Foo foo;
    Sphere sphere({1.0f, 0.0f, 5.0f}, 3.3f);

    AssertM(true, "Hello World");
    AssertF(false, "The world is {}", sphere);

    return 0;
}
