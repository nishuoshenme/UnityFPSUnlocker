#include "houdini.hh"

#if defined(__i386__) || defined(__x86_64__)

#include <dlfcn.h>

#ifdef __x86_64__
#define syslib "/system/lib64/"
#elif defined(__i386__)
#define syslib "/system/lib/"
#endif

namespace {
    using NativeBridgeGetVersion_t = uint32_t (*)();
    using NativeBridgeLoadLibrary_t = void* (*)(const char*, int);
    using NativeBridgeLoadLibraryExt_t = void* (*)(const char*, int, void*);
    using NativeBridgeGetTrampoline_t = void* (*)(void* handle, const char* name, const char* shorty, uint32_t len);
    using NativeBridgeGetError_t = const char* (*)();

    static NativeBridgeGetVersion_t NativeBridgeGetVersion = nullptr;
} // namespace

Houdini::Houdini() {
    void* libhoudini = dlopen("libhoudini.so", RTLD_NOW);
    if (libhoudini) {
        houdini_itf_ = reinterpret_cast<android::NativeBridgeCallbacks*>(dlsym(libhoudini, "NativeBridgeItf"));
        if (houdini_itf_) {
            houdini_ver_ = houdini_itf_->version;
        }
    }
    else {
        void* libnativebridge = dlopen(syslib "libnativebridge.so", RTLD_NOW);
        if (libnativebridge) {
            NativeBridgeGetVersion = (NativeBridgeGetVersion_t)dlsym(libnativebridge, "_ZN7android22NativeBridgeGetVersionEv");

            houdini_itf_ = new android::NativeBridgeCallbacks();
            houdini_itf_->loadLibrary = reinterpret_cast<NativeBridgeLoadLibrary_t>(dlsym(libnativebridge, "_ZN7android23NativeBridgeLoadLibraryEPKci"));
            houdini_itf_->loadLibraryExt = reinterpret_cast<NativeBridgeLoadLibraryExt_t>(dlsym(libnativebridge, "_ZN7android26NativeBridgeLoadLibraryExtEPKciPNS_25native_bridge_namespace_tE"));
            houdini_itf_->getTrampoline = reinterpret_cast<NativeBridgeGetTrampoline_t>(dlsym(libnativebridge, "_ZN7android25NativeBridgeGetTrampolineEPvPKcS2_j"));

            houdini_ver_ = NativeBridgeGetVersion();

            if (houdini_ver_ > 2) {
                houdini_itf_->getError = reinterpret_cast<NativeBridgeGetError_t>(dlsym(libnativebridge, "_ZN7android20NativeBridgeGetErrorEv"));
            }
        }
    }
}

absl::StatusOr<void*> Houdini::LoadLibrary(const char* name, int flag) {
    if (houdini_ver_ > 0 && houdini_itf_) {
        if (houdini_ver_ == 2) {
            return houdini_itf_->loadLibrary(name, flag);
        }
        else if (houdini_ver_ == 3) {
            return houdini_itf_->loadLibraryExt(name, flag, (void*)3);
        }
        else {
            return houdini_itf_->loadLibraryExt(name, flag, (void*)5);
        }
    }
    return absl::InternalError("Houdini init error");
}

absl::Status Houdini::CallJNI(void* handle, void* vm, void* reserved) {
    using JNI_OnLoad_t = int (*)(void*, void*);
    JNI_OnLoad_t jni_onload_ptr = nullptr;
    if (houdini_ver_ > 0 && houdini_itf_) {
        jni_onload_ptr = reinterpret_cast<JNI_OnLoad_t>(houdini_itf_->getTrampoline(handle, "JNI_OnLoad", nullptr, 0));

        if (jni_onload_ptr) {
            jni_onload_ptr(vm, reserved);
            return absl::OkStatus();
        }
        else {
            return absl::NotFoundError("JNI_OnLoad not found");
        }
    }
    return absl::InternalError("Houdini init error");
}

const char* Houdini::GetError() {
    if (houdini_ver_ > 2) {
        if (houdini_itf_) {
            auto ptr = reinterpret_cast<android::NativeBridgeCallbacks*>(houdini_itf_);
            return ptr->getError();
        }
    }
    return "(null)";
}

#endif // architecture defined