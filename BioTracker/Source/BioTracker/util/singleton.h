#pragma once

namespace BioTracker {
namespace Util {

template<class T>
class Singleton {

  protected:
    Singleton() {}
    ~Singleton() {}
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

  public:
    static T &getInstance();

};

template<class T>
T &Singleton<T>::getInstance() {
    static T instance;
    return instance;
}

}
}
