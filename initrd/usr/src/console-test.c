int _start()
{
	*(unsigned char*) 0xB8000 = '@';
	asm("cli; hlt");
}
