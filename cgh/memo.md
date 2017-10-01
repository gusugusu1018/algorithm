Floating point registers
SSEレジスタ(Streaming SIMD Extensionsレジスタ)(SIMD:Single Instruction - Multiple Data)
16個の128bitレジスタ
xmm0,xmm1,xmm2, ... , xmm15
Coreiシリーズは256bitもある。
ymm0,ymm1,ymm2, ... , ymm15
ymmレジスタは対応するxmmレジスタと下位128bitを共有している。
xmmレジスタは4つのfloat、または2つのdoubleという複数の値を持つことができる。

mov命令
mov	DEST	SRC
mov命令はレジスタからメモリ、メモリからレジスタにデータをコピーする命令

