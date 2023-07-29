/*CPUID not CUPID*/
//code: https://github.com/pdoane/osdev/blob/master/cpu/detect.c

#include "../include/CPUID.h"
#include "../include/kernel.h"
#include "../include/console.h"
#include "../include/Core.h"

void CPUINFO::ShowCPUInfo() {
    Assembly _asm_;
    uint32 eax, ebx, ecx, edx;

    uint32 LSF;
    char vendor[13];
    _asm_.cpuid(0, &LSF, (uint32*)(vendor+0),(uint32 *)(vendor + 8), (uint32 *)(vendor + 4));
    vendor[12] = '\0';

    Console console;
    console.Write("CPU VENTOR: ");
    console.WriteLine(vendor);

    if (LSF >=0x01) {
        _asm_.cpuid(0x01, &eax, &ebx, &ecx, &edx);
        console.Write("Features: ");
        if (edx & CPUID_FEAT_EDX_PSE) console.WriteLine(" PSE");
        if (edx & CPUID_FEAT_EDX_PAE) console.WriteLine(" PAE");
        if (edx & CPUID_FEAT_EDX_APIC) console.WriteLine(" APIC");
        if (edx & CPUID_FEAT_EDX_MTRR) console.WriteLine(" MTRR");

        console.Write("Instructions: ");
        if (edx & CPUID_FEAT_EDX_TSC)       console.WriteLine(" TSC");
        if (edx & CPUID_FEAT_EDX_MSR)       console.WriteLine(" MSR");
        if (edx & CPUID_FEAT_EDX_SSE)       console.WriteLine(" SSE");
        if (edx & CPUID_FEAT_EDX_SSE2)      console.WriteLine(" SSE2");
        if (ecx & CPUID_FEAT_ECX_SSE3)      console.WriteLine(" SSE3");
        if (ecx & CPUID_FEAT_ECX_SSSE3)     console.WriteLine(" SSSE3");
        if (ecx & CPUID_FEAT_ECX_SSE4_1)    console.WriteLine(" SSE41");
        if (ecx & CPUID_FEAT_ECX_SSE4_2)    console.WriteLine(" SSE42");
        if (ecx & CPUID_FEAT_ECX_AVX)       console.WriteLine(" AVX");
        if (ecx & CPUID_FEAT_ECX_F16C)      console.WriteLine(" F16C");
        if (ecx & CPUID_FEAT_ECX_RDRAND)    console.WriteLine(" RDRAND");
        if (ecx & CPUID_FEAT_ECX_CX16)      console.WriteLine(" CX16");
    }

    uint32 LEF;
    _asm_.cpuid(0x80000000, &LEF, &ebx, &ecx, &edx);
    if (LEF >=0x80000001) {
        _asm_.cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
        if (edx & CPUID_FEAT_EDX_64_BIT) {
            console.WriteLine("Detected 64bit CPU!");
        } else {
            console.WriteLine("Detected 32bit CPU!");
        }
    }

    if (LEF >= 0x80000004) {
        char name[48];
        _asm_.cpuid(0x80000002, (uint32 *)(name +  0), (uint32 *)(name +  4), (uint32 *)(name +  8), (uint32 *)(name + 12));
        _asm_.cpuid(0x80000003, (uint32 *)(name + 16), (uint32 *)(name + 20), (uint32 *)(name + 24), (uint32 *)(name + 28));
        _asm_.cpuid(0x80000004, (uint32 *)(name + 32), (uint32 *)(name + 36), (uint32 *)(name + 40), (uint32 *)(name + 44));

        string2 p = name;
        while(*p == ' ') {
            ++p;
        }

        console.Write("CPU NAME: ");
        console.WriteLine(p);
    }

}

string CPUINFO::GetCPUID(void) {
    Assembly _asm_;
    uint32 eax, ebx, ecx, edx;

    uint32 LSF;
    char vendor[13];
    Utils utils;
    _asm_.cpuid(0, &LSF, (uint32*)(vendor+0),(uint32 *)(vendor + 8), (uint32 *)(vendor + 4));
    vendor[12] = '\0';
    string a = vendor;
    return a;
}

bool CPUINFO::CheckCPUArch() {
    uint32 LEF;
    uint32 eax, ebx, ecx, edx;
    Assembly _asm_;
    _asm_.cpuid(0x80000000, &LEF, &ebx, &ecx, &edx);
    if (LEF >=0x80000001) {
        _asm_.cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
        if (edx & CPUID_FEAT_EDX_64_BIT) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}