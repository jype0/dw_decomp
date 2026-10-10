ENTRY(__start)

SECTIONS
{
    __romPos = 0;
    _gp = main_SDATA_START + 0x8000;
    header_ROM_START = __romPos;
    .header 0x80090000 : AT(header_ROM_START)
    {
        KEEP(BUILD_DIR/asm/main/data/header.data.s.o(.data .data.*));
        . = ALIGN(., 4);
    }
    __romPos += SIZEOF(.header);
    __romPos = ALIGN(__romPos, 4);
    . = ALIGN(., 4);

    main_ROM_START = __romPos;
    .main : AT(main_ROM_START)
    {
        main_TEXT_START = .;
        BUILD_DIR/asm/main/psyq/libapi.s.o(.text .text.*);
        BUILD_DIR/asm/main/psyq/libc.s.o(.text .text.*);
        BUILD_DIR/asm/main/psyq/libetc.s.o(.text .text.*);
        BUILD_DIR/asm/main/psyq/libgpu.s.o(.text .text.*);
        BUILD_DIR/asm/main/psyq/libgs.s.o(.text .text.*);
        BUILD_DIR/asm/main/psyq/libgte.s.o(.text .text.*);
        BUILD_DIR/src/main/model.c.o(.text .text.*);
        BUILD_DIR/src/main/world_object.c.o(.text .text.*);
        BUILD_DIR/src/main/file.c.o(.text .text.*);
        BUILD_DIR/src/main/math.c.o(.text .text.*);
        BUILD_DIR/src/main/graphics.c.o(.text .text.*);
        BUILD_DIR/src/main/font.c.o(.text .text.*);
        BUILD_DIR/src/main/drop_shadow.c.o(.text .text.*);
        BUILD_DIR/src/main/_psstart.c.o(.text .text.*);
        BUILD_DIR/src/main/partner_impl.c.o(.text .text.*);
        BUILD_DIR/src/main/tamer.c.o(.text .text.*);
        BUILD_DIR/src/main/fade.c.o(.text .text.*);
        BUILD_DIR/src/main/aabb.c.o(.text .text.*);
        BUILD_DIR/src/main/efe.c.o(.text .text.*);
        BUILD_DIR/asm/main/psyq/libcd.s.o(.text .text.*);
        BUILD_DIR/src/main/overworld.c.o(.text .text.*);
        BUILD_DIR/src/main/game_menu.c.o(.text .text.*);
        BUILD_DIR/src/main/map_collision.c.o(.text .text.*);
        BUILD_DIR/src/main/particle.c.o(.text .text.*);
        BUILD_DIR/src/main/anim.c.o(.text .text.*);
        BUILD_DIR/src/main/ui.c.o(.text .text.*);
        BUILD_DIR/src/main/item.c.o(.text .text.*);
        BUILD_DIR/src/main/sound.c.o(.text .text.*);
        BUILD_DIR/asm/main/psyq/libspu.s.o(.text .text.*);
        BUILD_DIR/asm/main/psyq/libsnd.s.o(.text .text.*);
        BUILD_DIR/src/main/map.c.o(.text .text.*);
        BUILD_DIR/src/main/effect_common.c.o(.text .text.*);
        BUILD_DIR/src/main/inventory.c.o(.text .text.*);
        BUILD_DIR/src/main/clock.c.o(.text .text.*);
        BUILD_DIR/src/main/partner.c.o(.text .text.*);
        BUILD_DIR/src/main/script_interp.c.o(.text .text.*);
        BUILD_DIR/src/main/entity_text.c.o(.text .text.*);
        BUILD_DIR/src/main/script_textbox.c.o(.text .text.*);
        BUILD_DIR/src/main/script_value.c.o(.text .text.*);
        BUILD_DIR/src/main/map_object.c.o(.text .text.*);
        BUILD_DIR/src/main/script_common.c.o(.text .text.*);
        BUILD_DIR/src/main/evolution.c.o(.text .text.*);
        BUILD_DIR/src/main/file_queue.c.o(.text .text.*);
        BUILD_DIR/src/main/line.c.o(.text .text.*);
        BUILD_DIR/src/main/vecmath.c.o(.text .text.*);
        BUILD_DIR/src/main/rng.c.o(.text .text.*);
        BUILD_DIR/src/main/fish.c.o(.text .text.*);
        BUILD_DIR/src/main/utils.c.o(.text .text.*);
        BUILD_DIR/src/main/utils2.c.o(.text .text.*);
        BUILD_DIR/src/main/tournament.c.o(.text .text.*);
        BUILD_DIR/src/main/evl.c.o(.text .text.*);
        BUILD_DIR/src/main/btl.c.o(.text .text.*);
        BUILD_DIR/src/main/battle_tick.c.o(.text .text.*);
        BUILD_DIR/src/main/kar.c.o(.text .text.*);
        BUILD_DIR/src/main/efe_table.c.o(.text .text.*);
        BUILD_DIR/src/main/sound_async.c.o(.text .text.*);
        BUILD_DIR/asm/main/psyq/libpress.s.o(.text .text.*);
        BUILD_DIR/asm/main/psyq/libds.s.o(.text .text.*);
        BUILD_DIR/src/main/butterfly.c.o(.text .text.*);
        BUILD_DIR/src/main/main_menu.c.o(.text .text.*);
        BUILD_DIR/asm/main/psyq/libmcrd.s.o(.text .text.*);
        BUILD_DIR/src/main/bubble.c.o(.text .text.*);
        BUILD_DIR/src/main/battle_ui.c.o(.text .text.*);
        BUILD_DIR/src/main/main.c.o(.text .text.*);
        BUILD_DIR/src/main/new_game.c.o(.text .text.*);
        BUILD_DIR/src/main/buff_model.c.o(.text .text.*);
        main_TEXT_END = .;
        main_TEXT_SIZE = ABSOLUTE(main_TEXT_END - main_TEXT_START);

        main_RODATA_START = .;

        __static_init = .;
        LONG(0x0)
        _gp_addr = .;
        LONG(_gp)
        _sbss_size = .;
        LONG(main_SBSS_SIZE)
        _bss_size = .;
        LONG(main_BSS_SIZE)
        _sdata_size = .;
        LONG(main_SDATA_SIZE)
        _data_size = .;
        LONG(main_DATA_SIZE)
        _rdata_size = .;
        LONG(main_RODATA_SIZE)
        _fsbss = .;
        LONG(main_SBSS_START)
        _fbss = .;
        LONG(main_BSS_START)
        _fsdata = .;
        LONG(main_SDATA_START)
        _fdata = .;
        LONG(main_DATA_START)
        _frdata = .;
        LONG(main_RODATA_START)
        _start_etbl = .;
        LONG(main_TEXT_START)
        _nbfunctions = .;
        LONG(main_TEXT_START)
        _ftext = .;
        LONG(main_TEXT_START)
        _stack_addr = .;
        LONG(0x801FFF00)
        _nbtvectors = .;
        LONG(0x0)
        _stack_size = .;
        LONG(0x40)
        _heap_size = .;
        LONG(0x400)
        _etext = .;
        LONG(main_TEXT_END)
        _edata = .;
        LONG(main_SDATA_END)
        _end = .;
        LONG(main_BSS_END)
        _fsdata_ROM = .;
        LONG(main_SDATA_START)
        _fdata_ROM = .;
        LONG(main_DATA_START)
        _sdata_ROM_size = .;
        LONG(main_SDATA_SIZE)
        _data_ROM_size = .;
        LONG(main_DATA_SIZE)
        _scratchpad_size = .;
        LONG(0x0)

        _overlay_section_addresses = .;
        LONG(main_TEXT_START)
        LONG(btl_HEADER)
        LONG(std_HEADER)
        LONG(fish_HEADER)
        LONG(evl_HEADER)
        LONG(kar_HEADER)
        LONG(vs_HEADER)
        LONG(mov_HEADER)
        LONG(doo2_HEADER)
        LONG(dooa_HEADER)
        LONG(trn_HEADER)
        LONG(shop_HEADER)
        LONG(dget_HEADER)
        LONG(trn2_HEADER)
        LONG(murd_HEADER)
        LONG(endi_HEADER)
        LONG(eab_HEADER)

        BUILD_DIR/asm/main/data/psyq/libc.rodata.s.o(.rodata .rodata.*);
        BUILD_DIR/asm/main/data/psyq/libetc.rodata.s.o(.rodata .rodata.*);
        BUILD_DIR/asm/main/data/psyq/libgpu.rodata.s.o(.rodata .rodata.*);
        BUILD_DIR/asm/main/data/psyq/libgs.rodata.s.o(.rodata .rodata.*);
        BUILD_DIR/src/main/tamer.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/efe.c.o(.rodata .rodata.*);
        BUILD_DIR/asm/main/data/psyq/libcd.rodata.s.o(.rodata .rodata.*);
        BUILD_DIR/src/main/overworld.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/game_menu.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/item.c.o(.rodata .rodata.*);
        BUILD_DIR/asm/main/data/psyq/libspu.rodata.s.o(.rodata .rodata.*);
        BUILD_DIR/asm/main/data/psyq/libsnd.rodata.s.o(.rodata .rodata.*);
        BUILD_DIR/src/main/map.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/inventory.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/partner.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/script_interp.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/script_textbox.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/script_value.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/map_object.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/script_common.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/line.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/vecmath.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/rng.c.o(.rodata .rodata.*);
        BUILD_DIR/asm/main/data/psyq/libpress.rodata.s.o(.rodata .rodata.*);
        BUILD_DIR/src/main/main_menu.c.o(.rodata .rodata.*);
        BUILD_DIR/asm/main/data/psyq/libmcrd.rodata.s.o(.rodata .rodata.*);
        BUILD_DIR/src/main/battle_ui.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/main.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/bubble.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/model.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/world_object.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/file.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/math.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/file_table_jp.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/graphics.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/font.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/drop_shadow.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/_psstart.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/fade.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/aabb.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/map_collision.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/particle.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/anim.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/ui.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/sound.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/clock.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/entity_text.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/evolution.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/file_queue.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/fish.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/utils.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/utils2.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/tournament.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/evl.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/btl.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/battle_tick.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/kar.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/efe_table.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/sound_async.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/butterfly.c.o(.rodata .rodata.*);
        main_RODATA_END = .;
        main_RODATA_SIZE = ABSOLUTE(main_RODATA_END - main_RODATA_START);

        main_DATA_START = .;
        BUILD_DIR/asm/main/data/psyq/libapi.data.s.o(.data .data.*);
        BUILD_DIR/asm/main/data/psyq/libc.data.s.o(.data .data.*);
        BUILD_DIR/asm/main/data/psyq/libetc.data.s.o(.data .data.*);
        BUILD_DIR/asm/main/data/psyq/libgpu.data.s.o(.data .data.*);
        BUILD_DIR/asm/main/data/psyq/libgte.data.s.o(.data .data.*);
        BUILD_DIR/src/main/model.c.o(.data .data.*);
        BUILD_DIR/src/main/file_table_jp.c.o(.data .data.*);
        BUILD_DIR/src/main/graphics.c.o(.data .data.*);
        BUILD_DIR/src/main/font.c.o(.data .data.*);
        BUILD_DIR/src/main/partner_impl.c.o(.data .data.*);
        BUILD_DIR/src/main/tamer.c.o(.data .data.*);
        BUILD_DIR/src/main/toilet_data.c.o(.data .data.*);
        BUILD_DIR/src/main/door_mapdata.c.o(.data .data.*);
        BUILD_DIR/src/main/efe.c.o(.data .data.*);
        BUILD_DIR/asm/main/data/psyq/libcd.data.s.o(.data .data.*);
        BUILD_DIR/src/main/game_menu.c.o(.data .data.*);
        BUILD_DIR/src/main/particle.c.o(.data .data.*);
        BUILD_DIR/src/main/ui.c.o(.data .data.*);
        BUILD_DIR/src/main/item.c.o(.data .data.*);
        BUILD_DIR/src/main/sound.c.o(.data .data.*);
        BUILD_DIR/asm/main/data/psyq/libspu.data.s.o(.data .data.*);
        BUILD_DIR/asm/main/data/psyq/libsnd.data.s.o(.data .data.*);
        BUILD_DIR/src/main/map.c.o(.data .data.*);
        BUILD_DIR/src/main/effect_common.c.o(.data .data.*);
        BUILD_DIR/src/main/inventory.c.o(.data .data.*);
        BUILD_DIR/src/main/clock.c.o(.data .data.*);
        BUILD_DIR/src/main/script_interp.c.o(.data .data.*);
        BUILD_DIR/src/main/entity_text.c.o(.data .data.*);
        BUILD_DIR/src/main/script_value.c.o(.data .data.*);
        BUILD_DIR/src/main/map_object.c.o(.data .data.*);
        BUILD_DIR/src/main/script_common.c.o(.data .data.*);
        BUILD_DIR/src/main/evolution.c.o(.data .data.*);
        BUILD_DIR/src/main/utils.c.o(.data .data.*);
        BUILD_DIR/src/main/utils2.c.o(.data .data.*);
        BUILD_DIR/src/main/tournament.c.o(.data .data.*);
        BUILD_DIR/src/main/btl.c.o(.data .data.*);
        BUILD_DIR/src/main/efe_table.c.o(.data .data.*);
        BUILD_DIR/src/main/sound_async.c.o(.data .data.*);
        BUILD_DIR/asm/main/data/psyq/libpress.data.s.o(.data .data.*);
        BUILD_DIR/src/main/butterfly.c.o(.data .data.*);
        BUILD_DIR/src/main/main_menu.c.o(.data .data.*);
        BUILD_DIR/asm/main/data/psyq/libmcrd.data.s.o(.data .data.*);
        BUILD_DIR/src/main/bubble.c.o(.data .data.*);
        BUILD_DIR/src/main/main.c.o(.data .data.*);
        main_DATA_END = .;
        main_DATA_SIZE = ABSOLUTE(main_DATA_END - main_DATA_START);

        main_SDATA_START = .;
        BUILD_DIR/src/main/model.c.o(.sdata .sdata.*);
        BUILD_DIR/src/main/file_table_jp.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/font.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/partner_impl.c.o(.sdata .sdata.*);
        BUILD_DIR/src/main/tamer.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/efe.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/overworld.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/game_menu.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/ui.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/item.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/sound.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/map.c.o(.sdata .sdata.*);
        BUILD_DIR/src/main/inventory.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/clock.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/script_interp.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/entity_text.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/script_value.c.o(.sdata .sdata.*);
        BUILD_DIR/src/main/map_object.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/script_common.c.o(.sdata .sdata.*);
        BUILD_DIR/src/main/file_queue.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/rng.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/utils2.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/efe_table.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/sound_async.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/main_menu.c.o(.sdata .sdata.*);
        BUILD_DIR/src/main/bubble.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/battle_ui.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/main.c.o(.sdata .sdata.*);
        BUILD_DIR/src/btl/battle_setup.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/btl/battle_main.c.o(.sdata .sdata.*);
        BUILD_DIR/src/btl/command_shout.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/btl/command_menu.c.o(.sdata .sdata.*);
        BUILD_DIR/src/btl/battle_hud.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/btl/battle_effect.c.o(.sdata .sdata.*);
        BUILD_DIR/src/std/std_setup.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/std/std_main.c.o(.sdata .sdata.*);
        BUILD_DIR/src/std/std_hud.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/std/std_effect.c.o(.sdata .sdata.*);
        BUILD_DIR/src/fish/fish.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/evl/evl.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/kar/kar.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/vs/vs_select.c.o(.sdata .sdata.*);
        BUILD_DIR/src/vs/vs_scene.c.o(.sdata .sdata.*);
        BUILD_DIR/src/vs/vs_intro.c.o(.sdata .sdata.*);
        BUILD_DIR/src/vs/vs_camera.c.o(.sdata .sdata.*);
        BUILD_DIR/src/vs/vs_combat.c.o(.sdata .sdata.*);
        BUILD_DIR/src/vs/vs_main.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/vs/vs_hud.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/vs/vs_effect.c.o(.sdata .sdata.*);
        BUILD_DIR/src/doo2/doo2.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/dooa/dooa.c.o(.sdata .sdata.*);
        BUILD_DIR/src/trn/trn_reward.c.o(.sdata .sdata.*);
        BUILD_DIR/src/trn/trn_hud.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/trn/trn_slots.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/shop/script_menu.c.o(.sdata .sdata.*);
        BUILD_DIR/src/trn2/trn2_hud.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/trn2/trn2_slots.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/murd/murd.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/endi/endi.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/eab/eab.c.o(.sdata .sdata.*);
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
        BUILD_DIR/src/main/font.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/partner_impl.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/tamer.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/fade.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/efe.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/overworld.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/game_menu.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/item.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/sound.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/map.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/effect_common.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/inventory.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/clock.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/partner.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/script_interp.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/script_textbox.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/map_object.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/script_common.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/evolution.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/file_queue.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/rng.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/fish.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/utils2.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/evl.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/btl.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/battle_tick.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/kar.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/sound_async.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/main_menu.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/battle_ui.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/main.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/new_game.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/buff_model.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/btl/battle_setup.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/btl/command_menu.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/btl/battle_hud.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/btl/battle_effect.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/std/std_setup.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/std/std_main.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/std/std_hud.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/std/std_effect.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/fish/fish.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/evl/evl.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/kar/kar.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/vs/vs_select.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/vs/vs_scene.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/vs/vs_intro.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/vs/vs_camera.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/vs/vs_combat.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/vs/vs_hud.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/vs/vs_effect.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/mov/mov.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/doo2/doo2.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/dooa/dooa.c.o(.sbss.*);
        BUILD_DIR/src/trn/trn_hp.c.o(.sbss.*);
        BUILD_DIR/src/trn/trn_mp.c.o(.sbss.*);
        BUILD_DIR/src/trn/trn_off.c.o(.sbss.*);
        BUILD_DIR/src/trn/trn_def.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/trn/trn_hud.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/dget/dget.c.o(.sbss.*);
        BUILD_DIR/src/trn2/trn2_hp_map99.c.o(.sbss.*);
        BUILD_DIR/src/trn2/trn2_def_map99.c.o(.sbss.*);
        BUILD_DIR/src/trn2/trn2_off.c.o(.sbss.*);
        BUILD_DIR/src/trn2/trn2_reward.c.o(.sbss.*);
        BUILD_DIR/src/trn2/trn2_hud.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/murd/murd.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/eab/eab.c.o(.sbss.*);
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
        BUILD_DIR/generated/libapi.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/libetc.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/libgpu.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/libgs.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/libgte.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/model.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/world_object.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/graphics.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/font.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/partner_impl.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/tamer.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/efe.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/libcd.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/overworld.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/particle.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/ui.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/item.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/sound.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/libspu.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/libsnd.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/map.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/clock.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/partner.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/script_interp.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/entity_text.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/map_object.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/script_common.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/file_queue.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/fish.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/evl.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/btl.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/efe_table.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/libds.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/butterfly.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/main_menu.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/libmcrd.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/bubble.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/battle_ui.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/main.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/new_game.c.o(.bss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/buff_model.c.o(.bss.*);
        . = ALIGN(., 4);
        main_BSS_END = .;
        main_BSS_SIZE = ABSOLUTE(main_BSS_END - main_BSS_START);
        main_MEMFILL_SIZE = ABSOLUTE(main_BSS_END - main_SBSS_START);
        main_MEMFILL_ALIGNED_SIZE = ABSOLUTE(((main_MEMFILL_SIZE + 0X7FF) / 0x800) * 0x800);
    }
    __romPos += SIZEOF(.bss);
    __romPos = ALIGN(__romPos, 4);

    /DISCARD/ :
    {
        *(*);
    }
}
