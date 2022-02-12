ENTRY(start)
SECTIONS
{
	/**
	 * 1. the multiboot header
	 * 2. the .text section
	 */
	.text BLOCK(4K) : ALIGN(4K)
	{
	    _kernel_seg_code = .;
		*(.text)
	}

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
	    _kernel_seg_data = .;
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
	    _kernel_seg_bss = .;
		*(.bss)
	}

    _kernel_seg_end = .;
}
