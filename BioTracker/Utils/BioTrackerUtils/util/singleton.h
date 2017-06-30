#pragma once
#include <map>
#include "settings/Settings.h"

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

/* 
 * Just like Singleton, but I want to have several different singleton instances of the same type which I can address by name.
 * Example is the Settings class, which is also threadsafe by plastering it with mutexes.
 * This way you can access everywhere in the application the current configuration from different threads without worries. 
 * Only recommendet, as performance is not relevant when accessing configuration.
*/
template<class T>
class TypedSingleton {

protected:
	TypedSingleton() {}
	~TypedSingleton() {}
	TypedSingleton(const TypedSingleton &) = delete;
	TypedSingleton &operator=(const TypedSingleton &) = delete;

public:
	static T *getInstance(std::string s);

};

template<class T>
T *TypedSingleton<T>::getInstance(std::string s) {
	static std::map<std::string, T*> instance;
	if (instance.find(s) != instance.end()) {
		return instance.find(s)->second;
	}
	else {
		T* t = new T(s);
		instance.insert(std::pair<std::string, T*>(s, t));
		return t;
	}
	return 0;
}

}
}
