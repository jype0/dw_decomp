# This file is used by modern gas.
# Defines the expected assembly macros

# Evaluate this file only once in case it's included more than once
.ifndef _MACRO_INC_GUARD
.internal _MACRO_INC_GUARD
.set _MACRO_INC_GUARD, 1

# A function symbol.
.macro glabel label, visibility=global
.ifndef PLAIN_SECTIONS
    .section .text.\label
.endif
    .\visibility \label
    .type \label, @function
    \label:
        .ent \label
.endm

# The end of a function symbol.
.macro endlabel label
    .size \label, . - \label
    .end \label
.endm

# An alternative entry to a function.
.macro alabel label, visibility=global
    .\visibility \label
    .type \label, @function
    \label:
        .aent \label
.endm

# A label referenced by an error handler table.
.macro ehlabel label, visibility=global
    .\visibility \label
    \label:
.endm


# A label referenced by a jumptable.
.macro jlabel label, visibility=global
.ifndef LOCAL_JLABELS
    .\visibility \label
.endif
    \label:
.endm


# A data symbol.
.macro dlabel label, visibility=global
    .\visibility \label
    .type \label, @object
    \label:
.endm

# End of a data symbol.
.macro enddlabel label
    .size \label, . - \label
.endm

# A data symbol in a section of its own.
.macro dlabel_section label, visibility=global
.ifndef PLAIN_SECTIONS
    .pushsection %S.\label
.endif
    .\visibility \label
    .type \label, @object
    \label:
.endm

# End of a data symbol in a section of its own.
.macro enddlabel_section label
    .size \label, . - \label
.ifndef PLAIN_SECTIONS
    .popsection
.endif
.endm


# Label to signal the symbol haven't been matched yet.
.macro nonmatching label, size=0
    .global \label\().NON_MATCHING
    .type \label\().NON_MATCHING, @object
    .size \label\().NON_MATCHING, \size
    \label\().NON_MATCHING:
.endm

.include "gte_macros.inc"

.endif
