#include <cstddef>
#include <cstring>
#include <new>
#include <string>

#include "absl/strings/string_view.h"
#include "tcmalloc/malloc_extension.h"

extern "C" {
    bool NeedsProcessBackgroundActions() {
        return tcmalloc::MallocExtension::NeedsProcessBackgroundActions();
    }

    void ProcessBackgroundActions() {
        tcmalloc::MallocExtension::ProcessBackgroundActions();
    }

    bool BridgeGetNumericProperty(const char* name, size_t len, size_t* value) {
        const auto result =
            tcmalloc::MallocExtension::GetNumericProperty(absl::string_view{name, len});
        if (!result.has_value()) {
            return false;
        }
        *value = *result;
        return true;
    }

    bool BridgeGetStats(const char** data, size_t* len) {
        if (data == nullptr || len == nullptr) {
            return false;
        }

        const std::string stats = tcmalloc::MallocExtension::GetStats();
        if (stats.empty()) {
            *data = nullptr;
            *len = 0;
            return true;
        }

        char* copy = new (std::nothrow) char[stats.size()];
        if (copy == nullptr) {
            return false;
        }
        std::memcpy(copy, stats.data(), stats.size());
        *data = copy;
        *len = stats.size();
        return true;
    }

    void BridgeFreeStats(const char* data, size_t len) {
        (void)len;
        delete[] data;
    }
}
