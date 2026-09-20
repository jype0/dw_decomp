ENTRY(__start)

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
        KEEP(build/asm/main/psyq/libapi.s.o(.text.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libc.s.o(.text.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libetc.s.o(.text.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libgpu.s.o(.text.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libgs.s.o(.text.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libgte.s.o(.text.*));
        . = ALIGN(., 4);
        build/src/main/model.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/world_object.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/file.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/math.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/graphics.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/_psstart.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/partner_impl.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/tamer.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/fade.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/aabb.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/efe.c.o(.text.*);
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libcd.s.o(.text.*));
        . = ALIGN(., 4);
        build/src/main/overworld.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/overworld_evochart_detail.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/overworld_menu_views.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/overworld_evochart_view.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/overworld_status_boxes.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/overworld_moves_text.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/overworld_moves_box.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/overworld_playerinfo_text.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/overworld_evochart_text.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/overworld_medal_text.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/overworld_card_text.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/map_collision.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/particle.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/anim.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/ui.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/item.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/sound.c.o(.text.*);
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libspu.s.o(.text.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libsnd.s.o(.text.*));
        . = ALIGN(., 4);
        build/src/main/map.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/inventory.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/clock.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/partner.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/entity_text.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/map_object.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/evolution.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/file_queue.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/graphics2.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/fish.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/utils.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/utils2.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/tournament.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/evl.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/btl.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/kar.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/efe_table.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/sound_async.c.o(.text.*);
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libpress.s.o(.text.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libds.s.o(.text.*));
        . = ALIGN(., 4);
        build/src/main/butterfly.c.o(.text.*);
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libmcrd.s.o(.text.*));
        . = ALIGN(., 4);
        build/src/main/bubble.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/battle_ui.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/main.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/sjis.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/vs.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/script_common.c.o(.text.*);
        . = ALIGN(., 4);
        /*
         * Four leaf helpers must remain in a separate TU to preserve the
         * retail callers' register allocation.  The other script functions
         * share one TU, so select its contiguous pre-helper range before
         * inserting the helper object; the remaining wildcard is script_ops.
         */
        build/src/main/script_ops.c.o(".text.scriptInstruction10to27");
        build/src/main/script_ops.c.o(".text.scriptInstruction28to3F");
        build/src/main/script_ops.c.o(".text.scriptInstruction46to58");
        build/src/main/script_ops.c.o(".text.scriptInstruction5Ato5F");
        build/src/main/script_ops.c.o(".text.scriptInstruction64to7E");
        build/src/main/script_ops.c.o(".text.setMapHeadActive");
        build/src/main/script_ops.c.o(".text.returnFromScriptFile");
        build/src/main/script_ops.c.o(".text.MAIN_func_801050C0");
        build/src/main/script_ops.c.o(".text.scriptUpdateEnergyBoundaries");
        build/src/main/script_ops.c.o(".text.MAIN_func_801053EC");
        build/src/main/script_ops.c.o(".text.MAIN_func_80105464");
        build/src/main/script_ops.c.o(".text.scriptStartAnimation");
        build/src/main/script_ops.c.o(".text.handleMusicOverride");
        build/src/main/script_ops.c.o(".text.tickScriptedMovement");
        build/src/main/script_ops.c.o(".text.initializeScripts");
        build/src/main/script_ops.c.o(".text.initializeLoadedNPCModels");
        build/src/main/script_ops.c.o(".text.runMapHeadScript");
        build/src/main/script_ops.c.o(".text.callScriptSection");
        build/src/main/script_ops.c.o(".text.tickScript");
        build/src/main/script_ops.c.o(".text.getScript");
        build/src/main/script_ops.c.o(".text.getScriptSection");
        . = ALIGN(., 4);
        build/src/main/script_helpers.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/script_ops.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/script_draw.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/font.c.o(.text.*);
        . = ALIGN(., 4);
        build/src/main/main_menu.c.o(.text.*);
        . = ALIGN(., 16);
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
        LONG(btl_TEXT_START)
        LONG(std_TEXT_START)
        LONG(fish_TEXT_START)
        LONG(evl_TEXT_START)
        LONG(kar_TEXT_START)
        LONG(vs_TEXT_START)
        LONG(mov_TEXT_START)
        LONG(doo2_TEXT_START)
        LONG(dooa_TEXT_START)
        LONG(trn_TEXT_START)
        LONG(dget_TEXT_START)
        LONG(trn2_TEXT_START)
        LONG(murd_TEXT_START)
        LONG(endi_TEXT_START)
        LONG(eab_TEXT_START)

        . = ALIGN(., 4);
        build/src/main/partner_impl.c.o(.rodata.*);
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libc.rodata.s.o(.rodata.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libetc.rodata.s.o(.rodata.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libgpu.rodata.s.o(.rodata.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libgs.rodata.s.o(.rodata.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libgte.s.o(.rodata.*));
        . = ALIGN(., 4);
        build/src/main/model.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/world_object.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/file.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/math.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/graphics.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/_psstart.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/partner_impl.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/tamer.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/fade.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/aabb.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/efe.c.o(.rodata.*);
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libcd.rodata.s.o(.rodata.*));
        . = ALIGN(., 4);
        build/src/main/overworld.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/overworld_menu_views.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/overworld_moves_box.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/overworld_evochart_view.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/overworld_status_boxes.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/overworld_evochart_detail.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/overworld_moves_text.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/overworld_playerinfo_text.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/overworld_evochart_text.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/overworld_medal_text.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/overworld_card_text.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/map_collision.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/particle.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/anim.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/ui.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/item.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/sound.c.o(.rodata.*);
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libspu.rodata.s.o(.rodata.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libsnd.rodata.s.o(.rodata.*));
        . = ALIGN(., 4);
        build/src/main/map.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/inventory.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/clock.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/partner.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/entity_text.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/map_object.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/evolution.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/file_queue.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/graphics2.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/fish.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/utils.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/utils2.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/tournament.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/evl.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/btl.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/kar.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/efe_table.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/sound_async.c.o(.rodata.*);
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libpress.rodata.s.o(.rodata.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libds.s.o(.rodata.*));
        . = ALIGN(., 4);
        build/src/main/butterfly.c.o(.rodata.*);
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libmcrd.rodata.s.o(.rodata.*));
        . = ALIGN(., 4);
        build/src/main/bubble.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/battle_ui.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/main.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/sjis.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/vs.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/script_common.c.o(.rodata.*);
        . = ALIGN(., 4);
        /*
         * MAIN_func_80106730 must be defined after scriptInstruction28to3F
         * or Metrowerks sees the leaf and changes the caller's frame by four
         * bytes.  Retail rodata orders the two comparison tables first, so
         * restore the table order explicitly within their common source TU.
         */
        build/src/main/script_ops.c.o(".rodata.@172");
        build/src/main/script_ops.c.o(".rodata.@245");
        build/src/main/script_ops.c.o(".rodata.@316");
        build/src/main/script_ops.c.o(".rodata.@492");
        build/src/main/script_ops.c.o(".rodata.@501");
        build/src/main/script_ops.c.o(".rodata.@392");
        build/src/main/script_ops.c.o(".rodata.@459");
        build/src/main/script_ops.c.o(".rodata.@468");
        build/src/main/script_ops.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/script_draw.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/font.c.o(.rodata.*);
        . = ALIGN(., 4);
        build/src/main/main_menu.c.o(.rodata.* .rodata);
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libapi.s.o(.rodata.*));
        . = ALIGN(., 4);
        main_RODATA_END = .;
        main_RODATA_SIZE = ABSOLUTE(main_RODATA_END - main_RODATA_START);

        main_DATA_START = .;
        KEEP(build/asm/main/data/psyq/libapi.data.s.o(.data.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libc.data.s.o(.data.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libetc.data.s.o(.data.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libgte.data.s.o(.data.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libetc2.data.s.o(.data.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libgpu.data.s.o(.data.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libgs.s.o(.data.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libgte2.data.s.o(.data.*));
        . = ALIGN(., 4);
        build/src/main/model.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/world_object.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/file.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/math.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/file_table.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/graphics.c.o(.data.*);
        . = ALIGN(., 4);
        KEEP(build/src/main/_psstart.c.o(.data.*));
        . = ALIGN(., 4);
        build/src/main/partner_impl.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/tamer.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/fade.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/aabb.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/toilet_data.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/door_mapdata.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/efe.c.o(.data.*);
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libcd.data.s.o(.data.*));
        . = ALIGN(., 4);
        build/src/main/overworld.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/overworld_evochart_detail.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/overworld_menu_views.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/overworld_evochart_view.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/overworld_status_boxes.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/overworld_moves_text.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/overworld_moves_box.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/overworld_playerinfo_text.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/overworld_evochart_text.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/overworld_medal_text.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/overworld_card_text.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/map_collision.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/particle.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/anim.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/ui.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/item.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/sound.c.o(.data.*);
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libspu.data.s.o(.data.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libsnd.data.s.o(.data.*));
        . = ALIGN(., 4);
        build/src/main/map.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/inventory.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/clock.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/partner.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/entity_text.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/map_object.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/evolution.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/file_queue.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/graphics2.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/fish.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/utils.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/utils2.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/tournament.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/evl.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/btl.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/kar.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/efe_table.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/sound_async.c.o(.data.*);
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libpress.data.s.o(.data.*));
        . = ALIGN(., 4);
        KEEP(build/asm/main/psyq/libds.s.o(.data.*));
        . = ALIGN(., 4);
        build/src/main/butterfly.c.o(.data.*);
        . = ALIGN(., 4);
        KEEP(build/asm/main/data/psyq/libmcrd.data.s.o(.data.*));
        . = ALIGN(., 4);
        build/src/main/bubble.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/battle_ui.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/main.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/sjis.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/vs.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/script_common.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/font.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/main_menu.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/sjis.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/vs.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/script_common.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/font.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/main_menu.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/vs.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/script_common.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/script_ops.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/script_helpers.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/script_draw.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/font.c.o(.data.*);
        . = ALIGN(., 4);
        build/src/main/main_menu.c.o(.data.*);
        . = ALIGN(., 4);
        main_DATA_END = .;
        main_DATA_SIZE = ABSOLUTE(main_DATA_END - main_DATA_START);

        main_SDATA_START = .;
        build/src/main/model.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/file_table.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/partner_impl.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/tamer.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/efe.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/overworld.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/ui.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/item.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/sound.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/map.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/inventory.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/clock.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/entity_text.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/map_object.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/file_queue.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/graphics2.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/utils.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/efe_table.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/sound_async.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/bubble.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/battle_ui.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/main.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/font.c.o(.sdata.*);
        . = ALIGN(., 4);
        build/src/main/main_menu.c.o(.sdata.*);
        . = ALIGN(., 4);
        KEEP(build/src/main/btl.c.o(.sdata.*));
        . = ALIGN(., 4);
        KEEP(build/src/main/std.c.o(.sdata.*));
        . = ALIGN(., 4);
        KEEP(build/src/main/fish.c.o(.sdata.*));
        . = ALIGN(., 4);
        KEEP(build/src/main/evl.c.o(.sdata.*));
        . = ALIGN(., 4);
        KEEP(build/src/main/kar.c.o(.sdata.*));
        . = ALIGN(., 4);
        KEEP(build/src/main/vs.c.o(.sdata.*));
        . = ALIGN(., 4);
        KEEP(build/src/main/doo2.c.o(.sdata.*));
        . = ALIGN(., 4);
        KEEP(build/src/main/dooa.c.o(.sdata.*));
        . = ALIGN(., 4);
        KEEP(build/src/main/trn.c.o(.sdata.*));
        . = ALIGN(., 4);
        KEEP(build/src/main/trn2.c.o(.sdata.*));
        . = ALIGN(., 4);
        KEEP(build/src/main/murd.c.o(.sdata.*));
        . = ALIGN(., 4);
        KEEP(build/src/main/endi.c.o(.sdata.*));
        . = ALIGN(., 4);
        KEEP(build/src/main/eab.c.o(.sdata.*));
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
        build/build/generated/unk_0x80134C4C.sbss.s.o(.sbss.*);
        . = ALIGN(., 4);
        build/src/main/fade.c.o(.sbss.*);
        . = ALIGN(., 4);
        build/build/generated/unk_0x80134CC0.sbss.s.o(.sbss.*);
        . = ALIGN(., 4);
        build/src/main/sound.c.o(.sbss.*);
        . = ALIGN(., 4);
        build/build/generated/unk_0x80134D9C.sbss.s.o(.sbss.*);
        . = ALIGN(., 4);
        build/src/main/inventory.c.o(.sbss.*);
        . = ALIGN(., 4);
        build/build/generated/unk_0x80134E1C.sbss.s.o(.sbss.*);
        . = ALIGN(., 4);
        KEEP(build/src/main/map_object.c.o(.sbss.*));
        . = ALIGN(., 4);
        build/build/generated/unk_0x80134E50.sbss.s.o(.sbss.*);
        . = ALIGN(., 4);
        build/src/main/file_queue.c.o(.sbss.*);
        . = ALIGN(., 4);
        build/build/generated/unk_0x80134E68.sbss.s.o(.sbss.*);
        . = ALIGN(., 4);
        build/src/main/sound_async.c.o(.sbss.*);
        . = ALIGN(., 4);
        build/build/generated/unk_0x80134E90.sbss.s.o(.sbss.*);
        . = ALIGN(., 4);
        build/src/main/font.c.o(.sbss.*);
        . = ALIGN(., 4);
        build/src/main/main_menu.c.o(.sbss.*);
        . = ALIGN(., 4);
        build/build/generated/unk_0x80135078.sbss.s.o(.sbss.*);
        . = ALIGN(., 4);
        build/src/main/mov.c.o(.sbss.*);
        . = ALIGN(., 4);
        build/src/main/doo2.c.o(.sbss.*);
        . = ALIGN(., 4);
        build/build/generated/unk_0x80135324.sbss.s.o(.sbss.*);
        . = ALIGN(., 4);
        build/src/main/trn.c.o(.sbss.*);
        . = ALIGN(., 4);
        build/build/generated/unk_0x801353A8.sbss.s.o(.sbss.*);
        . = ALIGN(., 4);
        build/src/main/trn2.c.o(.sbss.*);
        . = ALIGN(., 4);
        build/src/main/murd.c.o(.sbss.*);
        . = ALIGN(., 4);
        build/build/generated/unk_0x801353F0.sbss.s.o(.sbss.*);
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
        build/build/generated/unk_0x801353F8.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        build/build/generated/libgs.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        build/build/generated/libgte.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        build/build/generated/model.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        build/src/main/world_object.c.o(.bss.*);
        . = ALIGN(., 4);
        KEEP(build/build/generated/unk_0x80137A24.bss.s.o(.bss.*));
        . = ALIGN(., 4);
        KEEP(build/src/main/item.c.o(.bss.*));
        . = ALIGN(., 4);
        KEEP(build/build/generated/unk_0x8013D590.bss.s.o(.bss.*));
        . = ALIGN(., 4);
        build/src/main/sound.c.o(.bss.*);
        . = ALIGN(., 4);
        KEEP(build/build/generated/unk_0x8014DBEC.bss.s.o(.bss.*));
        . = ALIGN(., 4);
        build/src/main/map_object.c.o(.bss.*);
        . = ALIGN(., 4);
        build/src/main/file_queue.c.o(.bss.*);
        . = ALIGN(., 4);
        build/build/generated/unk_0x80154F80.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        build/src/main/butterfly.c.o(.bss.*);
        . = ALIGN(., 4);
        build/build/generated/libmrcd.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        build/build/generated/bubble.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        build/build/generated/battle_ui.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        build/build/generated/unk_0x801555D0.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        build/src/main/font.c.o(.bss.*);
        . = ALIGN(., 4);
        build/build/generated/unk_0x801BF768.bss.s.o(.bss.*);
        . = ALIGN(., 4);
        build/build/generated/unk_0x801BF768.bss.s.o(.bss.*);
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
