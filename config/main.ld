ENTRY(EntryPoint)

SECTIONS
{
    __romPos = 0;
    _gp = main_SDATA_START + 0x8000;
    header_ROM_START = __romPos;
    .header 0x80090000 : AT(header_ROM_START)
    {
        KEEP(build/asm/main/data/header.data.s.o(.data.*));
        . = ALIGN(., 4);
    }
    __romPos += SIZEOF(.header);
    __romPos = ALIGN(__romPos, 4);
    . = ALIGN(., 4);

    main_ROM_START = __romPos;
    .main : AT(main_ROM_START)
    {
        main_TEXT_START = .;
        KEEP(build/asm/main/psyq/libapi.s.o(.text.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libc.s.o(.text.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libetc.s.o(.text.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libgpu.s.o(.text.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libgs.s.o(.text.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libgte.s.o(.text.*))
        . = ALIGN(., 4);
        build/asm/main/model.s.o(.text.*)
        . = ALIGN(., 4);
        build/src/main/world_object.c.o(.text.*)
        . = ALIGN(., 4);
        build/src/main/file.c.o(.text.*)
        . = ALIGN(., 4);
        build/src/main/math.c.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/graphics.s.o(.text.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/entrypoint.s.o(.text.*))
        . = ALIGN(., 4);
        build/asm/main/partner_impl.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/tamer.s.o(.text.*)
        . = ALIGN(., 4);
        build/src/main/fade.c.o(.text.*)
        . = ALIGN(., 4);
        build/src/main/aabb.c.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/efe.s.o(.text.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libcd.s.o(.text.*))
        . = ALIGN(., 4);
        build/asm/main/map_impl.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/game_menu.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/map_collision.s.o(.text.*)
        . = ALIGN(., 4);
        build/src/main/particle.c.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/anim.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/ui.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/item.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/sound.s.o(.text.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libspu.s.o(.text.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libsnd.s.o(.text.*))
        . = ALIGN(., 4);
        build/asm/main/map.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/inventory.s.o(.text.*)
        . = ALIGN(., 4);
        build/src/main/clock.c.o(.text.*)
        . = ALIGN(., 4);
        build/src/main/partner.c.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/entity_text.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/tmd_object.s.o(.text.*)
        . = ALIGN(., 4);
        build/src/main/evolution.c.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/file_queue.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/graphics2.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/fish.s.o(.text.*)
        . = ALIGN(., 4);
        build/src/main/swap.c.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/utils.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/tournament.s.o(.text.*)
        . = ALIGN(., 4);
        build/src/main/evl.c.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/btl.s.o(.text.*)
        . = ALIGN(., 4);
        build/src/main/kar.c.o(.text.*)
        . = ALIGN(., 4);
        build/src/main/efe_table.c.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/sound_async.s.o(.text.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libpress.s.o(.text.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libds.s.o(.text.*))
        . = ALIGN(., 4);
        build/src/main/butterfly.c.o(.text.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libmcrd.s.o(.text.*))
        . = ALIGN(., 4);
        build/src/main/bubble.c.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/battle_ui.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/main.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/vs.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/script.s.o(.text.*)
        . = ALIGN(., 4);
        build/asm/main/main_menu.s.o(.text.*)
        . = ALIGN(., 4);
        main_TEXT_END = .;
        main_TEXT_SIZE = ABSOLUTE(main_TEXT_END - main_TEXT_START);

        main_RODATA_START = .;
        KEEP(build/asm/main/data/entrypoint.rodata.s.o(.rodata.*))
        . = ALIGN(., 4);
        build/asm/main/partner_impl.s.o(.rodata.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libc.rodata.s.o(.rodata.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libetc.rodata.s.o(.rodata.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libgpu.rodata.s.o(.rodata.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libgs.rodata.s.o(.rodata.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libgte.s.o(.rodata.*))
        . = ALIGN(., 4);
        build/asm/main/model.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/src/main/world_object.c.o(.rodata.*)
        . = ALIGN(., 4);
        build/src/main/file.c.o(.rodata.*)
        . = ALIGN(., 4);
        build/src/main/math.c.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/graphics.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/data/tamer.rodata.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/src/main/fade.c.o(.rodata.*)
        . = ALIGN(., 4);
        build/src/main/aabb.c.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/data/efe.rodata.s.o(.rodata.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libcd.rodata.s.o(.rodata.*))
        . = ALIGN(., 4);
        build/asm/main/data/map_impl.rodata.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/data/game_menu.rodata.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/map_collision.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/src/main/particle.c.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/anim.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/ui.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/data/item.rodata.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/sound.s.o(.rodata.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libspu.rodata.s.o(.rodata.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libsnd.rodata.s.o(.rodata.*))
        . = ALIGN(., 4);
        build/asm/main/data/map.rodata.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/inventory.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/src/main/clock.c.o(.rodata.*)
        . = ALIGN(., 4);
        build/src/main/partner.c.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/entity_text.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/data/tmd_object.rodata.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/src/main/evolution.c.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/file_queue.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/data/graphics2.rodata.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/fish.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/src/main/swap.c.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/utils.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/tournament.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/src/main/evl.c.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/btl.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/src/main/kar.c.o(.rodata.*)
        . = ALIGN(., 4);
        build/src/main/efe_table.c.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/sound_async.s.o(.rodata.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libpress.rodata.s.o(.rodata.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libds.s.o(.rodata.*))
        . = ALIGN(., 4);
        build/src/main/butterfly.c.o(.rodata.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libmcrd.rodata.s.o(.rodata.*))
        . = ALIGN(., 4);
        build/src/main/bubble.c.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/data/battle_ui.rodata.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/data/main.rodata.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/data/vs.rodata.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/data/script.rodata.s.o(.rodata.*)
        . = ALIGN(., 4);
        build/asm/main/data/main_menu.rodata.s.o(.rodata.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libapi.s.o(.rodata.*))
        . = ALIGN(., 4);
        main_RODATA_END = .;
        main_RODATA_SIZE = ABSOLUTE(main_RODATA_END - main_RODATA_START);

        main_DATA_START = .;
        KEEP(build/asm/main/data/psyq/libapi.data.s.o(.data.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libc.data.s.o(.data.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libetc.data.s.o(.data.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libgte.data.s.o(.data.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libetc2.data.s.o(.data.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libgpu.data.s.o(.data.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libgs.s.o(.data.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libgte2.data.s.o(.data.*))
        . = ALIGN(., 4);
        build/asm/main/data/model.data.s.o(.data.*)
        . = ALIGN(., 4);
        build/src/main/world_object.c.o(.data.*)
        . = ALIGN(., 4);
        build/src/main/file.c.o(.data.*)
        . = ALIGN(., 4);
        build/src/main/math.c.o(.data.*)
        . = ALIGN(., 4);
        build/src/main/file_table.c.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/data/graphics.data.s.o(.data.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/entrypoint.s.o(.data.*))
        . = ALIGN(., 4);
        build/asm/main/data/partner_impl.data.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/data/tamer.data.s.o(.data.*)
        . = ALIGN(., 4);
        build/src/main/fade.c.o(.data.*)
        . = ALIGN(., 4);
        build/src/main/aabb.c.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/efe.s.o(.data.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libcd.data.s.o(.data.*))
        . = ALIGN(., 4);
        build/asm/main/data/map_impl.data.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/game_menu.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/map_collision.s.o(.data.*)
        . = ALIGN(., 4);
        build/src/main/particle.c.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/anim.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/ui.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/item.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/sound.s.o(.data.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libspu.data.s.o(.data.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libsnd.data.s.o(.data.*))
        . = ALIGN(., 4);
        build/asm/main/data/map.data.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/data/evolution.data.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/data/utils.data.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/inventory.s.o(.data.*)
        . = ALIGN(., 4);
        build/src/main/clock.c.o(.data.*)
        . = ALIGN(., 4);
        build/src/main/partner.c.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/entity_text.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/tmd_object.s.o(.data.*)
        . = ALIGN(., 4);
        build/src/main/evolution.c.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/file_queue.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/graphics2.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/fish.s.o(.data.*)
        . = ALIGN(., 4);
        build/src/main/swap.c.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/utils.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/tournament.s.o(.data.*)
        . = ALIGN(., 4);
        build/src/main/evl.c.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/btl.s.o(.data.*)
        . = ALIGN(., 4);
        build/src/main/kar.c.o(.data.*)
        . = ALIGN(., 4);
        build/src/main/efe_table.c.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/data/9BB74.data.s.o(.data.*);
        . = ALIGN(., 4);
        build/asm/main/sound_async.s.o(.data.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libpress.data.s.o(.data.*))
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libds.s.o(.data.*))
        . = ALIGN(., 4);
        build/src/main/butterfly.c.o(.data.*)
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libmcrd.data.s.o(.data.*))
        . = ALIGN(., 4);
        build/asm/main/data/bubble.data.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/battle_ui.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/data/main.data.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/vs.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/script.s.o(.data.*)
        . = ALIGN(., 4);
        build/asm/main/main_menu.s.o(.data.*)
        . = ALIGN(., 4);
        main_DATA_END = .;
        main_DATA_SIZE = ABSOLUTE(main_DATA_END - main_DATA_START);

        main_SDATA_START = .;
        build/asm/main/data/A3B2C.sdata.s.o(.sdata.*)
        . = ALIGN(., 4);
        build/src/main/file_table.c.o(.sdata.*)
        . = ALIGN(., 4);
        build/asm/main/data/A41EC.sdata.s.o(.sdata.*)
        . = ALIGN(., 4);
        build/asm/main/data/map_impl.sdata.s.o(.sdata.*)
        . = ALIGN(., 4);
        build/asm/main/data/A4250.sdata.s.o(.sdata.*)
        . = ALIGN(., 4);
        build/src/main/efe_table.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/asm/main/data/A443C.sdata.s.o(.sdata.*);
        . = ALIGN(., 4);
        main_SDATA_END = .;
        main_SDATA_SIZE = ABSOLUTE(main_SDATA_END - main_SDATA_START);

        main_SBSS_START = .;
        . = ALIGN(., 0x800);
    }
    __romPos += SIZEOF(.main);
    __romPos = ALIGN(__romPos, 4);
    main_ROM_END = __romPos;
    main_ROM_SIZE = ABSOLUTE(main_ROM_END - main_ROM_START);
    . = ALIGN(., 4);

    sbss_ROM_START = __romPos;
    .sbss main_SBSS_START : AT(sbss_ROM_START)
    {
        build/build/generated/unk_0x80134C4C.sbss.s.o(.sbss.*)
        . = ALIGN(., 4);
        build/src/main/fade.c.o(.sbss.*)
        . = ALIGN(., 4);
        build/build/generated/unk_0x80134CC0.sbss.s.o(.sbss.*)
        . = ALIGN(., 4);
        main_SBSS_END = .;
        main_SBSS_SIZE = ABSOLUTE(main_SBSS_END - main_SBSS_START);
        main_BSS_START = .;
    }
    __romPos += SIZEOF(.sbss);
    __romPos = ALIGN(__romPos, 4);
    . = ALIGN(., 4);

    bss_ROM_START = __romPos;
    .bss main_BSS_START : AT(bss_ROM_START)
    {
        build/build/generated/unk_0x801353F8.bss.s.o(.bss.*)
        . = ALIGN(., 4);
        build/build/generated/libgs.bss.s.o(.bss.*)
        . = ALIGN(., 4);
        build/build/generated/libgte.bss.s.o(.bss.*)
        . = ALIGN(., 4);
        build/build/generated/model.bss.s.o(.bss.*)
        . = ALIGN(., 4);
        build/src/main/world_object.c.o(.bss.*)
        . = ALIGN(., 4);
        build/build/generated/unk_0x80137A24.bss.s.o(.bss.*)
        . = ALIGN(., 4);
        build/src/main/butterfly.c.o(.bss.*)
        . = ALIGN(., 4);
        build/build/generated/libmrcd.bss.s.o(.bss.*)
        . = ALIGN(., 4);
        build/build/generated/bubble.bss.s.o(.bss.*)
        . = ALIGN(., 4);
        build/build/generated/battle_ui.bss.s.o(.bss.*)
        . = ALIGN(., 4);
        build/build/generated/unk_0x801555D0.bss.s.o(.bss.*)
        . = ALIGN(., 4);
        main_BSS_END = .;
        main_BSS_SIZE = ABSOLUTE(main_BSS_END - main_BSS_START);
        main_MEMFILL_SIZE = ABSOLUTE(main_BSS_END - main_SBSS_START);
        main_MEMFILL_ALIGNED_SIZE = ABSOLUTE(((main_MEMFILL_SIZE + 0X7FF) / 0x800) * 0x800);
    }
    __romPos += SIZEOF(.bss);
    __romPos = ALIGN(__romPos, 4);
    . = ALIGN(., 4);

    /* Hack to export symbols */
#include "symbols_main.txt"

    /DISCARD/ :
    {
        *(*);
    }
}
