SECTIONS
{
	OVERLAY 0x80052AE0 : NOCROSSREFS
	{
		.BTL {
			btl_START = .;

			KEEP(BUILD_DIR/asm/btl/data/texture.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/data/model_0.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/data/model_1.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/data/model_2.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/data/model_3.rodata.s.o(.rodata .rodata.*));
			btl_HEADER = .;
			KEEP(BUILD_DIR/asm/btl/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/btl/btl.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/btl/data/btl.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80080800 : NOCROSSREFS
	{
		.DGET {
			dget_START = .;

			dget_HEADER = .;
			KEEP(BUILD_DIR/asm/dget/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/dget/dget.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/dget/data/dget.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80070000 : NOCROSSREFS
	{
		.DOO2 {
			doo2_START = .;

			doo2_HEADER = .;
			KEEP(BUILD_DIR/asm/doo2/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/doo2/doo2.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/doo2/data/doo2.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80080000 : NOCROSSREFS
	{
		.DOOA {
			dooa_START = .;

			dooa_HEADER = .;
			KEEP(BUILD_DIR/asm/dooa/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/dooa/dooa.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/dooa/data/dooa.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.EAB {
			eab_START = .;

			eab_HEADER = .;
			KEEP(BUILD_DIR/asm/eab/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/eab/eab.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/eab/data/eab.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.ENDI {
			endi_START = .;

			endi_HEADER = .;
			KEEP(BUILD_DIR/asm/endi/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/endi/endi.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/endi/data/endi.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.EVL {
			evl_START = .;

			evl_HEADER = .;
			KEEP(BUILD_DIR/asm/evl/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/evl/evl.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/evl/data/evl.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80070000 : NOCROSSREFS
	{
		.FISH {
			fish_START = .;

			fish_HEADER = .;
			KEEP(BUILD_DIR/asm/fish/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/fish/fish.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/fish/data/fish.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80053800 : NOCROSSREFS
	{
		.KAR {
			kar_START = .;

			kar_HEADER = .;
			KEEP(BUILD_DIR/asm/kar/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/kar/kar.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/kar/data/kar.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80010000 : NOCROSSREFS
	{
		.MOV {
			mov_START = .;

			mov_HEADER = .;
			KEEP(BUILD_DIR/asm/mov/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/mov/mov.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/mov/data/mov.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x8007C000 : NOCROSSREFS
	{
		.MURD {
			murd_START = .;

			murd_HEADER = .;
			KEEP(BUILD_DIR/asm/murd/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/murd/murd.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/murd/data/murd.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80080800 : NOCROSSREFS
	{
		.SHOP {
			shop_START = .;

			shop_HEADER = .;
			KEEP(BUILD_DIR/asm/shop/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/shop/shop.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/shop/data/shop.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80052AE0 : NOCROSSREFS
	{
		.STD {
			std_START = .;

			KEEP(BUILD_DIR/asm/std/data/texture.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/std/data/model_0.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/std/data/model_1.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/std/data/model_2.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/std/data/model_3.rodata.s.o(.rodata .rodata.*));
			std_HEADER = .;
			KEEP(BUILD_DIR/asm/std/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/std/std.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/std/data/std.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80088800 : NOCROSSREFS
	{
		.TRN2 {
			trn2_START = .;

			trn2_HEADER = .;
			KEEP(BUILD_DIR/asm/trn2/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/trn2/trn2.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/trn2/data/trn2.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80088800 : NOCROSSREFS
	{
		.TRN {
			trn_START = .;

			trn_HEADER = .;
			KEEP(BUILD_DIR/asm/trn/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/trn/trn.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/trn/data/trn.data.s.o(.data .data.*));
			LONG(0);
		}
	}

	OVERLAY 0x80052AE0 : NOCROSSREFS
	{
		.VS {
			vs_START = .;

			KEEP(BUILD_DIR/asm/vs/data/texture.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/vs/data/model_0.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/vs/data/model_1.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/vs/data/model_2.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/vs/data/model_3.rodata.s.o(.rodata .rodata.*));
			vs_HEADER = .;
			KEEP(BUILD_DIR/asm/vs/data/header.rodata.s.o(.rodata .rodata.*));
			KEEP(BUILD_DIR/asm/vs/vs.s.o(.text .text.*));
			KEEP(BUILD_DIR/asm/vs/data/vs.data.s.o(.data .data.*));
			LONG(0);
		}
	}
}
