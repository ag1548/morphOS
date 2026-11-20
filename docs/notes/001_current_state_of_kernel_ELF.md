# Kernel ELF
`$ readelf -hlSW kernel`

```text
ELF Header:
  Magic:   7f 45 4c 46 01 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF32
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Intel 80386
  Version:                           0x1
  Entry point address:               0x100030
  Start of program headers:          52 (bytes into file)
  Start of section headers:          32456 (bytes into file)
  Flags:                             0x0
  Size of this header:               52 (bytes)
  Size of program headers:           32 (bytes)
  Number of program headers:         2
  Size of section headers:           40 (bytes)
  Number of section headers:         8
  Section header string table index: 7

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .text             PROGBITS        00100000 001000 0036e9 00  AX  0   0 4096
  [ 2] .rodata           PROGBITS        00104000 005000 000fc2 00   A  0   0 4096
  [ 3] .bss              NOBITS          00105000 006000 004860 00  WA  0   0 4096
  [ 4] .comment          PROGBITS        00000000 005fc2 000012 01  MS  0   0  1
  [ 5] .symtab           SYMTAB          00000000 005fd4 000b70 10      6  52  4
  [ 6] .strtab           STRTAB          00000000 006b44 00134d 00      0   0  1
  [ 7] .shstrtab         STRTAB          00000000 007e91 000037 00      0   0  1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  D (mbind), p (processor specific)

Program Headers:
  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
  LOAD           0x001000 0x00100000 0x00100000 0x04fc2 0x04fc2 R E 0x1000
  LOAD           0x001000 0x00105000 0x00105000 0x00000 0x04860 RW  0x1000

 Section to Segment mapping:
  Segment Sections...
   00     .text .rodata 
   01     .bss 
```
