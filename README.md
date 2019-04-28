##Algorithm
Encode a 3 x 3 tic-tac-toe board into a single 18-bit string [EMPTY = 00, X = 10, O = 01]
Create a mask for all non-empty tiles in a separate 18-bit string.
Use predefined winning bitstrings (horizontal, vertical, diagnol)
`/* 01 00 00 00 01 00 00 00 01 */ 0x10101,`
`/* 00 00 01 00 01 00 01 00 00 */ 0x01110,`
`/* 01 01 01 00 00 00 00 00 00 */ 0x15000,` shift thrice to drop a row
`/* 01 00 00 01 00 00 01 00 00 */ 0x10410,` shift once to move a column

