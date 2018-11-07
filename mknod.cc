#include <napi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace Napi;

class MknodWorker : public Napi::AsyncWorker {
 public:
  MknodWorker(Napi::Function& callback, char *path, int mode, int dev)
    : Napi::AsyncWorker(callback), path(path), mode(mode), dev(dev) {}
  ~MknodWorker() {}

  void Execute () {
    //this->error = mknod(path, mode, dev);
    free(path);
  }

   void OnOK () {
    Napi::HandleScope scope(Env());

    if (this->error) {
      printf("hello");

      Callback().Call(
        Receiver().Value(),
        std::initializer_list<napi_value>{
          // Napi::Error::New(Env(), "mknod failed")
          Napi::String::New(Env(), "mknod failed")
        }
      );
    } else {
      Callback().Call(Receiver().Value(), std::initializer_list<napi_value>{});
    }
  }

  // void OnError(const Napi::Error& e)
  // {
  //   Callback().MakeCallback(
  //     Receiver().Value(),
  //     {
  //       1,
  //       e.Value()
  //     }
  //   );
  // }

 private:
  char *path;
  int mode;
  int dev;
  int error;
};

void Mknod(const Napi::CallbackInfo& info) {

  //Napi::HandleScope scope;

  Napi::Env env = info.Env();

  if (!info[0].IsString()) Napi::Error::New(env, "path must be a string").ThrowAsJavaScriptException();
  std::string path = info[0].As<Napi::String>().Utf8Value();

  if (!info[1].IsNumber()) Napi::Error::New(env, "mode must be a number").ThrowAsJavaScriptException();
  int mode = info[1].As<Napi::Number>().Uint32Value();

  if (!info[2].IsNumber()) Napi::Error::New(env, "dev must be a number").ThrowAsJavaScriptException();
  int dev = info[2].As<Napi::Number>().Uint32Value();

  if (!info[3].IsFunction()) Napi::Error::New(env, "callback must be a function").ThrowAsJavaScriptException();
  Napi::Function callback = info[3].As<Napi::Function>();

  printf("hey");

  char *path_alloc = (char *) malloc(1024);
  stpcpy(path_alloc, path.c_str());

  printf("plop");

  (new MknodWorker(callback, path_alloc, mode, dev))->Queue();
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "mknod"),
              Napi::Function::New(env, Mknod));
  return exports;
}

NODE_API_MODULE(mknod, InitAll)
