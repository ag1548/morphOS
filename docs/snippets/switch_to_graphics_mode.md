# Switching to Graphics Mode

- VBE - "VESA BIOS Extensions"

```c
#define VBE_DISPI_IOPORT_INDEX 0x01CE
#define VBE_DISPI_IOPORT_DATA  0x01CF

#define VBE_DISPI_INDEX_ID     0
#define VBE_DISPI_INDEX_XRES   1
#define VBE_DISPI_INDEX_YRES   2
#define VBE_DISPI_INDEX_BPP    3
#define VBE_DISPI_INDEX_ENABLE 4

void bochs_vbe_write(unsigned short index, unsigned short value) {
    // Send index to port 0x1CE
    asm volatile("outw %0, %1" :: "a"(index), "Nd"((unsigned short)VBE_DISPI_IOPORT_INDEX));
    // Send value to port 0x1CF
    asm volatile("outw %0, %1" :: "a"(value), "Nd"((unsigned short)VBE_DISPI_IOPORT_DATA));
}

void switch_to_graphics_mode(unsigned short width, unsigned short height, unsigned short bpp) {

    // Disable VBE temporarily to change settings
    bochs_vbe_write(VBE_DISPI_INDEX_ENABLE, 0);

    bochs_vbe_write(VBE_DISPI_INDEX_XRES, width);
    bochs_vbe_write(VBE_DISPI_INDEX_YRES, height);
    bochs_vbe_write(VBE_DISPI_INDEX_BPP, bpp);

    // Enable VBE with linear framebuffer (bit 1) + clear video memory flag (bit 6)
    bochs_vbe_write(VBE_DISPI_INDEX_ENABLE, 0x01 | 0x40);
}
```
