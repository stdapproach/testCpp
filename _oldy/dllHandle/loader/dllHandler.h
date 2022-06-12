#ifndef DLLHANDLER_H
#define DLLHANDLER_H
#include <iostream>

#include <memory>
#include <dlfcn.h>
#include <cassert>

namespace tmp {

template<typename T>
struct funcDescr
{
    typedef T Type;
    funcDescr(Type f_, const char* name)
        :f(f_),_name(name){}
    operator bool()const{return f;}
    const char* name()const{return _name;}
    Type f;
private:
    const char* _name;
};

struct dlib
{
//based on https://stackoverflow.com/questions/36040814/stdshared-ptr-and-dlopen-avoiding-undefined-behavior
public:
  template<typename T>
  T foo2(const char* name)
  {
      if (!handle) return nullptr;
      void* sym = dlsym(handle.get(), name);
      if (!sym) return nullptr;
      return reinterpret_cast<T>(sym);
  }
  template<typename T>
  funcDescr<T> resolve(const char* name)
  {
      return funcDescr<T>(foo2<T>(name), name);
  }

  dlib() = default;
  dlib(dlib const&)=default;
  dlib(dlib &&)=default;
  dlib& operator=(dlib const&)=default;
  dlib& operator=(dlib &&)=default;

  dlib(const char* name, int flag) {
    void* h = dlopen(name, flag);
    std::cout << "load Dll/so " << name << std::endl;
    if (h)
    {
      // set handle to cleanup the dlopen:
      handle=std::shared_ptr<void>(
        h,
        [name](void* handle){
          std::cout << "unload Dll/so" << name << std::endl;
          int r = dlclose(handle);
          assert(r==0);
        }
      );
    }
  }
  explicit operator bool() const { return (bool)handle; }
private:
  std::shared_ptr<void> handle;
};
}
#endif // DLLHANDLER_H
