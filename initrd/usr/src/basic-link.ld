ENTRY(_start)
SECTIONS
{
	. = 5M;
	.text BLOCK(128K) : ALIGN(4K)
	{
		*(.text)
	}
}
