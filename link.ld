ENTRY(_start)
SECTIONS
{
	. = 1M;
	__cx_kernel_start_marker = .;
	.text BLOCK(128K) : ALIGN(4K)
	{
		text_sect_phys_addr = .;
        *(.multiboot)
        *(.init)
        *(.fini)
		*(.text)
	}
	.rodata BLOCK(16K) : ALIGN(4K)
	{
		rodata_sect_phys_addr = .;
		*(.rodata)
	}
	.data BLOCK(16K) : ALIGN(4K)
	{
		data_sect_phys_addr = .;
		*(.data)
	}

/* Bootstrap kernel stack - shall be replaced or deleted later */
	.bss BLOCK(16K) : ALIGN(4K)
	{
		bss_sect_phys_addr = .;
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
		__cx_kernel_end_marker = .;
	}
}

