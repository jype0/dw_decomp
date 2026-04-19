SECTIONS
{
	OVERLAY 0x80052AE0 : NOCROSSREFS
	{
		.BTL {
			KEEP(build/asm/btl/data/texture.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);
			KEEP(build/asm/btl/data/model_0.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);
			KEEP(build/asm/btl/data/model_1.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);
			KEEP(build/asm/btl/data/model_2.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);
			KEEP(build/asm/btl/data/model_3.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			btl_TEXT_START = .;

			KEEP(build/asm/btl/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/btl/41C8.s.o(.text.*));
			. = ALIGN(., 4);

			KEEP(build/asm/btl/41C8.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/btl/41C8.s.o(.data.*));
			. = ALIGN(., 4);
			KEEP(build/asm/btl/data/20280.data.s.o(.data.*));
			. = ALIGN(., 4);

			KEEP(build/asm/btl/41C8.s.o(.bss.*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80080800 : NOCROSSREFS
	{
		.DGET {
			dget_TEXT_START = .;

			build/asm/dget/data/header.rodata.s.o(.rodata.*);
			. = ALIGN(., 4);

			build/src/dget/dget.c.o(.text.*);
			. = ALIGN(., 4);

			build/src/dget/dget.c.o(.rodata.*);
			. = ALIGN(., 4);

			build/src/dget/dget.c.o(.data.*);
			. = ALIGN(., 8);
		}
	}

	OVERLAY 0x80070000 : NOCROSSREFS
	{
		.DOO2 {
			doo2_TEXT_START = .;

			KEEP(build/asm/doo2/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/doo2/40.s.o(.text.*));
			. = ALIGN(., 4);

			KEEP(build/asm/doo2/40.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/doo2/40.s.o(.data.*));
			. = ALIGN(., 4);
			KEEP(build/asm/doo2/data/1A80.data.s.o(.data.*));
			. = ALIGN(., 4);

			KEEP(build/asm/doo2/40.s.o(.bss.*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80080000 : NOCROSSREFS
	{
		.DOOA {
			dooa_TEXT_START = .;

			KEEP(build/asm/dooa/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/dooa/40.s.o(.text.*));
			. = ALIGN(., 4);

			KEEP(build/asm/dooa/40.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/dooa/40.s.o(.data.*));
			. = ALIGN(., 4);
			KEEP(build/asm/dooa/data/48D4.data.s.o(.data.*));
			. = ALIGN(., 4);

			KEEP(build/asm/dooa/40.s.o(.bss.*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.EAB {
			eab_TEXT_START = .;

			KEEP(build/asm/eab/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/eab/40.s.o(.text.*));
			. = ALIGN(., 4);

			KEEP(build/asm/eab/40.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/eab/40.s.o(.data.*));
			. = ALIGN(., 4);
			KEEP(build/asm/eab/data/16DC.data.s.o(.data.*));
			. = ALIGN(., 4);

			KEEP(build/asm/eab/40.s.o(.bss.*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.ENDI {
			endi_TEXT_START = .;

			KEEP(build/asm/endi/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/endi/40.s.o(.text.*));
			. = ALIGN(., 4);

			KEEP(build/asm/endi/40.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/endi/40.s.o(.data.*));
			. = ALIGN(., 4);
			KEEP(build/asm/endi/data/D98.data.s.o(.data.*));
			. = ALIGN(., 4);

			KEEP(build/asm/endi/40.s.o(.bss.*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.EVL {
			evl_TEXT_START = .;

			KEEP(build/asm/evl/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/evl/40.s.o(.text.*));
			. = ALIGN(., 4);

			KEEP(build/asm/evl/40.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/evl/40.s.o(.data.*));
			. = ALIGN(., 4);
			KEEP(build/asm/evl/data/3EE8.data.s.o(.data.*));
			. = ALIGN(., 4);

			KEEP(build/asm/evl/40.s.o(.bss.*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80070000 : NOCROSSREFS
	{
		.FISH {
			fish_TEXT_START = .;

			KEEP(build/asm/fish/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/fish/40.s.o(.text.*));
			. = ALIGN(., 4);

			KEEP(build/asm/fish/40.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/fish/40.s.o(.data.*));
			. = ALIGN(., 4);
			KEEP(build/asm/fish/data/A160.data.s.o(.data.*));
			. = ALIGN(., 4);

			KEEP(build/asm/fish/40.s.o(.bss.*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80053800 : NOCROSSREFS
	{
		.KAR {
			kar_TEXT_START = .;

			KEEP(build/asm/kar/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/kar/40.s.o(.text.*));
			. = ALIGN(., 4);

			KEEP(build/asm/kar/40.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/kar/40.s.o(.data.*));
			. = ALIGN(., 4);
			KEEP(build/asm/kar/data/7310.data.s.o(.data.*));
			. = ALIGN(., 4);

			KEEP(build/asm/kar/40.s.o(.bss.*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80010000 : NOCROSSREFS
	{
		.MOV {
			mov_TEXT_START = .;

			KEEP(build/asm/mov/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/mov/40.s.o(.text.*));
			. = ALIGN(., 4);

			KEEP(build/asm/mov/40.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/mov/40.s.o(.data.*));
			. = ALIGN(., 4);
			KEEP(build/asm/mov/data/94C.data.s.o(.data.*));
			. = ALIGN(., 4);

			KEEP(build/asm/mov/40.s.o(.bss.*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x8007C000 : NOCROSSREFS
	{
		.MURD {
			murd_TEXT_START = .;

			KEEP(build/asm/murd/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/murd/40.s.o(.text.*));
			. = ALIGN(., 4);

			KEEP(build/asm/murd/40.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/murd/40.s.o(.data.*));
			. = ALIGN(., 4);
			KEEP(build/asm/murd/data/1748.data.s.o(.data.*));
			. = ALIGN(., 4);

			KEEP(build/asm/murd/40.s.o(.bss.*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80052AE0 : NOCROSSREFS
	{
		.STD {
			KEEP(build/asm/std/data/texture.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);
			KEEP(build/asm/std/data/model_0.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);
			KEEP(build/asm/std/data/model_1.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);
			KEEP(build/asm/std/data/model_2.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);
			KEEP(build/asm/std/data/model_3.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			std_TEXT_START = .;

			KEEP(build/asm/std/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/std/41C8.s.o(.text.*));
			. = ALIGN(., 4);

			KEEP(build/asm/std/41C8.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/std/41C8.s.o(.data.*));
			. = ALIGN(., 4);
			KEEP(build/asm/std/data/26E0C.data.s.o(.data.*));
			. = ALIGN(., 4);

			KEEP(build/asm/std/41C8.s.o(.bss.*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80088800 : NOCROSSREFS
	{
		.TRN2 {
			trn2_TEXT_START = .;

			KEEP(build/asm/trn2/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn2/40.s.o(.text.*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn2/40.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn2/40.s.o(.data.*));
			. = ALIGN(., 4);
			KEEP(build/asm/trn2/data/5130.data.s.o(.data.*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn2/40.s.o(.bss.*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80088800 : NOCROSSREFS
	{
		.TRN {
			trn_TEXT_START = .;

			KEEP(build/asm/trn/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn/40.s.o(.text.*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn/40.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn/40.s.o(.data.*));
			. = ALIGN(., 4);
			KEEP(build/asm/trn/data/6758.data.s.o(.data.*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn/40.s.o(.bss.*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80052AE0 : NOCROSSREFS
	{
		.VS {
			KEEP(build/asm/vs/data/texture.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);
			KEEP(build/asm/vs/data/model_0.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);
			KEEP(build/asm/vs/data/model_1.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);
			KEEP(build/asm/vs/data/model_2.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);
			KEEP(build/asm/vs/data/model_3.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			vs_TEXT_START = .;

			KEEP(build/asm/vs/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/vs/41C8.s.o(.text.*));
			. = ALIGN(., 4);

			KEEP(build/asm/vs/41C8.s.o(.rodata.*));
			. = ALIGN(., 4);

			KEEP(build/asm/vs/41C8.s.o(.data.*));
			. = ALIGN(., 4);
			KEEP(build/asm/vs/data/1CEDC.data.s.o(.data.*));
			. = ALIGN(., 4);

			KEEP(build/asm/vs/41C8.s.o(.bss.*));
			. = ALIGN(., 4);
		}
	}
}
