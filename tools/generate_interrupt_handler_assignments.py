text_ = """
{{
    const unsigned int f = reinterpret_cast<unsigned int>(&unimplemented_interrupt_handler_{0});
    construct_idt_descriptor(low, high, type_32bit_interrupt_gate, f);
    create_interrupt_descriptor({0}, low, high);
}}
"""
for i in range(256):
    print(text_.format(i))

