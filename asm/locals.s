.fileopt    comment, "ZP bytes reserved for general-purpose locals"
.fileopt    author,  "Max Bane"

.segment "ZEROPAGE"
; reserve some bytes of zeropage for general-purpose "local" vars, two of
; which are sized to hold addresses for indirect referencing
.exportzp local_0
local_0:  .res 1
.exportzp local_1
local_1:  .res 1
.exportzp local_2
local_2:  .res 1
.exportzp local_3
local_3:  .res 1

.exportzp addr_0
addr_0:   .res 2
.exportzp addr_1
addr_1:   .res 2

