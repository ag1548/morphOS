# TODOs

- Abstract Keyboard and Mouse away into PS2 controller device superclasses

- Implement the EFER.NXE flag in Virtual Memory:

```
The page-table entries carry the permission bits. In Intel's manual, protection information is held in the read/write flag (bit 1) and the user/supervisor flag (bit 2), and the processor checks those flags in all paging structures on the path. PAE also adds an execute-disable bit (bit 63), which blocks instruction fetches when EFER.NXE is enabled.
```
