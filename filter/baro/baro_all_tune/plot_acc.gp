set grid           # 目盛線を描画することを指定
set xlabel "microsec"  # x軸の見出しを指定
set ylabel "vel"           # 第一y軸の見出しを指定
# 凡例
set key top left   # 凡例は左上に描画
plot 'acc.txt' using 1:2 title 'vel' lw 1 with lines,\
	'acc.txt' using 1:3 title 'crt' lw 2 with lines,\
	'acc.txt' using 1:4 title 'bw' lw 2 with lines,\
	; pause -1"
