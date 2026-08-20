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

			build/src/btl/battle_setup.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/btl/battle_main.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/btl/command_shout.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/btl/command_menu.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/btl/battle_hud.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/btl/battle_effect.c.o(.text.*);
			. = ALIGN(., 4);

			build/src/btl/battle_main.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/btl/battle_setup.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/btl/command_menu.c.o(.rodata.*);
			. = ALIGN(., 4);
			KEEP(build/asm/btl/data/command_shout.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);
			build/src/btl/command_shout.c.o(.rodata.*);
			. = ALIGN(., 4);
			KEEP(build/asm/btl/data/battle_hud.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);
			build/src/btl/battle_hud.c.o(.rodata.*);
			. = ALIGN(., 4);
			KEEP(build/asm/btl/data/battle_effect.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			build/src/btl/battle_setup.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/btl/battle_main.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/btl/command_shout.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/btl/command_menu.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/btl/battle_hud.c.o(.data.*);
			. = ALIGN(., 4);
			KEEP(build/asm/btl/data/20B24.data.s.o(.data.*));
			. = ALIGN(., 4);

			build/src/btl/battle_setup.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/btl/battle_main.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/btl/command_shout.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/btl/command_menu.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/btl/battle_hud.c.o(.bss.*);
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

			build/src/eab/eab.c.o(.text.*);
			. = ALIGN(., 4);

			build/src/eab/eab.c.o(.rodata.*);
			. = ALIGN(., 4);

			build/src/eab/eab.c.o(.data.*);
			. = ALIGN(., 4);
			KEEP(build/asm/eab/data/16DC.data.s.o(.data.*));
			. = ALIGN(., 4);

			build/src/eab/eab.c.o(.bss.*);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80060000 : NOCROSSREFS
	{
		.ENDI {
			endi_TEXT_START = .;

			KEEP(build/asm/endi/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			build/src/endi/endi.c.o(.text.*);
			. = ALIGN(., 4);

			build/src/endi/endi.c.o(.rodata.*);
			. = ALIGN(., 4);

			build/src/endi/endi.c.o(.data.*);
			. = ALIGN(., 4);

			build/src/endi/endi.c.o(.bss.*);
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

			build/src/murd/murd.c.o(.text.*);
			. = ALIGN(., 4);

			build/src/murd/murd.c.o(.rodata.*);
			. = ALIGN(., 4);

			build/src/murd/murd.c.o(.data.*);
			. = ALIGN(., 4);
			KEEP(build/asm/murd/data/1748.data.s.o(.data.*));
			. = ALIGN(., 4);

			build/src/murd/murd.c.o(.bss.*);
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

			build/src/std/std_setup.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/std/std_main.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/std/std_hud.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/std/std_effect.c.o(.text.*);
			. = ALIGN(., 4);

			build/src/std/std_setup.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/std/std_main.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/std/std_hud.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/std/std_effect.c.o(.rodata.*);
			. = ALIGN(., 4);

			build/src/std/std_setup.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/std/std_main.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/std/std_hud.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/std/std_effect.c.o(.data.*);
			. = ALIGN(., 4);
			KEEP(build/asm/std/data/26E58.data.s.o(.data.*));
			. = ALIGN(., 4);

			build/src/std/std_setup.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/std/std_main.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/std/std_hud.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/std/std_effect.c.o(.bss.*);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80088800 : NOCROSSREFS
	{
		.TRN2 {
			trn2_TEXT_START = .;

			KEEP(build/asm/trn2/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			build/src/trn2/trn2_hp_map99.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_def_map99.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_hp_map107.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_mp.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_off.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_def_map108.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_reward.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_hud.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_slots.c.o(.text.*);
			. = ALIGN(., 4);

			build/src/trn2/trn2_hp_map107.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_hp_map99.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_mp.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_off.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_reward.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_def_map108.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_def_map99.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_hud.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_slots.c.o(.rodata.*);
			. = ALIGN(., 4);

			build/src/trn2/trn2_hp_map99.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_def_map99.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_hp_map107.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_mp.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_off.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_def_map108.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_reward.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_hud.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_slots.c.o(.data.*);
			. = ALIGN(., 4);
			KEEP(build/asm/trn2/data/5220.data.s.o(.data.*));
			. = ALIGN(., 4);

			build/src/trn2/trn2_hp_map99.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_def_map99.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_hp_map107.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_mp.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_off.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_def_map108.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_reward.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_hud.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn2/trn2_slots.c.o(.bss.*);
			. = ALIGN(., 4);
		}
	}

	OVERLAY 0x80088800 : NOCROSSREFS
	{
		.TRN {
			trn_TEXT_START = .;

			KEEP(build/asm/trn/data/header.rodata.s.o(.rodata.*));
			. = ALIGN(., 4);

			build/src/trn/trn_reward.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn/trn_hp.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn/trn_mp.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn/trn_off.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn/trn_def.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn/trn_speed.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn/trn_brain.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn/trn_hud.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/trn/trn_slots.c.o(.text.*);
			. = ALIGN(., 4);

			build/src/trn/trn_hp.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn/trn_off.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn/trn_speed.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn/trn_def.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn/trn_mp.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn/trn_brain.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn/trn_reward.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn/trn_slots.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/trn/trn_hud.c.o(.rodata.*);
			. = ALIGN(., 4);

			build/src/trn/trn_reward.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn/trn_hp.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn/trn_mp.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn/trn_off.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn/trn_def.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn/trn_speed.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn/trn_brain.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn/trn_hud.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/trn/trn_slots.c.o(.data.*);
			. = ALIGN(., 4);
			KEEP(build/asm/trn/data/6850.data.s.o(.data.*));
			. = ALIGN(., 4);

			build/src/trn/trn_reward.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn/trn_hp.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn/trn_mp.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn/trn_off.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn/trn_def.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn/trn_speed.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn/trn_brain.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn/trn_hud.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/trn/trn_slots.c.o(.bss.*);
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

			build/src/vs/vs_scene.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/vs/vs_main.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/vs/vs_hud.c.o(.text.*);
			. = ALIGN(., 4);
			build/src/vs/vs_effect.c.o(.text.*);
			. = ALIGN(., 4);

			build/src/vs/vs_scene.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/vs/vs_main.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/vs/vs_hud.c.o(.rodata.*);
			. = ALIGN(., 4);
			build/src/vs/vs_effect.c.o(.rodata.*);
			. = ALIGN(., 4);

			build/src/vs/vs_scene.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/vs/vs_main.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/vs/vs_hud.c.o(.data.*);
			. = ALIGN(., 4);
			build/src/vs/vs_effect.c.o(.data.*);
			. = ALIGN(., 4);
			KEEP(build/asm/vs/data/1CF48.data.s.o(.data.*));
			. = ALIGN(., 4);

			build/src/vs/vs_scene.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/vs/vs_main.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/vs/vs_hud.c.o(.bss.*);
			. = ALIGN(., 4);
			build/src/vs/vs_effect.c.o(.bss.*);
			. = ALIGN(., 4);
		}
	}
}
