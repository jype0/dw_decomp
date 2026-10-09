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
        KEEP(BUILD_DIR/asm/main/main.s.o(.text .text.*));
        BUILD_DIR/src/main/file.c.o(.text .text.*);
        BUILD_DIR/src/main/math.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/137C4.s.o(.text .text.*));
        BUILD_DIR/src/main/fade.c.o(.text .text.*);
        BUILD_DIR/src/main/aabb.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/1FA64.s.o(.text .text.*));
        BUILD_DIR/src/main/line.c.o(.text .text.*);
        BUILD_DIR/src/main/vecmath.c.o(.text .text.*);
        BUILD_DIR/src/main/rng.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/70030.s.o(.text .text.*));
        BUILD_DIR/src/main/tournament.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/72F18.s.o(.text .text.*));
        BUILD_DIR/src/main/kar.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/74FD4.s.o(.text .text.*));
        BUILD_DIR/src/main/sound_async.c.o(.text .text.*);
        KEEP(BUILD_DIR/asm/main/75A80.s.o(.text .text.*));
        main_TEXT_END = .;
        main_TEXT_SIZE = ABSOLUTE(main_TEXT_END - main_TEXT_START);

        main_RODATA_START = .;
        KEEP(BUILD_DIR/asm/main/data/main.rodata.s.o(.rodata .rodata.*));
        BUILD_DIR/src/main/vecmath.c.o(.rodata .rodata.*);
        KEEP(BUILD_DIR/asm/main/data/8B5B4.rodata.s.o(.rodata .rodata.*));
        BUILD_DIR/src/main/aabb.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/door_mapdata.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/fade.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/file.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/kar.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/line.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/math.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/rng.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/sound_async.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/toilet_data.c.o(.rodata .rodata.*);
        BUILD_DIR/src/main/tournament.c.o(.rodata .rodata.*);
        main_RODATA_END = .;
        main_RODATA_SIZE = ABSOLUTE(main_RODATA_END - main_RODATA_START);

        main_DATA_START = .;
        KEEP(BUILD_DIR/asm/main/data/main.data.s.o(.data .data.*));
        BUILD_DIR/src/main/toilet_data.c.o(.data .data.*);
        BUILD_DIR/src/main/door_mapdata.c.o(.data .data.*);
        KEEP(BUILD_DIR/asm/main/data/9A868.data.s.o(.data .data.*));
        BUILD_DIR/src/main/tournament.c.o(.data .data.*);
        KEEP(BUILD_DIR/asm/main/data/A592C.data.s.o(.data .data.*));
        BUILD_DIR/src/main/sound_async.c.o(.data .data.*);
        . = ALIGN(., 4);
        KEEP(BUILD_DIR/asm/main/data/A5A24.data.s.o(.data .data.*));
        main_DATA_END = .;
        main_DATA_SIZE = ABSOLUTE(main_DATA_END - main_DATA_START);

        main_SDATA_START = .;
        KEEP(BUILD_DIR/asm/main/data/main.sdata.s.o(.sdata .sdata.*));
        BUILD_DIR/src/main/rng.c.o(.sdata .sdata.*);
        KEEP(BUILD_DIR/asm/main/data/AB590.sdata.s.o(.sdata .sdata.*));
        BUILD_DIR/src/main/sound_async.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        KEEP(BUILD_DIR/asm/main/data/AB5B4.sdata.s.o(.sdata .sdata.*));
        BUILD_DIR/src/btl/battle_setup.c.o(.sdata .sdata.*);
        . = ALIGN(., 4);
        KEEP(BUILD_DIR/asm/main/data/AB714.sdata.s.o(.sdata .sdata.*));
        BUILD_DIR/src/doo2/doo2.c.o(.sdata .sdata.*);
        KEEP(BUILD_DIR/asm/main/data/ABC18.sdata.s.o(.sdata .sdata.*));
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
        BUILD_DIR/generated/unk_0x8013BD18.sbss.s.o(.sbss.*);
        BUILD_DIR/src/main/fade.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/unk_0x8013BD90.sbss.s.o(.sbss.*);
        BUILD_DIR/src/main/rng.c.o(.sbss.*);
        BUILD_DIR/generated/unk_0x8013BFFC.sbss.s.o(.sbss.*);
        BUILD_DIR/src/main/kar.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/src/main/sound_async.c.o(.sbss.*);
        BUILD_DIR/generated/unk_0x8013C020.sbss.s.o(.sbss.*);
        BUILD_DIR/src/btl/battle_setup.c.o(.sbss.*);
        BUILD_DIR/src/btl/command_menu.c.o(.sbss.*);
        . = ALIGN(., 4);
        BUILD_DIR/generated/unk_0x8013C10C.sbss.s.o(.sbss.*);
        BUILD_DIR/src/doo2/doo2.c.o(.sbss.*);
        BUILD_DIR/generated/unk_0x8013C3E8.sbss.s.o(.sbss.*);
        BUILD_DIR/src/trn2/trn2_hp_map99.c.o(.sbss.*);
        BUILD_DIR/generated/unk_0x8013C48A.sbss.s.o(.sbss.*);
        BUILD_DIR/src/eab/eab.c.o(.sbss.*);
        main_SBSS_END = .;
        main_SBSS_SIZE = ABSOLUTE(main_SBSS_END - main_SBSS_START);
    }
    __romPos += SIZEOF(.sbss);
    __romPos = ALIGN(__romPos, 4);

    /DISCARD/ :
    {
        *(*);
    }
}
