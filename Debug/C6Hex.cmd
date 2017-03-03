test.out
--binary
--map=Thex.map
--order=L

--boot
--bootorg=0xB0040000

--bootsection=.boot_load 0xB0000000
/*
--bootsection=.prog 0xB0020000
*/
--image
--zero
--fill=0xFFFFFFFF

ROMS
{
	FLASH: org = 0xB0000000, len = 0x50000, romwidth = 8, memwidth = 8 files={T.bin}
}


/*
--outfile=MT.bin
--memwidth=8
--romwidth=8

*/
