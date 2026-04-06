/* See LICENSE file for copyright and license details. */

SECTIONS
{
	OVERLAY 0x80052AE0 : NOCROSSREFS SUBALIGN(4)
	{
		.BTL {
			KEEP(build/asm/btl/data/texture.rodata.s.o(.rodata .rodata*));
			KEEP(build/asm/btl/data/model_0.rodata.s.o(.rodata .rodata*));
			KEEP(build/asm/btl/data/model_1.rodata.s.o(.rodata .rodata*));
			KEEP(build/asm/btl/data/model_2.rodata.s.o(.rodata .rodata*));
			KEEP(build/asm/btl/data/model_3.rodata.s.o(.rodata .rodata*));

			btl_TEXT_START = .;

			KEEP(build/asm/btl/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/btl/41C8.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/btl/41C8.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/btl/41C8.s.o(.data .data*));
			KEEP(build/asm/btl/data/20280.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/btl/41C8.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80080800 : NOCROSSREFS SUBALIGN(4)
	{
		.DGET {
			dget_TEXT_START = .;

			KEEP(build/asm/dget/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/dget/40.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/dget/40.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/dget/40.s.o(.data .data*));
			KEEP(build/asm/dget/data/14CC.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/dget/40.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80070000 : NOCROSSREFS SUBALIGN(4)
	{
		.DOO2 {
			doo2_TEXT_START = .;

			KEEP(build/asm/doo2/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/doo2/40.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/doo2/40.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/doo2/40.s.o(.data .data*));
			KEEP(build/asm/doo2/data/1A80.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/doo2/40.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80080000 : NOCROSSREFS SUBALIGN(4)
	{
		.DOOA {
			dooa_TEXT_START = .;

			KEEP(build/asm/dooa/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/dooa/40.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/dooa/40.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/dooa/40.s.o(.data .data*));
			KEEP(build/asm/dooa/data/48D4.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/dooa/40.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS SUBALIGN(4)
	{
		.EAB {
			eab_TEXT_START = .;

			KEEP(build/asm/eab/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/eab/40.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/eab/40.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/eab/40.s.o(.data .data*));
			KEEP(build/asm/eab/data/16DC.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/eab/40.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS SUBALIGN(4)
	{
		.ENDI {
			endi_TEXT_START = .;

			KEEP(build/asm/endi/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/endi/40.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/endi/40.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/endi/40.s.o(.data .data*));
			KEEP(build/asm/endi/data/D98.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/endi/40.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS SUBALIGN(4)
	{
		.EVL {
			evl_TEXT_START = .;

			KEEP(build/asm/evl/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/evl/40.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/evl/40.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/evl/40.s.o(.data .data*));
			KEEP(build/asm/evl/data/3EE8.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/evl/40.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80070000 : NOCROSSREFS SUBALIGN(4)
	{
		.FISH {
			fish_TEXT_START = .;

			KEEP(build/asm/fish/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/fish/40.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/fish/40.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/fish/40.s.o(.data .data*));
			KEEP(build/asm/fish/data/A160.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/fish/40.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80053800 : NOCROSSREFS SUBALIGN(4)
	{
		.KAR {
			kar_TEXT_START = .;

			KEEP(build/asm/kar/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/kar/40.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/kar/40.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/kar/40.s.o(.data .data*));
			KEEP(build/asm/kar/data/7310.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/kar/40.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80010000 : NOCROSSREFS SUBALIGN(4)
	{
		.MOV {
			mov_TEXT_START = .;

			KEEP(build/asm/mov/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/mov/40.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/mov/40.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/mov/40.s.o(.data .data*));
			KEEP(build/asm/mov/data/94C.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/mov/40.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x8007C000 : NOCROSSREFS SUBALIGN(4)
	{
		.MURD {
			murd_TEXT_START = .;

			KEEP(build/asm/murd/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/murd/40.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/murd/40.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/murd/40.s.o(.data .data*));
			KEEP(build/asm/murd/data/1748.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/murd/40.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80080800 : NOCROSSREFS SUBALIGN(4)
	{
		.SHOP {
			shop_TEXT_START = .;

			KEEP(build/asm/shop/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/shop/40.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/shop/40.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/shop/40.s.o(.data .data*));
			KEEP(build/asm/shop/data/7860.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/shop/40.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80052AE0 : NOCROSSREFS SUBALIGN(4)
	{
		.STD {
			KEEP(build/asm/std/data/texture.rodata.s.o(.rodata .rodata*));
			KEEP(build/asm/std/data/model_0.rodata.s.o(.rodata .rodata*));
			KEEP(build/asm/std/data/model_1.rodata.s.o(.rodata .rodata*));
			KEEP(build/asm/std/data/model_2.rodata.s.o(.rodata .rodata*));
			KEEP(build/asm/std/data/model_3.rodata.s.o(.rodata .rodata*));

			std_TEXT_START = .;

			KEEP(build/asm/std/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/std/41C8.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/std/41C8.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/std/41C8.s.o(.data .data*));
			KEEP(build/asm/std/data/26E0C.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/std/41C8.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80088800 : NOCROSSREFS SUBALIGN(4)
	{
		.TRN2 {
			trn2_TEXT_START = .;

			KEEP(build/asm/trn2/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn2/40.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn2/40.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn2/40.s.o(.data .data*));
			KEEP(build/asm/trn2/data/5130.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn2/40.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80088800 : NOCROSSREFS SUBALIGN(4)
	{
		.TRN {
			trn_TEXT_START = .;

			KEEP(build/asm/trn/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn/40.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn/40.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn/40.s.o(.data .data*));
			KEEP(build/asm/trn/data/6758.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/trn/40.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80052AE0 : NOCROSSREFS SUBALIGN(4)
	{
		.VS {
			KEEP(build/asm/vs/data/texture.rodata.s.o(.rodata .rodata*));
			KEEP(build/asm/vs/data/model_0.rodata.s.o(.rodata .rodata*));
			KEEP(build/asm/vs/data/model_1.rodata.s.o(.rodata .rodata*));
			KEEP(build/asm/vs/data/model_2.rodata.s.o(.rodata .rodata*));
			KEEP(build/asm/vs/data/model_3.rodata.s.o(.rodata .rodata*));

			vs_TEXT_START = .;

			KEEP(build/asm/vs/data/header.rodata.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/vs/41C8.s.o(.text .text*));
			. = ALIGN(., 4);

			KEEP(build/asm/vs/41C8.s.o(.rodata .rodata*));
			. = ALIGN(., 4);

			KEEP(build/asm/vs/41C8.s.o(.data .data*));
			KEEP(build/asm/vs/data/1CEDC.data.s.o(.data .data*));
			. = ALIGN(., 4);

			KEEP(build/asm/vs/41C8.s.o(.bss .bss*));
			. = ALIGN(., 4);
		}
	}
}
